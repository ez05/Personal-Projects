/*****************************************************************************
 *
 *                              CV_DCTfloats.c
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our CV_DCTfloats module. 
 *     The purpose of this module is to convert between a UArray2 of CV colors
 *     and a UArray2 of floats resulting from a discrete cosine transformation.
 *     This module contains two public functions, one that takes in a UArray2 
 *     of CV color values and returns a UArray2 of DCT floats values, and one 
 *     that takes in a UArray2 of DCT floats values and returns a UArray2 of 
 *     CV color values. Both these functions rely on mapping private apply 
 *     functions over the respective UArray2s to convert between the different 
 *     values. In compression data is lost due to imprecise nature of floating 
 *     point math. Additionally, data is lost in compression through the DCT
 *     conversion which stores only the color gradient in a block of 4 pixels,
 *     not their individual RGB values.
 *     
 *
 *****************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CV_DCTfloats.h"

/* this is the struct definition for the DCT_floats struct */
#include "DCT_floats.h"
/* this is the struct definition for the CV_colors struct */
#include "CV_colors.h"

/* 
 * struct to store the CV_colors in a 2 by 2 block of pixels
 * pix1:  a CV_colors struct representing one pixel
 * pix2:  a CV_colors struct representing one pixel
 * pix3:  a CV_colors struct representing one pixel
 * pix4:  a CV_colors struct representing one pixel
 */
struct CV_block {
        struct CV_colors pix1, pix2, pix3, pix4;  
};

/*
 * private helper functions, functions details are included in the function
 * contracts respectively
 */
static void       to_dctfloats_apply(int col, int row, UArray2_T array2, 
                                     void *val, void *dctfloats_array2);
static void       to_cv_apply(int col, int row, UArray2_T array2, void *val, 
                              void *cv_array2);
struct DCT_floats CV_to_DCTfloats(struct CV_block);
struct CV_block   DCTfloats_to_CV(struct DCT_floats dct);
bool              is_corner(int col, int row);

/* FUNCTION:  CV_DCTfloats_compress
 * Purpose:   Converts an UArray2 of component video color values to an UArray2
 *            of DCT floats
 * Arg:       cv_colors: pointer to an instance of UArray2 that stores the 
 *                       CV color values
 * Returns:   Pointer to an UArray2 that stores the DCT float values
 * Effect:    initializes a new UArray2 and recycles the CV colors UArray2.
 *            The dimensions of the new UArray2 of DCT floats are half the
 *            dimensions of the UArray2 of CV colors, because each group of
 *            DCT floats represents a 2 by 2 block of pixels.
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T CV_DCTfloats_compress(UArray2_T cv_colors)
{
        assert(cv_colors != NULL);

        /* initialize an uarray2 of DCT_floats structs */
        unsigned width = UArray2_width(cv_colors) / 2;
        unsigned height = UArray2_height(cv_colors) / 2;
        unsigned size = sizeof(struct DCT_floats);
        UArray2_T dct_floats = UArray2_new(width, height, size);

        UArray2_map_row_major(cv_colors, to_dctfloats_apply, dct_floats);

        UArray2_free(&cv_colors);
        
        return dct_floats;
}

/* FUNCTION:  CV_DCTfloats_decompress
 * Purpose:   Converts an UArray2 of DCT floats to an UArray2 
 *            of CV colors
 * Arg:       dct_floats: pointer to an instance of UArray2 that stores the 
 *                        DCT float values
 * Returns:   Pointer to an UArray2 that stores the CV color values
 * Effect:    initializes a new UArray2 and recycles the DCT floats UArray2.
 *            The dimensions of the new UArray2 of CV colors are double the
 *            dimensions of the UArray2 of DCT floats, because each group of
 *            DCT floats represents a 2 by 2 block of pixels
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T CV_DCTfloats_decompress(UArray2_T dct_floats)
{
        assert(dct_floats != NULL);

        /* initialize an uarray2 of CV_colors structs */
        unsigned width = UArray2_width(dct_floats) * 2;
        unsigned height = UArray2_height(dct_floats) * 2;
        unsigned size = sizeof(struct CV_colors);
        UArray2_T cv_colors = UArray2_new(width, height, size);

        UArray2_map_row_major(dct_floats, to_cv_apply, cv_colors);

        UArray2_free(&dct_floats);
        
        return cv_colors;
}

/* FUNCTION:  to_dctfloats_apply
 * Purpose:   For each pixel in the UArray2 of CV colors, if it is top left 
 *            pixel in a 2 by 2 pixel block, take the CV color values of every 
 *            pixel in that block and convert them to a single instance of DCT
 *            floats
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of CV colors
 *            val: a pointer to the current pixel containing a CV colors struct
 *            dctfloats_array2: pointer to the UArray2 of DCT floats
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a block of CV color
 *            values into a instance of DCT floats and adds it to the UArray2
 *            of DCT floats
 * Error:     N/A
 */
static void to_dctfloats_apply(int col, int row, UArray2_T array2, void *val, 
                               void *dctfloats_array2)
{
        /* Check if we are at the top left pixel of each 2 by 2 pixel block */
        if (is_corner(col, row)) {
                struct CV_block pixels;

                pixels.pix1 = /* current pixel */
                        *(struct CV_colors *)val;
                pixels.pix2 = /* the pixel to the right */
                        *(struct CV_colors *)UArray2_at(array2, col + 1, row);
                pixels.pix3 = /* the pixel below */
                        *(struct CV_colors *)UArray2_at(array2, col, row + 1);
                pixels.pix4 = /* the pixel to the bottom right */
                        *(struct CV_colors *)UArray2_at(array2, col + 1, 
                                                        row + 1);

                /* converts from the CV_colors structs to a DCT_floats struct */
                struct DCT_floats dct_floats = CV_to_DCTfloats(pixels);
                
                /* update the respective pixel in the dctfloats UArray2 */
                *(struct DCT_floats *)(UArray2_at(dctfloats_array2, col / 2, 
                                                  row / 2)) = dct_floats;
        } 
}

/* FUNCTION:  to_cv_apply
 * Purpose:   For each instance of DCT float values, convert the DCT floats into
 *            CV color values for each pixel in a 2 by 2 block of pixels and
 *            add them each to the UArray2 of CV color values
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of DCT ints (unused)
 *            val: a pointer to the current pixel containing a DCT floats struct
 *            cv_array2: pointer to the UArray2 of CV color values
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts an instance of DCT
 *            floats into a block of CV color values and adds each CV color
 *            value to the UArray2 of CV colors
 * Error:     N/A
 */
static void to_cv_apply(int col, int row, UArray2_T array2, void *val, 
                        void *cv_array2)
{
        /* converts a DCT_floats struct into a CV_block struct */
        struct DCT_floats dct_floats = (*(struct DCT_floats*)val);
        struct CV_block cv_block = DCTfloats_to_CV(dct_floats);

        /* for each pair of indices, we mutiply by 2 because the UArray2 of
           CV_colors stores individual pixel instead of a 2 by 2 pixel block */
        *(struct CV_colors *)(UArray2_at(cv_array2, col * 2, row * 2)) 
                = cv_block.pix1; /* current pixel */
                
        *(struct CV_colors *)(UArray2_at(cv_array2, (col * 2) + 1, row * 2))
                = cv_block.pix2; /* the pixel to the right */
        
        *(struct CV_colors *)(UArray2_at(cv_array2, col * 2, (row * 2) + 1)) 
                = cv_block.pix3; /* the pixel below */

        *(struct CV_colors *)(UArray2_at(cv_array2, (col * 2) + 1, 
                                         (row * 2) + 1)) 
                = cv_block.pix4; /* the pixel to the bottom right */
        
        (void) array2;
}


/* FUNCTION:  is_corner
 * Purpose:   Check if the pixel at (col, row) is at the top left corner pixel 
 *            in a 2 by 2 pixel block
 * Arg:       col: represents a column indice of a pixel
 *            row: represents a row indice of a pixel
 * Returns:   boolean indicating if the pixel is at the top left corner pixel 
 *            in a 2 by 2 pixel block;
 * Effect:    N/A
 * Error:     N/A
 */
bool is_corner(int col, int row)
{
        return ((col % 2 == 0) && (row % 2 == 0));
}


/* FUNCTION:  CV_to_DCTfloats
 * Purpose:   Convert a 2 by 2 block of CV color values into a single instance
 *            of DCT floats
 * Arg:       pixels: an initialized CV_block struct containing the CV color
 *            values of a 2 by 2 block of pixels  
 * Returns:   the DCT_floats instance
 * Effect:    N/A
 * Error:     N/A
 */
struct DCT_floats CV_to_DCTfloats(struct CV_block pixels)
{
        /* initialize a DCT_floats struct and 4 CV_colors structs from the 
           given CV_block */
        struct DCT_floats dct;
        struct CV_colors pix1 = pixels.pix1;
        struct CV_colors pix2 = pixels.pix2;
        struct CV_colors pix3 = pixels.pix3;
        struct CV_colors pix4 = pixels.pix4;
        
        /* calculate each DCT value and store it in the DCT_floats struct */
        dct.avgPb = (float)(pix4.pb + pix3.pb + pix2.pb + pix1.pb) / 4.0;
        dct.avgPr = (float)(pix4.pr + pix3.pr + pix2.pr + pix1.pr) / 4.0;
        dct.a = (float)(pix4.y + pix3.y + pix2.y + pix1.y) / 4.0;
        dct.b = (float)(pix4.y + pix3.y - pix2.y - pix1.y) / 4.0;
        dct.c = (float)(pix4.y - pix3.y + pix2.y - pix1.y) / 4.0;
        dct.d = (float)(pix4.y - pix3.y - pix2.y + pix1.y) / 4.0;

        return dct;
}

/* FUNCTION:  DCTfloats_to_CV
 * Purpose:   Convert an instance of DCT floats to a 2 by 2 block of CV color
 *            values
 * Arg:       dct: an initialized DCT_floats struct containing the DCT float
 *            values  
 * Returns:   A CV_block instance contaiing the 2 by 2 block of CV color values
 * Effect:    N/A
 * Error:     N/A
 */
struct CV_block DCTfloats_to_CV(struct DCT_floats dct)
{
        struct CV_block cv;

        /* calculate each pixel's CV color values and store them in the 
           CV_block struct */
        float y1 = dct.a - dct.b - dct.c + dct.d;
        float y2 = dct.a - dct.b + dct.c - dct.d;
        float y3 = dct.a + dct.b - dct.c - dct.d;
        float y4 = dct.a + dct.b + dct.c + dct.d;
        
        cv.pix1.y = y1;
        cv.pix2.y = y2;
        cv.pix3.y = y3;
        cv.pix4.y = y4;

        cv.pix1.pb = dct.avgPb;
        cv.pix2.pb = dct.avgPb;
        cv.pix3.pb = dct.avgPb;
        cv.pix4.pb = dct.avgPb;
        
        cv.pix1.pr = dct.avgPr;
        cv.pix2.pr = dct.avgPr;
        cv.pix3.pr = dct.avgPr;
        cv.pix4.pr = dct.avgPr;

        return cv;
}