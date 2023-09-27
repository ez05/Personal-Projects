/**************************************************************
 *
 *                     uarray2.h
 *
 *     Project:  iii
 *     Authors:  Eric Zhao
 *     Date:     September 25, 2022
 *
 *     Summary:
 *     The purpose of uarray2.h is to provide an interface for
 *     clients to use the implementation of uarray.c. Which 
 *     implements a two-dimensional Uarray using a singular 
 *     uarray object, by adapting Hanson's Uarray abstraction.
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uarray.h>

#ifndef UAARAY2_INCLUDED
#define UARRAY2_INCLUDED

#define UArray2_T T 
typedef struct T *T;

/* FUNCTION:  UArray2_new
 * Purpose:   Allocates, initializes, and returns a new two-dimensional array
 *            of width * height elements with bounds zero through width − 1
 *            and height - 1; each element occupies size bytes
 * Arg:       int width: integer representing the width of an 2D array
 *            int height: integer representing the height of an 2D array
 *            int size: integer representing byte size of each element
 * Returns:   A pointer to an uarray2
 * Effect:    Allocates memory for a uarray2
 * Error:     Checked runtime error for width and height to be negative or for
 *            size to be nonpositive, and uarray2 new can raise memory failure
 */
extern T UArray2_new(int width, int height, int size);

/* FUNCTION:  UArray2_free
 * Purpose:   Deallocates memory and clears *uarray2
 * Arg:       The address of an uarray2 object
 * Returns:   N/A
 * Effect:    Deallocates memory and clears *uarray2
 * Error:     Checked runtime error for *uarray2 or uarray2 to be null
 */
extern void UArray2_free(T *uarray2);

/* FUNCTION:  UArray2_size
 * Purpose:   Returns the size of each element in an uarray2
 * Arg:       pointer to uarray2 object
 * Returns:   An integer representing the size of each element in uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
extern int UArray2_size(T uarray2);

/* FUNCTION:  UArray2_width
 * Purpose:   Return the width of an uarray2 object
 * Arg:       pointer to uarray2 object
 * Returns:   An integer representing the width of an uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
extern int UArray2_width(T uarray2);

/* FUNCTION:  UArray2_height
 * Purpose:   Return the numbers of rows of an uarray2 object
 * Arg:       pointer to uarray2 object
 * Returns:   An integer representing the height of an uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
extern int UArray2_height(T uarray2);

/* FUNCTION:  UArray_at
 * Purpose:   Returns the element at (i, j) in uarray2
 * Arg:       T uarray2: pointer to uarray2 object
 *            int rows, col: represents the row and column index respectively
 * Returns:   Pointer to an uarray2 element at position (rows, col) in uarray2
 * Effect:    N/A
 * Error:     Runtime error for out of range rows and column
 *            Runtime error for uarray2 to be null
 */
extern void *UArray2_at(T uarray2, int rows, int col);

/* FUNCTION:  UArray2_map_col_maj
 * Purpose:   Calls apply function for every element in the uarray2, 
 *            beginning at (0, 0)
 * Arg:       T uarray2: pointer to uarray2 object
 *            void apply: function pointer that gets called on every slot 
 *                       in uarray2
 *            void *CL: supplied by client and its value will be changed
 *                      if necessary
 * Returns:   N/A
 * Effect:    Row indices vary more rapidly than column indices.
 * Error:     Checked runtime error for uarray2 to be null
 */
extern void UArray2_map_col_major(T uarray2, void apply(int cols, int rows, 
                                T a, void *val, void *cl), void *CL);

/* FUNCTION:  UArray2_map_row_major
 * Purpose:   Calls apply function for every element in the uarray2, 
 *            beginning at (0, 0)
 * Arg:       T uarray2: pointer to uarray2 object
 *            void apply: function pointer that gets called on every slot 
 *                       in uarray2
 *            void *CL: supplied by client and its value will be changed
 *                      if neccessary
 * Returns:   N/A
 * Effect:    Column indices vary more rapidly than row indices.
 * Error:     Checked runtime error for uarray2 to be null
 */
extern void UArray2_map_row_major(T uarray2, void apply(int cols, int rows, 
                                T a, void *val, void *cl), void *CL);

#endif 
#undef T