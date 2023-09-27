/*****************************************************************************
 *
 *                              RGBfloats_CV.c
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our RGBfloats_CV module. 
 *     The purpose of this module is to convert between a UArray2 of RGB float 
 *     values and a UArray2 of CV color values. This module contains two public
 *     functions, one that takes in a UArray2 of RGB float values and returns a
 *     UArray2 of CV color values, and one that takes in a UArray2 of CV color
 *     values and returns a UArray2 of RGB float values. Both these functions
 *     rely on mapping private apply functions over the respective UArray2s to
 *     convert between the different values. In compression and decompression,
 *     data is lost due to imprecise nature of floating point math.
 *     
 *
 *****************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include "RGBfloats_CV.h"

/* this is the private struct definition for the RGB_floats struct */
#include "RGB_floats.h"
/* this is the private struct definition for the CV_colors struct */
#include "CV_colors.h"

/*
 * private helper functions, functions details are included in the function
 * contracts respectively
 */
static void       to_cv_apply(int col, int row, UArray2_T array2, void *val, 
                              void *cv_array2);
static void       to_RGBfloats_apply(int col, int row, UArray2_T array2, 
                                     void *val, void *rgbfloats_array2);
struct CV_colors  RGB_to_CV(struct RGB_floats RGBvals);
struct RGB_floats CV_to_RGB(struct CV_colors CVvals);

/* FUNCTION:  RGBfloats_CV_compress
 * Purpose:   Converts an uarray2 of RGB_floats to an uarray2 of component 
              video colors space
 * Arg:       RGB_floats: pointer to an instance of UArray2 that stores the 
 *                        RGB floats
 * Returns:   Pointer to an UArray2 that stores the components video color
 *            space values for each pixel
 * Effect:    initializes a new UArray2 and recycles the RGB_floats UArray2.
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T RGBfloats_CV_compress(UArray2_T RGB_floats)
{
        assert(RGB_floats != NULL);

        /* initialize an uarray2 of CV_color structs */
        unsigned width = UArray2_width(RGB_floats);
        unsigned height = UArray2_height(RGB_floats);
        unsigned size = sizeof(struct CV_colors);
        UArray2_T cv_colors = UArray2_new(width, height, size);

        UArray2_map_row_major(RGB_floats, to_cv_apply, cv_colors);
        
        UArray2_free(&RGB_floats);

        return cv_colors;
}

/* FUNCTION:  RGBfloats_CV_decompress
 * Purpose:   Converts an uarray2 of component video colors space to 
              an uarray2 of RGB floats values
 * Arg:       CV_colors: pointer to an instance of UArray2 that stores the 
 *                       component video color space 
 * Returns:   Pointer to an UArray2 that stores the RGB floats
 * Effect:    initializes a new UArray2 and recycles the CV colors UArray2.
 * Error:     Runtime error a NULL pointer is passed in
 */
UArray2_T RGBfloats_CV_decompress(UArray2_T CV_colors)
{
        assert(CV_colors != NULL);

        /* initialize an uarray2 of RGB_floats structs */
        unsigned width = UArray2_width(CV_colors);
        unsigned height = UArray2_height(CV_colors);
        unsigned size = sizeof(struct RGB_floats);
        UArray2_T RGB_Floats = UArray2_new(width, height, size);

        UArray2_map_row_major(CV_colors, to_RGBfloats_apply, RGB_Floats);

        UArray2_free(&CV_colors);

        return RGB_Floats;
}

/* FUNCTION:  to_cv_apply
 * Purpose:   For each pixel in the array of RGB floats,
 *            convert each pixel's RGB float values into component video color
 *            space values (using a private helper function) and add the pixel
 *            to the new array of CV colors
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of RGB floats (unused)
 *            val: a pointer to the current pixel containing a RGB_floats struct
 *            cv_array2: pointer to the CV_colors UArray2
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a RGB floats struct
 *            into a CV_colors struct and adds it to the UArray2 of CV colors
 * Error:     N/A
 */
static void to_cv_apply(int col, int row, UArray2_T array2, void *val, 
                        void *cv_array2)
{
        struct RGB_floats rgb_floats = *(struct RGB_floats *)val;

        /* converts from a CV_colors struct to a RGB_floats struct using a 
           private helper function */
        struct CV_colors cv_colors = RGB_to_CV(rgb_floats);
        
        *(struct CV_colors *)(UArray2_at(cv_array2, col, row)) = cv_colors;

        (void) array2;
}

/* FUNCTION:  to_RGBfloats_apply
 * Purpose:   For each pixel in the array of component video space colors
 *            convert each pixel's CV colors values to a RGB float values
 *            (using a private helper function) and add the RGB float values 
 *            to the new array of RGB float values
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to an UArray2 of CV_colors (unused)
 *            val: pointer to the current pixel containing a CV_color struct
 *            rgbfloats_array2: pointer to an UArray2 of RGB_floats structs
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a CV_colors struct
 *            into a RGB_floats struct and adds the RGB_floats struct to the 
 *            new UArray2 of RGB_floats structs
 * Error:     N/A
 */
static void to_RGBfloats_apply(int col, int row, UArray2_T array2, void *val, 
                               void *rgbfloats_array2)
{
        struct CV_colors cv_colors = *(struct CV_colors *)val;

        /* converts from a RGB_floats struct to a CV_colors struct using a 
           private helper function */
        struct RGB_floats rgb_floats = CV_to_RGB(cv_colors);
        
        *(struct RGB_floats *)(UArray2_at(rgbfloats_array2, col, row)) = 
                                                                rgb_floats;
        (void) array2;
}

/* FUNCTION:  RGB_to_CV
 * Purpose:   Convert a struct of RGB floats to a struct of component video
              color space struct
 * Arg:       RGBvals: a struct containing the floats representation of red, 
                       green, and blue colors
 * Returns:   A CV_color struct
 * Effect:    N/A
 * Error:     N/A
 */
struct CV_colors RGB_to_CV(struct RGB_floats RGBvals)
{
        float red = RGBvals.red;
        float green = RGBvals.green;
        float blue = RGBvals.blue;
        
        /* initialize a CV_color struct and compute its fields */
        struct CV_colors CVvals;
        CVvals.y = (0.299 * red) + (0.587 * green) + (0.114 * blue);
        CVvals.pb = (-0.168736 * red) - (0.331264 * green) + (0.5 * blue);
        CVvals.pr = (0.5 * red) - (0.418688 * green) - (0.081312 * blue);

        return CVvals;
}

/* FUNCTION:  CV_to_RGB
 * Purpose:   Convert a struct of component video color space struct to 
 *            a struct of RGB floats
 * Arg:       CVvals: a struct containing a component video color space struct
 * Returns:   A RGB_floats struct
 * Effect:    N/A
 * Error:     N/A
 */
struct RGB_floats CV_to_RGB(struct CV_colors CVvals)
{
        float y = CVvals.y;
        float pb = CVvals.pb;
        float pr = CVvals.pr;
        
        /* initialize a RGB_floats struct and compute its fields */
        struct RGB_floats RGBvals;
        RGBvals.red = (1.0 * y) + (0.0 * pb) + (1.402 * pr);
        RGBvals.green = (1.0 * y) - (0.344136 * pb) - (0.714136 * pr);
        RGBvals.blue = (1.0 * y) + (1.772 * pb) + (0.0 * pr);

        return RGBvals;
}