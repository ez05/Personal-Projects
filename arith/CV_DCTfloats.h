/*****************************************************************************
 *
 *                              CV_DCTfloats.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the public interface of our CV_DCTfloats module. 
 *     The purpose of this module is to convert between a UArray2 of CV colors
 *     and a UArray2 of floats resulting from a discrete cosine transformation.
 *     This module contains two public functions, one that takes in a UArray2 
 *     of CV color values and returns a UArray2 of DCT floats values, and one 
 *     that takes in a UArray2 of DCT floats values and returns a UArray2 of 
 *     CV color values. In compression data is lost due to imprecise nature of 
 *     floating point math. Additionally, data is lost in compression through 
 *     the DCT conversion which stores only the color gradient in a block of 
 *     4 pixels, not their individual RGB values.
 *     
 *
 *****************************************************************************/
#include "uarray2.h"

#ifndef CV_DCTFLOATS_INCLUDED
#define CV_DCTFLOATS_INCLUDED

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
UArray2_T CV_DCTfloats_compress(UArray2_T cv_colors);

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
UArray2_T CV_DCTfloats_decompress(UArray2_T dct_floats);

#endif