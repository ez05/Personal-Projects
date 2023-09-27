/*****************************************************************************
 *
 *                              RGBfloats_CV.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the public interface of our RGBfloats_CV module. 
 *     The purpose of this module is to convert between a UArray2 of RGB float 
 *     values and a UArray2 of CV color values. This module contains two public
 *     functions, one that takes in a UArray2 of RGB float values and returns a
 *     UArray2 of CV color values, and one that takes in a UArray2 of CV color
 *     values and returns a UArray2 of RGB float values. In compression and 
 *     decompression, data is lost due to imprecise nature of floating point
 *     math.
 *     
 *
 ****************************************************************************/
#include "uarray2.h"

#ifndef RGBfloats_CV_INCLUDED
#define RGBfloats_CV_INCLUDED

/* FUNCTION:  RGBfloats_CV_compress
 * Purpose:   Converts an UArray2 of RGB_floats to an UArray2 of component 
              video colors space
 * Arg:       RGB_floats: pointer to an instance of UArray2 that stores the 
 *                        RGB floats
 * Returns:   Pointer to an UArray2 that stores the components video color
 *            space values for each pixel
 * Effect:    initializes a new UArray2 and recycles the RGB_floats UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T RGBfloats_CV_compress(UArray2_T RGB_floats);

/* FUNCTION:  RGBfloats_CV_decompress
 * Purpose:   Converts an UArray2 of component video colors space to 
              an UArray2 of RGB floats values
 * Arg:       CV_colors: pointer to an instance of UArray2 that stores the 
 *                       component video color space 
 * Returns:   Pointer to an UArray2 that stores the RGB floats
 * Effect:    initializes a new UArray2 and recycles the CV colors UArray2
 * Error:     Runtime error a NULL pointer is passed in
 */
UArray2_T RGBfloats_CV_decompress(UArray2_T CV_Colors);

#endif