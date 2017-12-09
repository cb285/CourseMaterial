#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* Future stuff
	Fix error handling (it is a mess)
	return set pixel value from RGB plane
*/

#ifndef UEIMAGE_H
#define UEIMAGE_H

// Data types used in prototypes
enum padmode {UEIM_ZERO, UEIM_SAME};
enum imgtype {UEIM_BAW=1, UEIM_GRY, UEIM_COL};
enum colplane {UEIM_RED, UEIM_GRN, UEIM_BLU};
typedef enum padmode padmode_t;
typedef enum imgtype imgtype_t;
typedef enum colplane cplane_t;
typedef unsigned char *ueimage_pixp;

struct ueimage {
	int width;
	int height;
	int pixr, pixc;   // current pixel location
	imgtype_t imtype;
	ueimage_pixp image;
	int *image_row_index;
	int image_plane_index[3];
	double *mask;
	int *hood;
	padmode_t pmode;
	cplane_t cplane;
	int mr_north, mr_south, mc_east, mc_west;
	int hr_north, hr_south, hc_east, hc_west;
	int error;
};

typedef struct ueimage ueimage_t;

#define GET_RED_VALUE(p) (p>>16)
#define GET_GRN_VALUE(p) ((p>>8) & 0xFF)
#define GET_BLU_VALUE(p) (p & 0xFF)

#define SET_RED_VALUE(r, vp) (*vp = ((*vp)&0x00FFFF) | (r&0xFF)<<16)
#define SET_GRN_VALUE(g, vp) (*vp = ((*vp)&0xFF00FF) | (g&0xFF)<<8)
#define SET_BLU_VALUE(b, vp) (*vp = ((*vp)&0xFFFF00) | (b&0xFF))
#define SET_RGB_VALUE(r, g, b) ((r<<16)+(g<<8)+b)

// Initialization
void ueimage_init_image(struct ueimage *x);

// Image info getters/setters
int ueimage_get_width(struct ueimage *x);
int ueimage_get_height(struct ueimage *x);
void ueimage_set_plane(struct ueimage *x, cplane_t pl);
cplane_t ueimage_get_plane(struct ueimage *x);
void ueimage_set_pad_mode(struct ueimage *x, padmode_t pm);
padmode_t ueimage_get_pad_mode(struct ueimage *x);
imgtype_t ueimage_get_image_type(struct ueimage *x);

// Image creation
// via File I/O
void ueimage_read_image(struct ueimage *x, char filename[]);
void ueimage_write_image(struct ueimage *x, char filename[]);

// Image creation
void ueimage_new_image(struct ueimage *x, int rows, int cols, imgtype_t it);
void ueimage_new_zero_image(struct ueimage *x, int rows, int cols, imgtype_t it);
void ueimage_copy_image(struct ueimage *x, struct ueimage *y);
void ueimage_clone_image(struct ueimage *x, struct ueimage *y);


// Pixel access
int ueimage_get_value(struct ueimage *x, int row, int col);
int ueimage_get_value_pad(struct ueimage *x, int row, int col);
void ueimage_set_value(struct ueimage *x, int row, int col, int v);
void ueimage_set_value_pad(struct ueimage *x, int row, int col, int v);
int ueimage_get_plane_value(struct ueimage *x, int row, int col);
void ueimage_set_plane_value(struct ueimage *x, int row, int col, int v);

// Pixel iterators
int ueimage_set_pix_rc(struct ueimage *s, int r, int c);
int ueimage_pix_next(struct ueimage *s);
int ueimage_pix_incr(struct ueimage *s);
int ueimage_pix_incc(struct ueimage *s);

int ueimage_get_pix_value(struct ueimage *s);
int ueimage_get_pix_value_next(struct ueimage *s);
int ueimage_get_pix_value_incr(struct ueimage *s);
int ueimage_get_pix_value_incc(struct ueimage *s);

int ueimage_set_pix_value(struct ueimage *s, int v);
int ueimage_set_pix_value_next(struct ueimage *s, int v);
int ueimage_set_pix_value_incr(struct ueimage *s, int v);
int ueimage_set_pix_value_incc(struct ueimage *s, int v);


// Processing
void ueimage_set_mask(struct ueimage *x, void *mask, int mr, int mc);
double ueimage_apply_mask_pad(struct ueimage *x, int row, int col);
int *ueimage_init_hood(struct ueimage *x, int mr, int mc);
int *ueimage_get_hood_pad(struct ueimage *x, int row, int col);

// Lower level method

// Image manipulation
void ueimage_zero_raster(struct ueimage *x);
void ueimage_copy_raster(struct ueimage *x, struct ueimage *y);

// Memory handling
void ueimage_alloc_raster(struct ueimage *x);
void ueimage_free_image(struct ueimage *x);

// File I/O
void ueimage_read_header(struct ueimage *x, FILE *fp);
void ueimage_read_raster(struct ueimage *x, FILE *fp);
void ueimage_write_header(struct ueimage *x, FILE *fp);
void ueimage_write_raster(struct ueimage *x, FILE *fp);

// All source code here
void ueimage_init_image(struct ueimage *x) {
	x->width = 0;
	x->height = 0;
	x->pixr = 0;
	x->pixc = 0;
	x->imtype = 0;
	x->image = NULL;
	x->image_row_index = NULL;
	int p;
	for(p=0; p<3; p++) {
		x->image_plane_index[p] = 0;
	}
	x->mask = NULL;
	x->hood = NULL;
	x->pmode = UEIM_ZERO;
	x->cplane = UEIM_RED;
	x->mr_north = 0;
	x->mr_south = 0;
	x->mc_east = 0;
	x->mc_west = 0;
	x->error = 0;
}

static void ueimage_skip_to_nonwhite(FILE *fp)
{
	int c;

	// Skip over whitespace
	while(isspace(c = fgetc(fp)))
		;
	// Skip any comment lines
	if(c == '#') {
		c = fgetc(fp);
		while (c != '\r' && c != '\n')
			c = fgetc(fp);
		// Got newline or linefeed, skip any whitespace
		ueimage_skip_to_nonwhite(fp);
	}
	else {
		// Push back last character
		ungetc(c, fp);
	}
}

void ueimage_read_image(struct ueimage *x, char file[]) {
	FILE *fp = fopen(file, "rb");
	if (!fp) {
		x->error = 10;
		fprintf(stderr, "error: could not open file %s\n", file);
		return;
	}

	ueimage_read_header(x, fp);
	ueimage_alloc_raster(x);
	ueimage_read_raster(x, fp);
	fclose(fp);
}

void ueimage_write_image(struct ueimage *x, char file[]) {
	FILE *fp = fopen(file, "wb");
	if (!fp) {
		x->error = 10;
		fprintf(stderr, "error: could not open file %s\n", file);
		return;
	}
	ueimage_write_header(x, fp);
	ueimage_write_raster(x, fp);
	fclose(fp);
}

void ueimage_read_header(struct ueimage *x, FILE *fp)
{
	char magic[2];
	int n = fread(magic, sizeof(char), 2, fp);
	if (n!=2) {
		x->error = 12;
		fprintf(stderr, "error: unable to read file header\n");
	}

	if (magic[0]== 'P' && magic[1] == '5') {
		x->imtype = UEIM_GRY;
	}
	else if (magic[0]== 'P' && magic[1] == '6') {
		x->imtype = UEIM_COL;
	}
	else {
		x->error = 14;
		fprintf(stderr, "error: input file is not a supported file.\n");
	}

	ueimage_skip_to_nonwhite(fp);
	fscanf(fp, " %d", &x->width);

	ueimage_skip_to_nonwhite(fp);
	fscanf(fp, " %d", &x->height);

	ueimage_skip_to_nonwhite(fp);
	// scan but ignore maxval
	fscanf(fp, " %*d");
}

void ueimage_write_header(struct ueimage *x, FILE *fp) {
	if(x->imtype == UEIM_GRY) {
		fwrite("P5", sizeof(char), 2, fp);
	}
	else {
		fwrite("P6", sizeof(char), 2, fp);
	}
	fprintf(fp, "\n# Created by ueimage writer\n");
	fprintf(fp, "%d %d\n", x->width, x->height);
	fprintf(fp, "%d\n", 255);
}

int ueimage_get_width(struct ueimage *x) {
	return x->width;
}

int ueimage_get_height(struct ueimage *x) {
	return x->height;
}

imgtype_t ueimage_get_image_type(struct ueimage *x)
{
	return x->imtype;
}

void ueimage_alloc_raster(struct ueimage *x){
	if(x->image != NULL)
		free(x->image);

	if(x->imtype == UEIM_GRY) {
		size_t numels = x->width*x->height;
		x->image = (unsigned char *)malloc(numels);
		if(x->image == NULL) {
			x->error = 21;
			fprintf(stderr, "error: can not allocate space for image.\n");
			return;
		}
	}
	else {
		size_t numels = 3*x->width*x->height;
		x->image = (unsigned char *)malloc(numels);
		if(x->image == NULL) {
			x->error = 21;
			fprintf(stderr, "error: can not allocate space for image.\n");
			return;
		}
        int p;
		for(p=0; p<3; p++)
			x->image_plane_index[p] = p*x->width*x->height;
	}


	if(x->image_row_index != NULL)
		free(x->image_row_index);
	x->image_row_index = (int *)malloc(x->height*sizeof(int));
	if(x->image_row_index == NULL) {
		x->error = 22;
		fprintf(stderr, "error: can not allocate space for image_row_index.\n");
		return;

	}
	int row;
	for(row=0; row<x->height; row++) {
		x->image_row_index[row] = x->width*row;
	}
}

void ueimage_read_raster(struct ueimage *x, FILE *fp){

	ueimage_skip_to_nonwhite(fp);

	if(x->imtype == UEIM_GRY) {
		size_t numels = x->width*x->height;
		size_t n = fread(x->image, sizeof(unsigned char), numels, fp);
		if(n != numels) {
			x->error = 22;
			fprintf(stderr, "error: can not read image data.\n");
			return;
		}
	}
	else {
		// Store pixels in RGB plane order
		unsigned char pixel[3];
		int npels, i;
		for (npels = 0; npels < x->width*x->height; npels++) {
			fread(pixel, sizeof(unsigned char), 3, fp);
			for(i=0; i<3; i++) {
				*(x->image + x->image_plane_index[i] + npels) = pixel[i];
			}
		}
	}
}

void ueimage_write_raster(struct ueimage *x, FILE *fp)
{
	if(x->imtype == UEIM_GRY) {
		size_t numels = x->width*x->height;
		size_t n = fwrite(x->image, sizeof(unsigned char), numels, fp);
		if(n != numels) {
			x->error = 32;
			fprintf(stderr, "error: can not write image data.\n");
			return;
		}
	}
	else {
		// must write out in pixel order
		unsigned char pixel[3];
		int i, npels;
		for (npels = 0; npels < x->width*x->height; npels++) {
			for(i=0; i<3; i++) {
				pixel[i] = *(x->image + x->image_plane_index[i] + npels);
			}
			fwrite(pixel, sizeof(unsigned char), 3, fp);
		}
	}
}

int ueimage_get_value(struct ueimage *x, int row, int col){
	if(x->imtype == UEIM_GRY)
		return *(x->image + x->image_row_index[row] + col);
	else {
		int r = *(x->image + x->image_plane_index[0] + x->image_row_index[row] + col);
		r <<= 8;
		r += *(x->image + x->image_plane_index[1] + x->image_row_index[row] + col);
		r <<= 8;
		r += *(x->image + x->image_plane_index[2] + x->image_row_index[row] + col);
		return r;
	}

}

void ueimage_set_value(struct ueimage *x, int row, int col, int v){
	if(x->imtype == UEIM_GRY)
		*(x->image + x->image_row_index[row] + col) = v;
	else {
		*(x->image + x->image_plane_index[0] + x->image_row_index[row] + col) = (v >> 16);
		*(x->image + x->image_plane_index[1] + x->image_row_index[row] + col) = (v >> 8) & 0xFF;
		*(x->image + x->image_plane_index[2] + x->image_row_index[row] + col) = v & 0xFF;
	}
}

int ueimage_get_plane_value(struct ueimage *x, int row, int col){
	if(x->imtype == UEIM_GRY)
		return *(x->image + x->image_row_index[row] + col);
	else {
		if(x->cplane == UEIM_GRN) {
			return *(x->image + x->image_plane_index[1] + x->image_row_index[row] + col);
		}
		else if(x->cplane == UEIM_BLU) {
			return *(x->image + x->image_plane_index[2] + x->image_row_index[row] + col);
		}
		else {
			return *(x->image + x->image_plane_index[0] + x->image_row_index[row] + col);
		}
	}
}

void ueimage_set_plane_value(struct ueimage *x, int row, int col, int v){
	if(x->imtype == UEIM_GRY)
		*(x->image + x->image_row_index[row] + col) = v;
	else {
		if(x->cplane == UEIM_GRN) {
			*(x->image + x->image_plane_index[1] + x->image_row_index[row] + col) = v;
		}
		else if(x->cplane == UEIM_BLU) {
			*(x->image + x->image_plane_index[2] + x->image_row_index[row] + col) = v;
		}
		else {
			*(x->image + x->image_plane_index[0] + x->image_row_index[row] + col) = v;
		}
	}
}

int ueimage_get_value_pad(struct ueimage *x, int row, int col){
	if(x->pmode == UEIM_ZERO) {
		if (row < 0 || row >= x->height)
			return 0;
		else if (col < 0 || col >= x->width)
			return 0;
	}
	else {
		if (row < 0) row = 0;
		else if (row >= x->height)
			row = x->height-1;

		if (col < 0) col = 0;
		else if (col >= x->width)
			col = x->width-1;
	}
	if(x->imtype == UEIM_GRY)
		return *(x->image + x->image_row_index[row] + col);
	else {
		int r = *(x->image + x->image_plane_index[0] + x->image_row_index[row] + col);
		r <<= 8;
		r += *(x->image + x->image_plane_index[1] + x->image_row_index[row] + col);
		r <<= 8;
		r += *(x->image + x->image_plane_index[2] + x->image_row_index[row] + col);
		return r;
	}
}

void ueimage_set_value_pad(struct ueimage *x, int row, int col, int v){
	if (row >= 0 && row < x->height && col >= 0 && col < x->width) {
		if(x->imtype == UEIM_GRY)
			*(x->image + x->image_row_index[row] + col) = v;
		else {
			*(x->image + x->image_plane_index[0] + x->image_row_index[row] + col) = (v >> 16);
			*(x->image + x->image_plane_index[1] + x->image_row_index[row] + col) = (v >> 8) & 0xFF;
			*(x->image + x->image_plane_index[2] + x->image_row_index[row] + col) = v & 0xFF;
		}
	}
	return;
}

int ueimage_set_pix_rc(struct ueimage *s, int r, int c) {
	s->pixr = r;
	s->pixc = c;
	return s->pixr >= 0 && s->pixc >= 0 && s->pixr < s->height && s->pixc < s->width;
}

int ueimage_pix_next(struct ueimage *s) {
	s->pixc++;
	if (s->pixc >= s->width) {
		s->pixr++; s->pixc = 0;
	}
	return s->pixr < s->height && s->pixc < s->width;
}

int ueimage_pix_prev(struct ueimage *s) {
	s->pixc--;
	if (s->pixc < 0) {
		s->pixr--; s->pixc = s->height-1;
	}
	return s->pixr >= 0 && s->pixc >= 0;
}

int ueimage_pix_incc(struct ueimage *s) {
	s->pixc++;
	if (s->pixc >= s->width) {
		s->pixr++; s->pixc = 0;
	}
	return s->pixr < s->height && s->pixc < s->width;
}

int ueimage_pix_incr(struct ueimage *s) {
	s->pixr++;
	if (s->pixr >= s->height) {
		s->pixc++; s->pixr = 0;
	}
	return s->pixr < s->height && s->pixc < s->width;
}

int ueimage_get_pix_value(struct ueimage *s) {
	if (s->pixr >= s->height || s->pixc >= s->width)
		return -1;
	else
		return ueimage_get_value(s, s->pixr, s->pixc);
}

int ueimage_set_pix_value(struct ueimage *s, int v) {
	if (s->pixr >= s->height || s->pixc >= s->width)
		return -1;
	else {
		ueimage_set_value(s, s->pixr, s->pixc, v);
		return v;
	}
}

int ueimage_get_pix_value_next(struct ueimage *s) {
	int v = ueimage_get_pix_value(s);
	s->pixc++;
	if (s->pixc >= s->width) {
		s->pixr++; s->pixc = 0;
	}
	return v;
}

int ueimage_get_pix_value_incr(struct ueimage *s) {
	int v = ueimage_get_pix_value(s);
	s->pixr++;
	if (s->pixr >= s->height) {
		s->pixc++; s->pixr = 0;
	}
	return v;
}

int ueimage_get_pix_value_incc(struct ueimage *s) {
	int v = ueimage_get_pix_value(s);
	s->pixc++;
	if (s->pixc >= s->width) {
		s->pixr++; s->pixc = 0;
	}
	return v;
}

int ueimage_set_pix_value_next(struct ueimage *s, int v) {
	int err = ueimage_set_pix_value(s, v);
	if(err != -1) {
		s->pixc++;
		if (s->pixc >= s->width) {
			s->pixr++; s->pixc = 0;
		}
	}
	return err;
}

int ueimage_set_pix_value_incc(struct ueimage *s, int v) {
	int err = ueimage_set_pix_value(s, v);
	if(err != -1) {
		s->pixc++;
		if (s->pixc >= s->width) {
			s->pixr++; s->pixc = 0;
		}
	}
	return err;
}

int ueimage_set_pix_value_incr(struct ueimage *s, int v) {
	int err = ueimage_set_pix_value(s, v);
	if(err != -1) {
		s->pixr++;
		if (s->pixr >= s->height) {
			s->pixc++; s->pixr = 0;
		}
	}
	return err;
}

void ueimage_new_image(struct ueimage *x, int rows, int cols, imgtype_t it) {
	x->height = rows;
	x->width = cols;
	x->imtype = it;
	ueimage_alloc_raster(x);
}

void ueimage_new_zero_image(struct ueimage *x, int rows, int cols, imgtype_t it) {
	x->height = rows;
	x->width = cols;
	x->imtype = it;
	ueimage_alloc_raster(x);
	ueimage_zero_raster(x);
}

void ueimage_copy_image(struct ueimage *x, struct ueimage *y) {
	y->height = x->height;
	y->width = x->width;
	y->imtype = x->imtype;
	ueimage_alloc_raster(x);
	ueimage_copy_raster(x, y);
}

void ueimage_clone_image(struct ueimage *x, struct ueimage *y) {
	y->height = x->height;
	y->width = x->width;
	y->imtype = x->imtype;
	ueimage_alloc_raster(y);
}

void ueimage_zero_raster(struct ueimage *x) {
    int row, col;
	for(row=0; row<x->height; row++)
		for(col=0; col<x->width; col++)
			ueimage_set_value(x, row, col, 0);
}

void ueimage_copy_raster(struct ueimage *x, struct ueimage *y) {
    int row, col;
	for(row=0; row<x->height; row++)
		for(col=0; col<x->width; col++)
			ueimage_set_value(y, row, col, ueimage_get_value(x, row, col));
}

void ueimage_free_image(struct ueimage *x){
	if(x->image != NULL)
		free(x->image);
	x->image = NULL;
	if(x->image_row_index != NULL)
		free(x->image_row_index);
	x->image_row_index = NULL;
	if(x->mask != NULL)
		free(x->mask);
	x->mask = NULL;
	if(x->hood != NULL)
		free(x->hood);
	x->hood = NULL;
}

int *ueimage_init_hood(struct ueimage *s, int mr, int mc) {
	if(s->hood != NULL)
		free(s->hood);
	s->hood = (int *)malloc(mr*mc*sizeof(int));
	s->hr_north = -(mr/2);
	s->hr_south = s->hr_north + mr - 1;
	s->hc_east = -(mc/2);
	s->hc_west = s->hc_east + mc - 1;
	return s->hood;
}

int *ueimage_get_hood_pad(struct ueimage *x, int row, int col) {
	int ind = 0;
	int r, c;
	if(x->imtype == UEIM_GRY) {
		for(r=x->hr_north; r<=x->hr_south; r++) {
			for(c=x->hc_east; c<=x->hc_west; c++) {
				x->hood[ind++] = ueimage_get_value_pad(x, row+r, col+c);
			}
		}
	}
	else {
		int v;
		for(r=x->hr_north; r<=x->hr_south; r++) {
			for(c=x->hc_east; c<=x->hc_west; c++) {
				v = ueimage_get_value_pad(x, row+r, col+c);
				if(x->cplane == UEIM_GRN) {
					x->hood[ind++] = GET_GRN_VALUE(v);
				}
				else if(x->cplane == UEIM_BLU) {
					x->hood[ind++] = GET_BLU_VALUE(v);
				}
				else {
					x->hood[ind++] = GET_RED_VALUE(v);
				}
			}
		}
	}
	return x->hood;
}

void ueimage_set_mask(struct ueimage *s, void *mask, int mr, int mc) {
	double *m = mask;
	int r, c;
	if(s->mask != NULL)
		free(s->mask);
	s->mask = (double *)malloc(mr*mc*sizeof(double));
	s->mr_north = -(mr/2);
	s->mr_south = s->mr_north + mr - 1;
	s->mc_east = -(mc/2);
	s->mc_west = s->mc_east + mc - 1;
	for (r=0; r<mr; r++)
		for(c=0; c<mc; c++)
			s->mask[r*mc + c] = m[r*mc + c];
}

void ueimage_set_pad_mode(struct ueimage *x, padmode_t pm) {
	x->pmode = pm;
}

padmode_t ueimage_get_pad_mode(struct ueimage *x) {
	return x->pmode;
}

cplane_t ueimage_get_plane(struct ueimage *x) {
	return x->cplane;
}

void ueimage_set_plane(struct ueimage *x, cplane_t pl) {
	x->cplane = pl;
}

double ueimage_apply_mask_pad(struct ueimage *x, int row, int col) {
	double sum = 0;
	int maskind = 0;
	int r, c;

	if (x->mask == NULL) {
		fprintf(stderr, "error: mask not initialized\n");
		return 0.;
	}

	if(x->imtype == UEIM_GRY) {
		for(r=x->mr_north; r<=x->mr_south; r++) {
			for(c=x->mc_east; c<=x->mc_west; c++) {
				sum += ueimage_get_value_pad(x, row+r, col+c)*x->mask[maskind++];
			}
		}
	}
	else {
		int val;
		for(r=x->mr_north; r<=x->mr_south; r++) {
			for(c=x->mc_east; c<=x->mc_west; c++) {
				val = ueimage_get_value_pad(x, row+r, col+c);
				if (x->cplane == UEIM_BLU)
					sum += GET_BLU_VALUE(val)*x->mask[maskind++];
				else if (x->cplane == UEIM_GRN)
					sum += GET_GRN_VALUE(val)*x->mask[maskind++];
				else
					sum += GET_RED_VALUE(val)*x->mask[maskind++];
			}
		}
	}
	return sum;
}

#endif
