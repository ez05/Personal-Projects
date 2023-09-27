/*****************************************************************************
 *
 *                              Codewords_File.c
 *
 *     Project: Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our Codewords_File module. 
 *     The purpose of this module is to convert between a UArray2 of bitpacked
 *     codwords and a binary file of codewords. This module contains two public
 *     functions, one that takes in a UArray2 of codewords and writes a binary
 *     file of codewords to stdout and one that reads in a binary file of
 *     codewords and returns a UArray2 of codewords. Both these functions rely 
 *     on mapping private apply functions over the respective UArray2s to 
 *     convert between the different values. Data is not lost during
 *     reading or writing in this module.
 * 
 *
 ****************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include "Codewords_File.h"
#include <stdbool.h>
#include <stdio.h>
#include <bitpack.h>
#include "uarray2.h"
/* this is the struct definition for the DCT_ints struct */
#include "DCT_ints.h"

/* Represents the bit size of a character */
#define CHAR_BITS 8

/* Represents the number of bytes storing data in a codeword */
/* (uses the #define values from the DCT_ints struct)*/
#define CODEWORD_BYTES ((A_WIDTH + 3 * BCD_WIDTH + 2 * AVG_PBPR_WIDTH) / 8)

/*
 * private helper functions, functions details are included in the function
 * contracts respectively
 */
static void     print_apply(int col, int row, UArray2_T array2, void *val, 
                            void *mtds);
static void     read_apply (int col, int row, UArray2_T array2, void *val, 
                            void *mtd_file);
static void     print_codeword(uint64_t codeword, unsigned size);
static uint64_t read_codeword (FILE *file, unsigned size);


/* FUNCTION:  Codewords_File_print
 * Purpose:   Prints a UArray2 of codewords into standard output
 * Arg:       codewords: pointer to a UArray2 of codewords
 * Returns:   N/A
 * Effect:    Recycles the UArray2 of codewords
 * Error:     Runtime error if a NULL pointer is passed in
 */
void Codewords_File_print(UArray2_T codewords)
{
        assert(codewords != NULL);

        /* prints the header */
        unsigned width = UArray2_width(codewords);
        unsigned height = UArray2_height(codewords);
        printf("COMP40 Compressed image format 2\n%u %u\n", width * 2, 
                                                            height * 2);

        /* map over the UArray2 and prints to standard output */
        UArray2_map_row_major(codewords, print_apply, NULL);

        UArray2_free(&codewords);
}

/* FUNCTION:  Codewords_File_read
 * Purpose:   Reads from a binary file and initialize a UArray2 of codewords
 * Arg:       file: pointer to a file instance
 * Returns:   Pointer to an instance of UArray2 of codewords
 * Effect:    N/A
 * Error:     Runtime error if a NULL pointer is passed in
 *            Runtime error for not correctly formatted header
 */
UArray2_T Codewords_File_read(FILE *file)
{
        assert(file != NULL);

        /* gets width and height of the image from the header */
        unsigned height, width;
        int read = fscanf(file, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height); 
        assert(read == 2);
        int c = getc(file);
        assert(c == '\n');

        /* Initialize a UArray2 of codewords with half the dimensions from
           the original binary file */
        unsigned size = sizeof(uint64_t);
        UArray2_T codewords = UArray2_new(width / 2, height / 2, size);

        UArray2_map_row_major(codewords, read_apply, file);

        return codewords;
}

/* FUNCTION:  print_apply
 * Purpose:   For each element in the UArray2 of codewords, prints each 
              codeword to standard output
 * Arg:       col: a int representing the current column in the array (unused)
 *            row: a row representing the current row in the array (unused)
 *            array2: pointer to a UArray2 of codewords (unused)
 *            val: pointer to the current element containing a codeword
 *            cl: unused closure variable
 * Returns:   N/A
 * Effect:    Calls a private helper function that prints a codeword
 * Error:     N/A
 */
static void print_apply(int col, int row, UArray2_T array2, void *val, 
                        void *cl)
{
        uint64_t codeword = *(uint64_t *)val;
        
        print_codeword(codeword, CODEWORD_BYTES);

        (void) col;
        (void) row;
        (void) cl;
        (void) array2;
}

/* FUNCTION:  read_apply
 * Purpose:   Reads from a file and write to the UArray2 of codewords 
 * Arg:       col: a int representing the current column in the array (unused)
 *            row: a row representing the current row in the array (unused)
 *            array2: pointer to a UArray2 of codewords (unused)
 *            val: pointer to the current element containing a codeword
 *            cl: pointer to a file
 * Returns:   N/A
 * Effect:    Calls a private helper function that reads from a file
 * Error:     N/A
 */
static void read_apply(int col, int row, UArray2_T array2, void *val, 
                       void *file)
{
        *(uint64_t *)val = read_codeword((FILE *)file, CODEWORD_BYTES);

        (void) col;
        (void) row;
        (void) array2;
}

/* FUNCTION:  print_codeword
 * Purpose:   write a codeword to stdout
 * Arg:       codeword: a uint64_t representing a codeword
 *            size: the size of the codeword (in bytes)
 * Returns:   N/A
 * Effect:    Codeword is printed to stdout
 * Error:     N/A
 */
static void print_codeword(uint64_t codeword, unsigned size)
{
        unsigned i;
        for (i = 0; i < size; i++) {
                /* total number of bits in a codeword - bits that have already 
                   been packed */
                unsigned lsb = (CODEWORD_BYTES * CHAR_BITS) - 
                               ((i + 1) * CHAR_BITS);

                /* get the current bits and prints */               
                unsigned current_bits = Bitpack_getu(codeword, CHAR_BITS, lsb);
                putchar(current_bits);
        }
}

/* FUNCTION:  read_codeword
 * Purpose:   Read in a codeword from the binary file
 * Arg:       file: an opened binary file of codewords
 *            size: the size of the codeword (in bytes)
 * Returns:   a uint64_t storing the codeword
 * Effect:    Data is read in from the file in byte sized chunks.
 * Error:     Runtime error if EOF is hit while reading
 *            Runtime error if file is not provided
 */
static uint64_t read_codeword(FILE *file, unsigned size)
{
        assert(file != NULL);

        uint64_t codeword = 0;
        unsigned i;
        for (i = 0; i < size; i++) {
                /* reads in one byte */
                int bits_read = getc(file);
                assert(bits_read != EOF);
                
                /* total number of bits in a codeword - bits that have already 
                   been packed */
                unsigned lsb = (CODEWORD_BYTES * CHAR_BITS) - 
                               ((i + 1) * CHAR_BITS); 
                
                /* pack the byte in the codeword */
                codeword = Bitpack_newu(codeword, CHAR_BITS, lsb, bits_read);
        }

        return codeword;
}