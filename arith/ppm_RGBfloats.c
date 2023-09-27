/****************************************************************************
 *
 *                              ppm_RGBfloats.c
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our ppm_RGBfloats module. 
 *     The purpose of this module is to convert between a PPM file and a 
 *     UArray2 of RGB float values. This module contains two public functions,
 *     one that reads in a PPM file and returns a UArray2 of RGB float values,
 *     and one that takes in a UArray2 of RGB float values and writes a PPM 
 *     file to stdout. Both these functions rely on mapping private apply 
 *     functions over the respective UArray2s to convert between the different
 *     values. This module also relies on the Pnm_ppm module to read and write
 *     the PPM files. In compression data is potentially lost in the process of
 *     trimming down the width and the height to an even number from the
 *     original file. The denominator from the original file is lost from the
 *     conversion between RGB values and their scaled floats representation.
 *     Additionally, the exact RGB ratios are lost due to imprecise nature of
 *     floating point math.
 * 
 *
 ****************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include "ppm_RGBfloats.h"
#include "a2plain.h"
#include "a2methods.h"
#include "pnm.h"

/* this is the struct definition for the RGB_floats struct */
#include "RGB_floats.h"

/*
 * private helper functions, functions details are included in the function
 * contracts respectively
 */
static void     to_floats_apply(int col, int row, UArray2_T RGBVals, void *val,
                                void *denom_ua2);
static void     to_RGBvals_apply(int col, int row, UArray2_T RGBfloats, 
                                 void *val, void *denom_ua2);
static float    RGBval_to_float(unsigned val, unsigned denom);
static unsigned float_to_RGBval(float scaledFloat, unsigned denom);
static unsigned trim_dimension(unsigned dimension);
static Pnm_ppm  pnm_ppm_new(unsigned width, unsigned height, unsigned denom, 
                            A2Methods_T methods, UArray2_T array2);
static struct   Denom_uarray2 new_denom_uarray2(UArray2_T array2, 
                                                unsigned denom);

/* 
 * arrray2: a pointer to an instance of UArray2; the instance stores the 
 *          compressed or decompressed pixels
 * denominator: the denominator of a pnm_ppm
 */
struct Denom_uarray2 {
        UArray2_T array2;
        unsigned denominator;
};

/* chosen denominator for decompression */
#define DENOMINATOR 255

/* FUNCTION:  ppm_RGBfloats_compress
 * Purpose:   Converts a given ppm file to an uarray2 of RGB float values
 * Arg:       file: a file pointer that stores the original image pixels
 * Returns:   pointer to an UArray2 instance that stores the compressed pixels
 * Effect:    allocates and recycles a RGB_floats struct
 *            recycles the pnm_ppm instance
 *            note that the file has already been opened
 * Error:     Runtime error if file is empty, if methods is null, or if memory
 *            allocation for the struct fails
 */
UArray2_T ppm_RGBfloats_compress(FILE *file)
{
        /* ensure file have been opened */
        assert(file != NULL);
        
        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);
        
        Pnm_ppm pixmap = Pnm_ppmread(file, methods);

        /* initialize a new array2 to store the RGBfloats */
        unsigned new_width = trim_dimension(pixmap->width);
        unsigned new_height = trim_dimension(pixmap->height);
        unsigned new_size = sizeof(struct RGB_floats);
        UArray2_T RGB_floats = UArray2_new(new_width, new_height, new_size);
                                                    
        /* initialize a struct that stores the new array and the denominator 
           to pass into our apply function */
        struct Denom_uarray2 *dm_ua2 = malloc(sizeof(*dm_ua2));
        assert(dm_ua2 != NULL);
        *dm_ua2 = new_denom_uarray2(RGB_floats, pixmap->denominator);
                
        UArray2_map_row_major(pixmap->pixels, to_floats_apply, dm_ua2);

        free(dm_ua2);
        Pnm_ppmfree(&pixmap);
        
        return RGB_floats;
}



/* FUNCTION:  ppm_RGBfloats_decompress
 * Purpose:   Prints an uarray2 of RGB float values 
 * Arg:       RGB_floats: pointer to an instance of uarray2 that stores the 
 *                        RGB values in floats
 * Returns:   N/A
 * Effect:    allocates and recycles a Pnm_rgb struct
 *            recycles the pnm_ppm instance
 *            note that the file has already been opened
 * Error:     Runtime error if a NULL pointer is passed in
 *            if methods is null, or if memory allocation for the struct fails
 */
void ppm_RGBfloats_decompress(UArray2_T RGB_floats)
{
        /* ensure file have been opened */
        assert(RGB_floats!= NULL);

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* initialize a new array2 that stores the RGB values */
        unsigned width = UArray2_width(RGB_floats); 
        unsigned height = UArray2_height(RGB_floats);
        unsigned size = sizeof(struct Pnm_rgb);
        UArray2_T unsigned_rgb = UArray2_new(width, height, size);

        /* initialize a struct that stores the new array and the denominator 
           to pass into our apply function; this is done using a private
           helper function new_denom_uarray2 */
        struct Denom_uarray2 *dm_ua2 = malloc(sizeof(*dm_ua2));
        assert(dm_ua2 != NULL);
        *dm_ua2 = new_denom_uarray2(unsigned_rgb, DENOMINATOR);

        UArray2_map_row_major(RGB_floats, to_RGBvals_apply, dm_ua2);

        /* initialize a pnm_ppm instance and write to stdout */
        Pnm_ppm pixmap = pnm_ppm_new(width, height, DENOMINATOR, methods, 
                                     unsigned_rgb);
        Pnm_ppmwrite(stdout, pixmap);

        Pnm_ppmfree(&pixmap);
        UArray2_free(&RGB_floats);
        free(dm_ua2);
}


/* FUNCTION:  to_floats_apply
 * Purpose:   For each pixel in the array of RGB scaled ints,
 *            convert each scaled RGB int to a RGB float (using a private 
 *            helper function) and add the pixel to the new array of 
 *            RGB floats
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            RGBVals: pointer to an uarray2 of scaled RGB ints (unused)
 *            val: pointer to the current pixel containing an Pnm_rgb struct
 *            denom_ua2: a Denom_uarray2 struct containing the new array of
 *                       scaled RGB ints and the RGB denominator
 * Returns:   N/A
 * Effect:    Populates the new uarray2 of scaled RGB ints
 * Error:     N/A
 */
static void to_floats_apply(int col, int row, UArray2_T RGBVals, void *val, 
                            void *denom_ua2)
{
        /* typecast and dereference closure and currente element */
        UArray2_T new_array2 = ((struct Denom_uarray2 *)denom_ua2)->array2;
        unsigned denom = ((struct Denom_uarray2 *)denom_ua2)->denominator;
        Pnm_rgb rgb_vals = val;

        int width = UArray2_width(new_array2);
        int height = UArray2_height(new_array2);
        
        /* check if col and row is within range */
        if ((col < width) && (row < height)) {
                /* initialize a struct and converts each RGBVal to floats 
                   using a private helper function */
                struct RGB_floats floats;
                floats.red = RGBval_to_float(rgb_vals->red, denom);
                floats.green = RGBval_to_float(rgb_vals->green, denom);
                floats.blue = RGBval_to_float(rgb_vals->blue, denom);
                
                /* add the RGB_floats struct to the new array of floats */
                *(struct RGB_floats *)(UArray2_at(new_array2, col, row)) =
                                                                floats;
        }
        
        (void) RGBVals;
}


/* FUNCTION:  to_RGBvals_apply
 * Purpose:   For each pixel in the array of RGB floats, convert each RGB value
 *            convert each RGB float to a scaled RGB int (using our private
 *            helper function) and add the pixel to the new array of scaled 
 *            RGB ints
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            RGBfloats: pointer to an uarray2 instance of RGBfloats (unused)
 *            val: pointer to the current pixel containing an RGB_floats struct
 *            denom_ua2: a Denom_uarray2 struct containing the new array of
 *                       scaled RGB ints and the RGB denominator
 * Returns:   N/A
 * Effect:    Populates the new uarray2 of scaled RGB ints
 * Error:     N/A
 */
static void to_RGBvals_apply(int col, int row, UArray2_T RGBfloats, void *val, 
                             void *denom_ua2)
{
        /* typecast and dereference closure and currente element */
        UArray2_T new_array2 = ((struct Denom_uarray2 *)denom_ua2)->array2;
        unsigned denom = ((struct Denom_uarray2 *)denom_ua2)->denominator;
        struct RGB_floats floats = *(struct RGB_floats*)val;

        /* initialize a struct and converts each float to RGB values 
           using a private helper function */
        struct Pnm_rgb rgb;
        rgb.red = float_to_RGBval(floats.red, denom);
        rgb.green = float_to_RGBval(floats.green, denom);
        rgb.blue = float_to_RGBval(floats.blue, denom);

        /* add the RGB_floats struct to the new array of floats */
        *(Pnm_rgb)(UArray2_at(new_array2, col, row)) = rgb;

        (void) RGBfloats;
}


/* FUNCTION:  trim_dimension
 * Purpose:   Round down a dimension to the closest even value
 * Arg:       dimension: an unsigned int representing the given dimension
 * Returns:   An unsigned int holding the trimmed dimension
 * Effect:    N/A
 * Error:     N/A
 */
static unsigned trim_dimension(unsigned dimension)
{
        if (dimension % 2 == 1) {
                return dimension - 1;
        } else {
                return dimension;
        }
}


/* FUNCTION:  RGBval_to_float
 * Purpose:   Convert a scaled RGB value to a float
 * Arg:       val: an unsigned int storing a scaled RGB value
 *            denom: an unsigned int storing the denominator 
 * Returns:   The RGB float value
 * Effect:    N/A
 * Error:     N/A
 */
static float RGBval_to_float(unsigned val, unsigned denom)
{
        return (float)val / (float)denom;
}


/* FUNCTION:  float_to_RGBval
 * Purpose:   convert an RGB float to a scaled RGB unsigned int
 * Arg:       val: float holding the given RGB value
 *            denom: an unsigned int holding the denominator
 * Returns:   An unsigned int representing the scaled RGB value
 * Effect:    N/A
 * Error:     N/A
 */
static unsigned float_to_RGBval(float val, unsigned denom)
{
        float RGB_val = val * (float)denom;

        /* check if the converted RGB valus are within 0 and the denominator 
         and if not, round them */
        if (RGB_val > denom) {
                RGB_val = denom;
        }
        
        if (RGB_val < 0) {
                RGB_val = 0;
        }

        return RGB_val;
}


/* FUNCTION:  new_denom_uarray2
 * Purpose:   Create a new instance of the Denom_uarray2 struct
 * Arg:       array2: an initialized uarray2
 *            denom: an unsigned value for the denominator
 * Returns:   An initialized Denom_uarray2 struct
 * Effect:    N/A
 * Error:     Runtime error if null pointer passed in as arg
 */
static struct Denom_uarray2 new_denom_uarray2(UArray2_T array2, unsigned denom)
{
        assert(array2 != NULL);
        
        struct Denom_uarray2 dm_ua2;
        dm_ua2.array2 = array2;
        dm_ua2.denominator = denom;

        return dm_ua2;
}


/* FUNCTION:  pnm_ppm_new
 * Purpose:   Create a new instance of a Pnm_ppm on the heap
 * Arg:       width: the width of the uarray2 in the Pnm_ppm
 *            height: the height of the uarray2 in the Pnm_ppm
 *            denom: the denominator of the Pnm_ppm
 *            methods: the methods of the Pnm_ppm
 *            array2: the uarray2 of the Pnm_ppm
 * Returns:   Pointer to a new Pnm_ppm instance
 * Effect:    Allocates memory for a new Pnm_ppm on the heap
 * Error:     Runtime error if memory cannot be allocated or if null pointers
 *            passed in as args
 */
static Pnm_ppm pnm_ppm_new(unsigned width, unsigned height, unsigned denom, 
                    A2Methods_T methods, UArray2_T array2)
{
        assert(array2 != NULL);
        assert(methods != NULL);
        
        /* initialize an instance of pnm_ppm */
        Pnm_ppm pixmap = malloc(sizeof(*pixmap));
        assert(pixmap != NULL);
        
        pixmap->width = width;
        pixmap->height = height;
        pixmap->denominator = denom;
        pixmap->methods = methods;
        pixmap->pixels = array2;
        
        return pixmap;
}