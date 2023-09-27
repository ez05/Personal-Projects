/*****************************************************************************
 *
 *                              Codewords_File.c
 *
 *     Projet: Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the public interface of our Codewords_File module. 
 *     The purpose of this module is to convert between a UArray2 of bitpacked
 *     codwords and a binary file of codewords. This module contains two public
 *     functions, one that takes in a UArray2 of codewords and writes a binary
 *     file of codewords to stdout and one that reads in a binary file of
 *     codewords and returns a UArray2 of codewords. Data is not lost during
 *     reading or writing in this module.
 * 
 *
 ****************************************************************************/
#include "uarray2.h"
#include <stdio.h>

#ifndef CODEWORDSFILE_INCLUDED
#define CODEWORDSFILE_INCLUDED

/* FUNCTION:  Codewords_File_print
 * Purpose:   Prints a UArray2 of codewords into standard output
 * Arg:       codewords: pointer to a UArray2 of codewords
 * Returns:   N/A
 * Effect:    Recycles the UArray2 of codewords
 * Error:     Runtime error if a NULL pointer is passed in
 */
void Codewords_File_print(UArray2_T codewords);

/* FUNCTION:  Codewords_File_read
 * Purpose:   Reads from a binary file and initialize a UArray2 of codewords
 * Arg:       file: pointer to a file instance
 * Returns:   Pointer to an instance of UArray2 of codewords
 * Effect:    N/A
 * Error:     Runtime error if a NULL pointer is passed in
 *            Runtime error for not correctly formatted header
 */
UArray2_T Codewords_File_read(FILE *file);

#endif