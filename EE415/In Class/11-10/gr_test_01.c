// Process in place

#include "ueimage.h"

int main(int argc, char *argv[])
{
	// Allocate and initialize image data structures
	ueimage_t x;
	ueimage_init_image(&x);
	
	// Read in image x
	ueimage_read_image(&x, "clown.pgm");
	
	int width = ueimage_get_width(&x);
	int height = ueimage_get_height(&x);
	// Loop over all pixels
	int v;
	for(int row=0; row<height; row++) {
		for(int col=0; col<width; col++) {
			v = ueimage_get_value(&x, row, col);
			ueimage_set_value(&x, row, col, 255 - v);
		}
	}

	// Write output image to file
	ueimage_write_image(&x, "out01.pgm");
	
	// Free memory
	ueimage_free_image(&x);

	return 0;
}
