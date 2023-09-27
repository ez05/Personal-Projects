/*****************************************************************************
 *
 *                              DCTfloats_DCTints.c
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private implementation of our DCTfloats_DCTints module. The
 *     purpose of this module is to convert between a UArray2 of DCT (discrete
 *     cosine transformation) floats and a UArray2 scaled DCT ints. This module
 *     contains two public functions, one that takes in a UArray2 of DCT float
 *     values and returns a UArray2 of DCT scaled int values, and one that 
 *     takes in a UArray2 of DCT scaled int values and returns a UArray2 of DCT
 *     float values. Both these functions rely on mapping private apply
 *     functions over the respective UArray2s to convert between the different
 *     values. In compression, data is lost when mapping a float value to a
 *     specific int in our scale. More data is lost when compressing
 *     extreme DCT float values due to rounding.
 *     
 *
 *****************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "DCTfloats_DCTints.h"
#include "arith40.h"

/* this is the struct definition for the DCT_floats struct */
#include "DCT_floats.h"
/* this is the struct definition for the DCT_ints struct */
#include "DCT_ints.h"

/*
 * private helper functions, functions details are included in the function
 * contracts respectively
 */
static void              to_dctints_apply(int col, int row, UArray2_T array2,
                                          void *val, void *dctfloats_ua2);
static void              to_dctfloats_apply(int col, int row, UArray2_T array2,
                                            void *val, void *dctints_ua2);
static struct DCT_floats DCT_ints_to_floats(struct DCT_ints dct_ints);
static struct DCT_ints   DCT_floats_to_ints(struct DCT_floats dct_floats);
static unsigned          scale_a(float a);
static float             unscale_a(unsigned a);
static int               scale_bcd(float bcd);
static float             unscale_bcd(int bcd);

/* FUNCTION:  DCTfloats_ints_compress
 * Purpose:   Converts a UArray2 of DCT floats to a UArray2 of DCT scaled ints
 * Arg:       dct_floats: pointer to an instance of UArray2 that stores the 
 *                        DCT floats values
 * Returns:   Pointer to an UArray2 that stores the DCT scaled int values
 * Effect:    initializes a new UArray2 and recycles the dct_floats UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTfloats_ints_compress(UArray2_T dct_floats)
{
        assert(dct_floats != NULL);
        
        /* initialize an uarray2 of DCT_ints structs */
        unsigned width = UArray2_width(dct_floats);
        unsigned height = UArray2_height(dct_floats);
        unsigned size = sizeof(struct DCT_ints);
        UArray2_T dct_ints = UArray2_new(width, height, size);

        UArray2_map_row_major(dct_floats, to_dctints_apply, dct_ints);

        UArray2_free(&dct_floats);
        
        return dct_ints;
}

/* FUNCTION:  DCTfloats_ints_decompress
 * Purpose:   Converts a UArray2 of DCT scaled int to a UArray2 of DCT floats
 * Arg:       dct_ints: pointer to an instance of UArray2 that stores the 
 *                      DCT scaled ints
 * Returns:   Pointer to an UArray2 that stores the DCT floats values
 * Effect:    initializes a new UArray2 and recycles the dct_ints UArray2
 * Error:     Runtime error if a NULL pointer is passed in
 */
UArray2_T DCTfloats_ints_decompress(UArray2_T dct_ints)
{
        assert(dct_ints != NULL);

        /* initialize an uarray2 of DCT_floats structs */
        unsigned width = UArray2_width(dct_ints);
        unsigned height = UArray2_height(dct_ints);
        unsigned size = sizeof(struct DCT_floats);
        UArray2_T dct_floats = UArray2_new(width, height, size);

        UArray2_map_row_major(dct_ints, to_dctfloats_apply, dct_floats);

        UArray2_free(&dct_ints);
        
        return dct_floats;
}

/* FUNCTION:  to_dctints_apply
 * Purpose:   For each element in the UArray2 of DCT_floats, convert each 
 *            DCT_floats struct into a DCT_ints struct, and add them to the
 *            UArray2 of DCT_ints
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of DCT_floats (unused)
 *            val: pointer to the current pixel containing a DCT_floats struct
 *            dct_ints_ua2: pointer to the UArray2 of DCT_ints
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a DCT_floats struct
 *            to a DCT_ints struct and adds it to the UArray2 of DCT_ints 
 * Error:     N/A
 */
static void to_dctints_apply(int col, int row, UArray2_T array2, void *val, 
                             void *dct_ints_ua2)
{
        struct DCT_floats *dct_floats = val;

        /* converts a DCT_floats struct to a DCT_ints struct using a private
           helper function */
        struct DCT_ints dct_ints = DCT_floats_to_ints(*dct_floats);
        
        /* store a DCT_ints to the UArray2 of DCT_ints at the respective
           column and row indices */
        *(struct DCT_ints *)(UArray2_at(dct_ints_ua2, col, row)) = dct_ints;

        (void) array2;
}

/* FUNCTION:  to_dctfloats_apply
 * Purpose:   For each element in the UArray2 of DCT_ints, convert each 
 *            DCT_ints struct into a DCT_floats struct, and add them to the
 *            UArray2 of DCT floats
 * Arg:       col: a int representing the current column in the array
 *            row: a row representing the current row in the array
 *            array2: pointer to a UArray2 of DCT_ints (unused)
 *            val: pointer to the current pixel containing a DCT_int struct
 *            dct_floats_ua2: pointer to the UArray2 of DCT floats
 * Returns:   N/A
 * Effect:    Calls a private helper function that converts a DCT_ints struct
 *            to a DCT_float struct, and adds it to the UArray2 of DCT_floats 
 * Error:     N/A
 */
static void to_dctfloats_apply(int col, int row, UArray2_T array2, void *val,
                               void *dct_floats_ua2)
{
        struct DCT_ints *dct_ints = val;

        /* converts a DCT_ints struct to a DCT_floats struct using a private
           helper function */
        struct DCT_floats dct_floats = DCT_ints_to_floats(*dct_ints);
        
        /* store a DCT_floats to the UArray2 of DCT_floats at the respective
           column and row indices */
        *(struct DCT_floats *)(UArray2_at(dct_floats_ua2, col, row)) 
                = dct_floats;

        (void) array2;
}

/* FUNCTION:  DCT_floats_to_ints
 * Purpose:   Convert an instance of DCT floats to DCT scaled ints using our
 *            private helper functions
 * Arg:       dct_floats: an instance of a DCT_floats struct 
 * Returns:   An instance of a DCT_ints struct
 * Effect:    N/A
 * Error:     N/A
 */
static struct DCT_ints DCT_floats_to_ints(struct DCT_floats dct_floats)
{
        struct DCT_ints dct_ints;

        /* initialize the DCT_ints struct by computing each of its field using
           private helper functions */
        dct_ints.avgPb = Arith40_index_of_chroma(dct_floats.avgPb);
        dct_ints.avgPr = Arith40_index_of_chroma(dct_floats.avgPr);
        dct_ints.a = scale_a(dct_floats.a);
        dct_ints.b = scale_bcd(dct_floats.b);
        dct_ints.c = scale_bcd(dct_floats.c);
        dct_ints.d = scale_bcd(dct_floats.d);

        return dct_ints;
}

/* FUNCTION:  DCT_ints_to_floats
 * Purpose:   Convert an instance of DCT scaled ints to DCT floats using our
 *            private helper functions
 * Arg:       dct_ints: an instance of a DCT_ints struct 
 * Returns:   An instance of a DCT_floats struct
 * Effect:    N/A
 * Error:     N/A
 */
static struct DCT_floats DCT_ints_to_floats(struct DCT_ints dct_ints)
{
        struct DCT_floats dct_floats;
        
        /* initialize the DCT_floats struct by computing each of its field 
           using private helper functions */
        dct_floats.avgPb = Arith40_chroma_of_index(dct_ints.avgPb);
        dct_floats.avgPr = Arith40_chroma_of_index(dct_ints.avgPr);
        dct_floats.a = unscale_a(dct_ints.a);
        dct_floats.b = unscale_bcd(dct_ints.b);
        dct_floats.c = unscale_bcd(dct_ints.c);
        dct_floats.d = unscale_bcd(dct_ints.d);
        
        return dct_floats;
}

/* FUNCTION:  scale_a
 * Purpose:   Convert a float representing an a value to a scaled int
 * Arg:       a: a float representing an a value
 * Returns:   The a value in the form of a scaled int
 * Effect:    An additional note is that this function uses the #define values
 *            from the DCT_ints struct definition
 * Error:     N/A
 */
static unsigned scale_a(float a)
{
        /* if the a value is out of range, round it to fit the range */
        if (a < 0) {
                a = 0;
        }
        if (a > 1) {
                a = 1;
        }
        
        return a * A_MAX;
}

/* FUNCTION:  uscale_a
 * Purpose:   Convert a scaled int representing an a value to a float
 * Arg:       a: a scaled int representing an a value
 * Returns:   The a value in the form of a float
 * Effect:    An additional note is that this function uses the #define values
 *            from the DCT_ints struct definition
 * Error:     N/A
 */
static float unscale_a(unsigned a)
{
        /* if the a value is out of range, round it to fit the range */
        if (a > A_MAX) {
                a = A_MAX;
        }
        
        return ((float) a / A_MAX);
}

/* FUNCTION:  scale_bcd
 * Purpose:   Convert a float representing a b, c, or d value to a
 *            scaled int
 * Arg:       bcd: a float representing a b, c, or d value
 * Returns:   The b, c, or d value in the form of a scaled int
 * Effect:    An additional note is that this function uses the #define values
 *            from the DCT_ints struct definition
 * Error:     N/A
 */
static int scale_bcd(float bcd)
{
        /* if the bcd value is out of range, round it to fit the range */
        if (bcd > BCD_FLOAT_BOUND) {
                bcd = BCD_FLOAT_BOUND;
        }
        if (bcd < -BCD_FLOAT_BOUND) {
                bcd = -BCD_FLOAT_BOUND;
        }
        
        return bcd * BCD_SCALE_FACTOR;
}

/* FUNCTION:  unscale_bcd
 * Purpose:   Convert a scaled int representing a b, c, or d value to a
 *            float value
 * Arg:       bcd: an scaled int representing a b, c, or d value
 * Returns:   The b, c, or d value in the form of a float
 * Effect:    An additional note is that this function uses the #define values
 *            from the DCT_ints struct definition
 * Error:     N/A
 */
static float unscale_bcd(int bcd)
{
        /* if the bcd value is out of range, round it to fit the range */
        if (bcd > (int)BCD_INT_BOUND) {
                bcd = BCD_INT_BOUND;
        }
        if (bcd < -BCD_INT_BOUND) {
                bcd = -BCD_INT_BOUND;
        }
        
        return (float) bcd / BCD_SCALE_FACTOR;
}