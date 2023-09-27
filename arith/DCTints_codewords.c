/*****************************************************************************
 *
 *                              DCTints_codeword.c
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our DCTints_codewords module. The
 *     purpose of this module is to convert between a UArray2 of DCT (discrete
 *     cosine transformation) scaled ints and a UArray2 of bitpacked codewords.
 *     This module contains two public functions, one that takes in a UArray2
 *     of DCT scaled int values and returns a UArray2 of bitpacked codewords,
 *     and one that takes in a UArray2 of bitpacked codewords and returns a
 *     UArray2 of DCT scaled int values. Both these functions rely on mapping
 *     private apply functions over the respective UArray2s to convert between
 *     the different values. Data is not lost during compression or
 *     decompression in this module.
 *     
 *
 *****************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "bitpack.h"
#include "DCTints_codewords.h"

/* this is the struct definition for the DCT_ints struct */
#include "DCT_ints.h"

/*
 * private helper functions, functions details are included in the function
 * contracts respectively
 */
static void     to_codewords_apply(int col, int row, UArray2_T array2, 
                                   void *val, void *codewords);
static void     to_dctints_apply  (int col, int row, UArray2_T array2, 
                                   void *val, void *dct_ints);
uint64_t        pack_word(struct DCT_ints dct_ints);
struct DCT_ints unpack_word(uint64_t codeword);

/* FUNCTION:  DCTints_codewords_compress
 * Purpose:   Converts a UArray2 of DCT ints to a UArray2 of codewords
 * Arg:       dct_ints: pointer to an instance of UArray2 that stores the 
 *                      DCT ints values
 * Returns:   Pointer to an UArray2 that stores the codewords
 * Effect:    initializes a new UArray2 and recycles the dct_ints UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTints_codewords_compress(UArray2_T dct_ints)
{
        assert(dct_ints != NULL);

        /* initialize an uarray2 of codewords */
        unsigned width = UArray2_width(dct_ints);
        unsigned height = UArray2_height(dct_ints);
        unsigned size = sizeof(uint64_t);
        UArray2_T codewords = UArray2_new(width, height, size);

        UArray2_map_row_major(dct_ints, to_codewords_apply, codewords);

        UArray2_free(&dct_ints);
        
        return codewords;
        
}

/* FUNCTION:  DCTints_codewords_decompress
 * Purpose:   Converts a UArray2 of codewords to a UArray2 of DCT ints
 * Arg:       codewords: pointer to an instance of UArray2 that stores the 
 *                       codewords
 * Returns:   Pointer to an UArray2 that stores the DCT scaled int values
 * Effect:    initializes a new UArray2 and recycles UArray2 of codewords
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTints_codewords_decompress(UArray2_T codewords)
{
        assert(codewords != NULL);

        /* initialize an uarray2 of DCT_int structs */
        unsigned width = UArray2_width(codewords);
        unsigned height = UArray2_height(codewords);
        unsigned size = sizeof(struct DCT_ints);
        UArray2_T dct_ints = UArray2_new(width, height, size);

        UArray2_map_row_major(codewords, to_dctints_apply, dct_ints);

        UArray2_free(&codewords);
        
        return dct_ints;
}

/* FUNCTION:  to_codewords_apply
 * Purpose:   For each element in the UArray2 of DCT_ints, convert each 
 *            DCT_ints struct into a bitpacked codeword, and add it to the
 *            UArray2 of codewords
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of DCT_ints (unused)
 *            val: pointer to the current element containing a DCT_ints struct
 *            codewords: pointer to the UArray2 of codewords
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a DCT_ints struct
 *            to a bitpacked codeword and adds it to the UArray2 of codewords 
 * Error:     N/A
 */
static void to_codewords_apply(int col, int row, UArray2_T array2, void *val, 
                               void *codewords)
{
        struct DCT_ints *dct_ints = val;
        
        /* converts a DCT_ints struct to a codeword using a private helper
           function */
        uint64_t codeword = pack_word(*dct_ints);

        /* store a codeword to the UArray2 of codeword at the respective
           column and row indices */
        *(uint64_t *)(UArray2_at(codewords, col, row)) = codeword;

        (void) array2;
}

/* FUNCTION:  to_dctints_apply
 * Purpose:   For each element in the UArray2 of codewords, convert each 
 *            codeword into a DCT_ints struct, and add it to the
 *            UArray2 of DCT ints values
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of codewords (unused)
 *            val: pointer to the current element containing a codeword
 *            dct_ints: pointer to the UArray2 of DCT_ints
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a codeword
 *            to a DCT_Ints struct and adds it to the UArray2 of DCT_ints 
 * Error:     N/A
 */
static void to_dctints_apply(int col, int row, UArray2_T array2, void *val, 
                             void *dct_ints)
{
        uint64_t codeword = *(uint64_t *)val;

        /* converts a codeword to a DCT_ints struct using a private helper
           function */ 
        struct DCT_ints dct_int = unpack_word(codeword);

        /* store a DCT_ints to the UArray2 of DCT_ints at the respective
           column and row indices */
        *(struct DCT_ints *)(UArray2_at(dct_ints, col, row)) = dct_int;
                                                        
        (void) array2;
}

/* FUNCTION:  pack_word
 * Purpose:   Convert a DCT_ints struct of DCT scaled
 *            int values into a bitpacked codeword
 * Arg:       dct_ints: a DCT_ints struct storing the DCT scaled int values
 * Returns:   A bitpacked codeword
 * Effect:    An additional note is that this function uses the #define values
 *            from the DCT_ints struct definition
 * Error:     N/A
 */
uint64_t pack_word(struct DCT_ints dct_ints)
{
        uint64_t codeword = 0;
        
        /* incremented after each field is packed */
        unsigned lsb_index = 0;

        /* Pack average Pb and Pr from the given DCT_ints */
        codeword = Bitpack_newu(codeword, AVG_PBPR_WIDTH, lsb_index, 
                                dct_ints.avgPr);
        lsb_index += AVG_PBPR_WIDTH;
        codeword = Bitpack_newu(codeword, AVG_PBPR_WIDTH, lsb_index, 
                                dct_ints.avgPb);
        lsb_index += AVG_PBPR_WIDTH;

        /* Pack b, c, and d from the given DCT_ints */
        codeword = Bitpack_news(codeword, BCD_WIDTH, lsb_index, dct_ints.d);
        lsb_index += BCD_WIDTH;
        codeword = Bitpack_news(codeword, BCD_WIDTH, lsb_index, dct_ints.c);
        lsb_index += BCD_WIDTH;
        codeword = Bitpack_news(codeword, BCD_WIDTH, lsb_index, dct_ints.b);
        lsb_index += BCD_WIDTH;
        
        /* Pack a from the given DCT_ints */
        codeword = Bitpack_newu(codeword, A_WIDTH, lsb_index, dct_ints.a);

        return codeword;
}

/* FUNCTION:  unpack_word
 * Purpose:   Convert a bitpacked int into a DCT_ints struct of DCT scaled
 *            int values
 * Arg:       codeword: a uint64_t storing the bitpacked codeword
 * Returns:   An instance of the DCT_ints struct
 * Effect:    An additional note is that this function uses the #define values
 *            from the DCT_ints struct definition
 * Error:     N/A
 */
struct DCT_ints unpack_word(uint64_t codeword)
{
        struct DCT_ints dct_int;
        
        /* To be incremented after each extraction */
        unsigned lsb_index = 0;
        
        /* Extract average Pb and Pr from the given codeword */
        dct_int.avgPr = Bitpack_getu(codeword, AVG_PBPR_WIDTH, lsb_index);
        lsb_index += AVG_PBPR_WIDTH;
        dct_int.avgPb = Bitpack_getu(codeword, AVG_PBPR_WIDTH, lsb_index);
        lsb_index += AVG_PBPR_WIDTH;

        /* Extract b, c, and d from the given codeword */
        dct_int.d = Bitpack_gets(codeword, BCD_WIDTH, lsb_index);
        lsb_index += BCD_WIDTH;
        dct_int.c = Bitpack_gets(codeword, BCD_WIDTH, lsb_index);
        lsb_index += BCD_WIDTH;
        dct_int.b = Bitpack_gets(codeword, BCD_WIDTH, lsb_index);
        lsb_index += BCD_WIDTH;
        
        /* Extract a from the given codeword */
        dct_int.a = Bitpack_getu(codeword, A_WIDTH, lsb_index);
        
        return dct_int;
}