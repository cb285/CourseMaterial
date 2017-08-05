/* File: main.c
 * rotates a ppm image to the left 90 degrees.
 * --------------------------------------------------------------
 * Class: CS 210                        Instructor: Dr. Deborah Hwang
 * Assignment: Programming Assignment 7 Date assigned:  4/7/15
 * Programmer: Clayton Brutus           Date completed: 4/12/15
 */

#include <stdio.h>      /* printf, scanf, fscanf, fscanf */
#include <stdlib.h>
#include <string.h>     /* strcmp */

#define MAX_ROWS 500        /* max image size is 500 x 500 pixels */
#define MAX_COLUMNS 500

/* define type pixel_t to contain red green and blue color values */
typedef struct
{
    int red, green, blue;
} pixel_t;

void read_ppm_image (pixel_t image[MAX_ROWS][MAX_COLUMNS], int *width, int *height, int *max_color);
void rotate_image_left (pixel_t original[MAX_ROWS][MAX_COLUMNS], pixel_t result[MAX_ROWS][MAX_COLUMNS], int rotated_width, int rotated_height);
void write_ppm_image (pixel_t image[MAX_ROWS][MAX_COLUMNS], int width, int height, int max_color);

int main()
{
    /* declare arrays for storing image data */
    static pixel_t input_image[MAX_ROWS][MAX_COLUMNS];
    static pixel_t output_image[MAX_ROWS][MAX_COLUMNS];

    /* declare variables for storing input and output image width, height, and max color value */
    int width, height, max_color, rotated_width, rotated_height;

    /* display program header */
    printf("PPM Image Rotator\n");

    /* read input image */
    read_ppm_image(input_image, &width, &height, &max_color);

    /* define output image width and height */
    rotated_width = height;
    rotated_height = width;

    /* rotate input image */
    rotate_image_left(input_image, output_image, rotated_height, rotated_width);

    /* write output image to file */
    write_ppm_image(output_image, rotated_width, rotated_height, max_color);

    return 0;
}

/*  Function: read_ppm_image
    gets input file name from user, then reads the file into an array
 */
void read_ppm_image (pixel_t image[MAX_ROWS][MAX_COLUMNS],  /* P'back: array containing input image data */
                     int *width,                            /* P'back: width of the input image          */
                     int *height,                           /* P'back: height of the imput image         */
                     int *max_color)                        /* P'back: max color value                   */
{
    /* declare string for storing input file name */
    char filename[FILENAME_MAX];

    /* declare input file pointer */
    FILE *input_file;

    /* declare string for storing "magic" file header */
    char magic[2];

    /* declare loop counting variables */
    int i,j;

    /* ask user for input image file name */
    printf("Enter the name of a PPM image file: ");
    scanf("%s", filename);

    /* open the file for reading */
    input_file = fopen (filename, "r");

    /* check if the file is valid */
    if (input_file == NULL)
    {
        /* if file is invalid: display error message and end program */
        printf ("Error: unable to open file %s\n", filename);
        exit (1);
    }

    /* check if file is a PPM image */
    fscanf (input_file, "%s", magic);

    if(strcmp(&filename[strlen(filename) - 4], ".ppm") || strcmp(magic, "P3"))
    {
        /* if file is not a PPM image: display error and exit program */
        printf("Error: %s is not a PPM image file\n", filename);
        exit (1);
    }

    /* read in width and height of input file */
    fscanf (input_file, "%d %d", width, height);
    fscanf (input_file, "%d", max_color);

    /* store input image data in an array */
    for(i=0; i < *height; i++)
        for(j=0; j < *width; j++)
            fscanf(input_file, "%d %d %d", &image[i][j].red, &image[i][j].green, &image[i][j].blue);

    /* close the input file */
    fclose(input_file);
}

/*  Function: rotate_image_left
    rotates an array of ppm image data to the left 90 degrees
 */
void rotate_image_left (pixel_t original[MAX_ROWS][MAX_COLUMNS],  /* Rec'd: array of image data             */
                        pixel_t result[MAX_ROWS][MAX_COLUMNS],    /* P'back: array of rotated image data    */
                        int width, int height)    /* Rec'd: width and height of output image */
{
    /* declare loop counter variables */
    int i, j;

    /* rotate data and write to resulting array */

    for (i = 0; i < width; i++)
        for (j = 0; j < height; j++)
            result[width - i - 1][j] = original[j][i];
}

/*  Function: write_ppm_image
    writes an array of image data to a user specified ppm image file
 */
void write_ppm_image (pixel_t image[MAX_ROWS][MAX_COLUMNS], /* Rec'd: array of image data                */
                      int width, int height, int max_color)                /* Rec'd: width and height of image to print */
{
    /* declare output file pointer */
    FILE *output_file;

    /* declare string for output file name */
    char filename[FILENAME_MAX];

    /* declare loop count variables */
    int i, j;

    /* ask user for output file name */
    printf("Enter the name of the output file: ");
    scanf("%s", filename);

    /* open output file for writing */
    output_file = fopen(filename, "w");

    /* check if file open succeeded */
    if (output_file == NULL)
    {
        /* if file open failed: display error and end program. */
        printf("Error.\n");
        exit(1);
    }

    /* write ppm image file header */
    fprintf(output_file, "%s\n", "P3");
    fprintf(output_file, "%d %d\n", width, height);
    fprintf(output_file, "%d\n", max_color);

    /* write ppm image data to the file */
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
            fprintf(output_file, "%d %d %d\n", image[i][j].red, image[i][j].green, image[i][j].blue);

    /* close output file */
    fclose(output_file);
}
