/****************************************************************************
 *
 *                                 DCT_ints.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private struct definition for our DCT_ints struct. This
 *     struct contains 6 different int variables representing the average Pb
 *     and Pr, a, b, c, and d across 4 pixels in a 2 by 2 pixels block. 
 *     a, b, c, and d are the brightness values across 4 pixels in a 2 by 2 
 *     pixels block. It also includes a collection of defined values
 *     describing the sizes and bounds of our different DCT values. This struct
 *     is used by the DCTfloats_DCTints module and the DCTints_codewords 
 *     module.
 *
 ****************************************************************************/

#ifndef DCTINTSDEF_INCLUDED
#define DCTINTSDEF_INCLUDED

/* Represents the bit size of a and its maximum decimal value */
#define A_WIDTH 6
#define A_MAX ((1 << A_WIDTH) - 1)

/* Represents the bit size of b, c, and d, their absolute decimal 
   value bounds */
#define BCD_WIDTH 6
#define BCD_INT_BOUND ((1 << (BCD_WIDTH - 1)) - 1)
#define BCD_FLOAT_BOUND 0.3

/* Represents the scale factor used to scale the BCD values */
#define BCD_SCALE_FACTOR ((float)BCD_INT_BOUND / BCD_FLOAT_BOUND)

/* Represents the bit size of the avg PB and avg PR values */
#define AVG_PBPR_WIDTH 4

/* 
 * avgPb:       average scaled Pb values across 4 pixels in a 2 by 2 
                pixels block
 * avgPr:       average scaled Pr values across 4 pixels in a 2 by 2 
                pixels block
 * a, b, c, d:  the scaled brightness values across 4 pixels in a 2 by 2 
                pixels block
 */
struct DCT_ints {
        unsigned a, avgPb, avgPr;
        int b, c, d;        
};


#endif