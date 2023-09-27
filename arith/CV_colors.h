/****************************************************************************
 *
 *                              CV_colors.h
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
 *     pixels block. This struct is used in the RGBfloats_CV module and the
 *     CV_DCTfloats module.
 *
 ****************************************************************************/
#ifndef CVCOLORSDEF_INCLUDED
#define CVCOLORSDEF_INCLUDED

/* 
 * y, pb, pr: y represents the luminance and pb and pr represent 
 *            color-difference signals
 */
struct CV_colors {
        float y, pb, pr;
};

#endif