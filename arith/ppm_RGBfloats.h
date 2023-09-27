/****************************************************************************
 *
 *                              ppm_RGBfloats.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the public interface of our ppm_RGBfloats module. 
 *     The purpose of this module is to convert between a PPM file and a 
 *     UArray2 of RGB float values. This module contains two public functions,
 *     one that reads in a PPM file and returns a UArray2 of RGB float values,
 *     and one that takes in a UArray2 of RGB float values and writes a PPM 
 *     file to stdout. In compression data is potentially lost in the process
 *     of trimming down the width and the height to an even number from the
 *     original file. The denominator from the original file is lost from the 
 *     conversion between RGB values and their scaled floats representation.
 *     Additionally, the exact RGB ratios are lost due to imprecise nature of
 *     floating point math.
 * 
 *
 ****************************************************************************/
#include <stdio.h>
#include "uarray2.h"

#ifndef PPMRGBFLOATS_INCLUDED
#define PPMRGBFLOATS_INCLUDED

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
UArray2_T ppm_RGBfloats_compress(FILE *file);


/* FUNCTION:  ppm_RGBfloats_decompress
 * Purpose:   Prints an uarray2 of RGB float values 
 * Arg:       RGB_floats: pointer to an instance of uarray2 that stores the 
 *                        RGB values in floats
 * Returns:   N/A
 * Effect:    allocates and recycles a Pnm_rgb struct
 *            recycles the pnm_ppm instance
 *            note that the file has already been opened
 * Error:     Runtime error if uarray2 is uninitialized, 
 *            if methods is null, or if memory allocation for the struct fails
 */
void ppm_RGBfloats_decompress(UArray2_T RGB_floats);


#endif