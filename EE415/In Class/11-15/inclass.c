#include "ueimage.h"
#include <stdio.h>
#include <math.h>


ueimage_t xx_imbilinear(ueimage_t* a, double mag);
ueimage_t xx_imnearest(ueimage_t* a, double mag);


int main(int argc, char *argv[])
{
	// Allocate and initialize image data structures
	ueimage_t a;
	ueimage_init_image(&a);
	
	// Read in image x
	ueimage_read_image(&a, "Fig0222(b)(cameraman).pgm");
	
	ueimage_t b = xx_imbilinear(&a, 0.5);
	ueimage_t c = xx_imbilinear(&b, 2.0);
	ueimage_t d = xx_imnearest(&b, 2.0);
	
	// Write output image to file
	ueimage_write_image(&b, "img_b.pgm");
	ueimage_write_image(&c, "img_c.pgm");
	ueimage_write_image(&d, "img_d.pgm");
	
	// Free memory
	ueimage_free_image(&a);
	ueimage_free_image(&b);
	ueimage_free_image(&c);
	ueimage_free_image(&d);

	return 0;
}

ueimage_t xx_imbilinear(ueimage_t* img_a, double mag)
{
   	int a_width = ueimage_get_width(img_a);
	int a_height = ueimage_get_height(img_a);
	
	int b_width = (int)round(a_width*mag);
	int b_height = (int)round(a_height*mag);

	double sv = (double)(a_height-1)/(double)(b_height-1);
	double sh = (double)(a_width-1)/(double)(b_width-1);
	
	ueimage_t img_b;
	ueimage_init_image(&img_b);
	ueimage_new_image(&img_b, b_height, b_width, UEIM_GRY);
	
	ueimage_set_pad_mode(img_a, UEIM_SAME);
	
	int ra, ca;
	double dx, dy;
	int v00, v01, v10, v11;
	int a, b, c, d, v;
	
	// Loop over all pixels in output image
	for(int rb=0; rb<b_height; rb++) {
	   ra = (int)(rb*sv);
	   dx = rb*sv - ra;
	   
	   for(int cb=0; cb<b_width; cb++) {
	      ca = (int)(cb*sh);
	      dy = cb*sh - ca;

	      v00 = ueimage_get_value_pad(img_a, ra, ca);
	      v01 = ueimage_get_value_pad(img_a, ra+1, ca);
	      v10 = ueimage_get_value(img_a, ra, ca+1);
	      v11 = ueimage_get_value(img_a, ra+1, ca+1);

	      a = v10 - v00;
	      b = v01 - v00;
	      c = v11 + v00 - v01 - v10;
	      d = v00;
	      
	      v = a*dx + b*dy + c*dx*dy + d;
	      
	      ueimage_set_value(&img_b, rb, cb, v);
	   }
	}
	
	return img_b;
}


ueimage_t xx_imnearest(ueimage_t* img_a, double mag)
{
   	int a_width = ueimage_get_width(img_a);
	int a_height = ueimage_get_height(img_a);
	
	int b_width = (int)round(a_width*mag);
	int b_height = (int)round(a_height*mag);
	
	double sv = (double)(a_height-1)/(double)(b_height-1);
	double sh = (double)(a_width-1)/(double)(b_width-1);
	
	ueimage_t img_b;
	ueimage_init_image(&img_b);
	ueimage_new_image(&img_b, b_height, b_width, UEIM_GRY);
	
	ueimage_set_pad_mode(img_a, UEIM_SAME);
	
	int ra, ca, v;
	
	// Loop over all pixels in output image
	for(int rb=0; rb<b_height; rb++) {
	   ra = (int)round(rb*sv);
	   
	   for(int cb=0; cb<b_width; cb++) {
	      ca = (int)round(cb*sh);
	      
	      v = ueimage_get_value_pad(img_a, ra, ca);
	      ueimage_set_value(&img_b, rb, cb, v);
	   }
	}
	
	return img_b;
}

