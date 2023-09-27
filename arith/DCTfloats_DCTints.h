/*****************************************************************************
 *
 *                            DCTfloats_DCTints.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the public interface of our DCTfloats_DCTints module. The
 *     purpose of this module is to convert between a UArray2 of DCT (discrete
 *     cosine transformation) floats and a UArray2 scaled DCT ints. This module
 *     contains two public functions, one that takes in a UArray2 of DCT float
 *     values and returns a UArray2 of DCT scaled int values, and one that 
 *     takes in a UArray2 of DCT scaled int values and returns a UArray2 of DCT
 *     float values. In compression, data is lost when mapping a float value to
 *     a specific int in our scale. More data is lost when compressing
 *     extreme DCT float values due to rounding.
 *     
 *
 *****************************************************************************/
#include "uarray2.h"

#ifndef DCTFLOATSDCTINTS_INCLUDED
#define DCTFLOATSDCTINTS_INCLUDED

/* FUNCTION:  DCTfloats_ints_compress
 * Purpose:   Converts a UArray2 of DCT floats to a UArray2 of DCT scaled ints
 * Arg:       dct_floats: pointer to an instance of UArray2 that stores the 
 *                        DCT floats values
 * Returns:   Pointer to an UArray2 that stores the DCT scaled int values
 * Effect:    initializes a new UArray2 and recycles the dct_floats UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTfloats_ints_compress(UArray2_T dct_floats);

/* FUNCTION:  DCTfloats_ints_decompress
 * Purpose:   Converts a UArray2 of DCT scaled int to a UArray2 of DCT floats
 * Arg:       dct_ints: pointer to an instance of UArray2 that stores the 
 *                      DCT scaled ints
 * Returns:   Pointer to an UArray2 that stores the DCT floats values
 * Effect:    initializes a new UArray2 and recycles the dct_ints UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTfloats_ints_decompress(UArray2_T dct_ints);

#endif