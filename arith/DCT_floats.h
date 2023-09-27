/****************************************************************************
 *
 *                              DCT_floats.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private struct definition for our DCT_floats struct. This
 *     struct contains 6 different float variables representing the average Pb
 *     and Pr, a, b, c, and d across 4 pixels in a 2 by 2 pixels block. 
 *     a, b, c, and d are the brightness values across 4 pixels in a 2 by 2 
 *     pixels block. This struct is used by the CV_DCTfloats module and the
 *     DCTfloats_DCTints module.
 *
 ****************************************************************************/
#ifndef DCTFLOATSDEF_INCLUDED
#define DCTFLOATSDEF_INCLUDED

/* 
 * avgPb:       average Pb values across 4 pixels in a 2 by 2 pixels block
 * avgPr:       average Pr values across 4 pixels in a 2 by 2 pixels block
 * a, b, c, d:  the brightness values across 4 pixels in a 2 by 2 pixels block
 */
struct DCT_floats {
        float avgPb, avgPr, a, b, c, d;
};

#endif