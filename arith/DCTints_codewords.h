/*****************************************************************************
 *
 *                              DCTints_codeword.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the public interface of our DCTints_codewords module. The
 *     purpose of this module is to convert between a UArray2 of DCT (discrete
 *     cosine transformation) scaled ints and a UArray2 of bitpacked codewords.
 *     This module contains two public functions, one that takes in a UArray2
 *     of DCT scaled int values and returns a UArray2 of bitpacked codewords,
 *     and one that takes in a UArray2 of bitpacked codewords and returns a
 *     UArray2 of DCT scaled int values. Data is not lost during compression or
 *     decompression in this module.
 *     
 *
 *****************************************************************************/
#include "uarray2.h"

#ifndef DCTINTSCODEWORDS_INCLUDED
#define DCTINTSCODEWORDS_INCLUDED

/* FUNCTION:  DCTints_codewords_compress
 * Purpose:   Converts a UArray2 of DCT ints to a UArray2 of codewords
 * Arg:       dct_ints: pointer to an instance of UArray2 that stores the 
 *                      DCT ints values
 * Returns:   Pointer to an UArray2 that stores the codewords
 * Effect:    initializes a new UArray2 and recycles the dct_ints UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTints_codewords_compress(UArray2_T dct_ints);

/* FUNCTION:  DCTints_codewords_decompress
 * Purpose:   Converts a UArray2 of codewords to a UArray2 of DCT ints
 * Arg:       codewords: pointer to an instance of UArray2 that stores the 
 *                       codewords
 * Returns:   Pointer to an UArray2 that stores the DCT scaled int values
 * Effect:    initializes a new UArray2 and recycles UArray2 of codewords
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTints_codewords_decompress(UArray2_T codewords);

#endif