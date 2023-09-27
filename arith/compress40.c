/*****************************************************************************
 *
 *                               compress40.c
 *
 *     Project: Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our compress40 module. This module
 *     allows the user to compress a PPM image to a binary representation, and
 *     to decompress a compressed binary representation to a PPM image.
 *     In compression, this program discards information not easily seen by the
 *     human eye (this is lossy compression).
 * 
 *
 ****************************************************************************/
#include <stdio.h>
#include <assert.h>
#include "compress40.h"
#include "uarray2.h"

/* 6 modules that handles compression and decompression incrementally */
#include "ppm_RGBfloats.h"
#include "RGBfloats_CV.h"
#include "CV_DCTfloats.h"
#include "DCTfloats_DCTints.h"
#include "DCTints_codewords.h"
#include "Codewords_File.h"

/* FUNCTION:  compress40
 * Purpose:   Compress a ppm file
 * Arg:       file: pointer to a file
 * Returns:   N/A
 * Effect:    Calls compression functions from different modules
 * Error:     Runtime error if a NULL pointer is passed in
 */
extern void compress40(FILE *input)
{
        assert(input != NULL);
        
        UArray2_T rgb_floats = ppm_RGBfloats_compress(input);
        UArray2_T cv_colors = RGBfloats_CV_compress(rgb_floats);
        UArray2_T dct_floats = CV_DCTfloats_compress(cv_colors);
        UArray2_T dct_ints = DCTfloats_ints_compress(dct_floats);
        UArray2_T codewords = DCTints_codewords_compress(dct_ints);
        Codewords_File_print(codewords);
}

/* FUNCTION:  decompress40
 * Purpose:   Decompress a compressed binary image file
 * Arg:       file: pointer to a file
 * Returns:   N/A
 * Effect:    Calls decompression functions from different modules
 * Error:     Runtime error if a NULL pointer is passed in
 */
extern void decompress40(FILE *input) 
{
        assert(input != NULL);

        UArray2_T codewords = Codewords_File_read(input);
        UArray2_T dct_ints = DCTints_codewords_decompress(codewords);
        UArray2_T dct_floats = DCTfloats_ints_decompress(dct_ints);
        UArray2_T cv_colors = CV_DCTfloats_decompress(dct_floats);
        UArray2_T rgb_floats = RGBfloats_CV_decompress(cv_colors);
        ppm_RGBfloats_decompress(rgb_floats);
}
