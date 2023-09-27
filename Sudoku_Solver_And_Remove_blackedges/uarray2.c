/**************************************************************
 *
 *                     uarray2.c
 *
 *     Project:  iii
 *     Authors:  Eric Zhao
 *     Date:     September 25, 2022
 *
 *     Summary:
 *     The purpose of uarray2.c is to implement a two-dimensional
 *     Uarray using a singular uarray object, by adapting Hanson's
 *     Uarray abstraction.
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uarray.h>
#include <mem.h>
#include "uarray2.h"

/* 
   width indicates the number of element in a row
   height indicates the number of element in a column
   size indicates the byte size of each element
   Uarray_T is a pointer to an uarray object
 */
struct UArray2_T {
        int width;
        int height;
        int size;
        UArray_T uarray;
};

/* private helper function */
int index_helper(T uarray2, int cols, int rows);

/* FUNCTION:  UArray2_new
 * Purpose:   Allocates, initializes, and returns a new two-dimensional array
 *            of width * height elements with bounds zero through width − 1
 *            and height - 1; each element occupies size bytes
 * Arg:       int width: integer representing the width of an 2D array
 *            int height: integer representing the height of an 2D array
 *            int size: integer representing byte size of each element
 * Returns:   A pointer to an uarray2 object
 * Effect:    Allocates memory for a uarray2
 * Error:     Checked runtime error for width and height to be negative or for
 *            size to be nonpositive, and uarray2 new can raise memory failure
 *            Runtime error if fails to allocate memory for UArray2
 */
T UArray2_new(int width, int height, int size)
{
        assert(width > -1 && height > -1);

        /* allocates memory for UArray2 object */
        T UArray2 = malloc(sizeof(struct T));
        assert(UArray2 != NULL);
        
        if (width > 0 && height > 0) {
                /* non-empty uarray2 */
                UArray2->uarray = UArray_new(height * width, size);
        } else {
                /* empty uarray2 */
                UArray2->uarray = NULL;
        }
        UArray2->height = height;
        UArray2->size = size;
        UArray2->width = width;

        return UArray2;
}

/* FUNCTION:  UArray2_free
 * Purpose:   Deallocates memory and clears *uarray2
 * Arg:       The address of an uarray2 object
 * Returns:   N/A
 * Effect:    Deallocates memory and clears *uarray2
 * Error:     Checked runtime error for *uarray2 or uarray2 to be null
 */
void UArray2_free(T *uarray2)
{
        assert((*uarray2) != NULL || uarray2 != NULL);
        UArray_free(&((*uarray2)->uarray));
        free(*uarray2);
}

/* FUNCTION:  UArray2_size
 * Purpose:   Returns the size of each element in an uarray2
 * Arg:       pointer to uarray2 object
 * Returns:   An integer representing the size of each element in uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
int UArray2_size(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->size;
}

/* FUNCTION:  UArray2_width
 * Purpose:   Return the width of an uarray2 object
 * Arg:       pointer to uarray2 object
 * Returns:   An integer representing the width of an uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->width;
}

/* FUNCTION:  UArray2_height
 * Purpose:   Return the numbers of rows of an uarray2 object
 * Arg:       pointer to uarray2 object
 * Returns:   An integer representing the height of an uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL);
        return uarray2->height;
}

/* FUNCTION:  UArray_at
 * Purpose:   Returns the element at (cols, rows) in uarray2
 * Arg:       T uarray2: pointer to an uarray2 object
 *            int rows, col: represents the row and column index respectively
 * Returns:   Pointer to an uarray2 element at position (rows, col) in uarray2
 * Effect:    N/A
 * Error:     Runtime error for out of range rows and column
 *            Runtime error for uarray2 to be null
 */
void *UArray2_at(T uarray2, int cols, int rows)
{
        assert(uarray2 != NULL);
        assert(rows > -1 && cols > -1);
        assert(rows < uarray2->height && cols < uarray2->width);
        return UArray_at(uarray2->uarray, index_helper(uarray2, cols, rows));
}

/* FUNCTION:  index_helper
 * Purpose:   Computes the index of an element from a 2-dimensional array to
 *            an one dimensional array
 * Arg:       T uarray2: pointer to uarray2 object
 *            int rows, col: represents the row and column index respectively
 * Returns:   An integer indicating the element at (cols, rows)
 * Effect:    N/A
 * Error:     N/A
 */
int index_helper(T uarray2, int cols, int rows)
{
        return ((rows * uarray2->width) + cols);
}

/* FUNCTION:  UArray2_map_col_maj
 * Purpose:   Calls apply function for every element in the uarray2, 
 *            beginning at (0, 0)
 * Arg:       T uarray2: pointer to an uarray2 object
 *            void apply: function pointer that gets called on every element 
 *                       in uarray2
 *            void *CL: supplied by client and its value will be changed
 *                      if necessary
 * Returns:   N/A
 * Effect:    Row indices vary more rapidly than column indices.
 *            Closure variable will be changed if necessary
 * Error:     Checked runtime error for uarray2 to be null
 */
void UArray2_map_col_major(T uarray2, void apply(int cols, int rows, 
                        T a, void *val, void *cl), void *CL)
{
        assert(uarray2 != NULL);
        for (int i = 0; i < uarray2->width; i++) {
                for (int j = 0; j < uarray2->height; j++) {
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), CL);
                }
        }
}

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
 *            Closure variable will be changed if necessary
 * Error:     Checked runtime error for uarray2 to be null
 */
void UArray2_map_row_major(T uarray2, void apply(int cols, int rows, 
                        T a, void *val, void *cl), void *CL)
{
        assert(uarray2 != NULL);
        for (int i = 0; i < uarray2->height; i++) {
                for (int j = 0; j < uarray2->width; j++) {
                        apply(j, i, uarray2, UArray2_at(uarray2, j, i), CL);
                }
        }
}