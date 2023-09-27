/*
 *      uarray2rep.h
 *      by Eric Zhao
 *      Project:    Arith
 *
 *      Summary:
 *              This is the private struct definition of our 2 dimensional
 *      unboxed array data structure: UArray2. Our UArray2 is represented by
 *      one long single dimensional unboxed array whose indices correspond to
 *      different rows and columns. The UArray2 can be used to store a 2D array
 *      of any type of data that the client chooses to supply.
 */

#include <uarray.h>
#ifndef UARRAY2REP_INCLUDED
#define UARRAY2REP_INCLUDED

struct UArray2_T {
        int width;
        int height;
        UArray_T Uarray;
};

#endif