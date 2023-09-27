/****************************************************************************
 *
 *                              RGB_floats.h
 *
 *     Project:    Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the private struct definition for our RGB_floats struct. This
 *     struct contains 3 different float variables representing a pixel's red,
 *     green, and blue values (in a ratio from 0 - 1). This is used by the
 *     ppm_RGBfloats module and the RGBfloats_CV module.
 *
 ****************************************************************************/
#ifndef RGBFLOATSDEF_INCLUDED
#define RGBFLOATSDEF_INCLUDED

/* 
 * red:   float representation of color red in RGB
 * green: float representation of color green in RGB
 * blue:  float representation of color blue in RGB
 */
struct RGB_floats {
        float red, green, blue;
};

#endif