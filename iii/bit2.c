/**************************************************************
 *
 *                     bit2.c
 *
 *     Project:  iii
 *     Authors:  Eric Zhao
 *     Date:     September 25, 2022
 *
 *     Summary:
 *     The purpose of 
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bit.h>
#include <mem.h>
#include "bit2.h"

/* 
   width indicates the number of bits in a row
   height indicates the number of bits in a column
   Bit_T is a pointer to a bit object
 */
struct Bit2_T {
        int height;
        int width;
        Bit_T bit;
};

/* private helper function */
int index_helper(T Bit2, int cols, int rows);

/* FUNCTION:  Bit2_new
 * Purpose:   Allocates, initializes, and returns a new two-dimensional 
 *            bit array of width * height with bounds zero through width − 1
 *            and height - 1
 * Arg:       int width: integer representing the width of an 2D array
 *            int height: integer representing the height of an 2D array
 * Returns:   A pointer to a Bit2 object
 * Effect:    Allocates memory for a Bit2
 * Error:     Checked runtime error for width and height to be negative 
 *            Bit2_new can raise memory failure
 *            Runtime error if fails to allocate memory for Bit2
 */
T Bit2_new(int width, int height)
{
        assert(width > -1 && height > -1);

        /* allocates memory for bit2 object */
        T bit2 = malloc(sizeof(struct T));
        assert(bit2 != NULL);

        if (width > 0 && height > 0) {
                /* non-empty bit2 */
                bit2->bit = Bit_new(width * height);
        } else { 
                /* empty bit2 */
                bit2->bit = NULL;
        }
        bit2->height = height;
        bit2->width = width;

        return bit2;
}

/* FUNCTION:  Bit2_free
 * Purpose:   Deallocates memory and clears *bit2
 * Arg:       The address of an uarray2 object
 * Returns:   N/A
 * Effect:    Deallocates memory and clears *uarray2
 * Error:     Checked runtime error for *bit2 or bit2 to be null
 */
void Bit2_free(T *Bit2)
{
        assert((*Bit2) != NULL && Bit2 != NULL);
        Bit_free(&((*Bit2)->bit));
        free((*Bit2));
}

/* FUNCTION:  Bit2_width
 * Purpose:   Return the width of a bit2 object
 * Arg:       pointer to bit2 object
 * Returns:   An integer representing the width of an uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for bit2 to be null
 */
int Bit2_width(T Bit2)
{
        assert(Bit2 != NULL);
        return Bit2->width;
}

/* FUNCTION:  Bit2_height
 * Purpose:   Return the numbers of rows of a bit2 object
 * Arg:       pointer to bit2 object
 * Returns:   An integer representing the height of an uarray2
 * Effect:    N/A
 * Error:     Checked runtime error for uarray2 to be null
 */
int Bit2_height(T Bit2)
{
        assert(Bit2 != NULL);
        return Bit2->height;
}

/* FUNCTION:  index_helper
 * Purpose:   Computes the index of a bit from a 2-dimensional array to
 *            an one dimensional array
 * Arg:       T Bit2: pointer to Bit2 object
 *            int rows, col: represents the row and column index respectively
 * Returns:   An integer indicating the bit at (cols, rows)
 * Effect:    N/A
 * Error:     N/A
 */
int index_helper(T Bit2, int cols, int rows)
{
        return ((rows * Bit2->width) + cols);
}

/* FUNCTION:  Bit2_get
 * Purpose:   returns the bit at (cols, rows)
 * Arg:       T Bit2: pointer to Bit2 object
 *            int rows, col: represents the row and column index respectively
 * Returns:   the bit at (cols, rows)
 * Effect:    N/A
 * Error:     Runtime error for Bit2 to be NULL
 *            Runtime error for out of bound cols and rows
 */
int Bit2_get(T Bit2, int cols, int rows)
{
        assert(Bit2 != NULL);
        assert(cols > -1 && rows > -1);
        assert(cols < Bit2->width && rows < Bit2->height);
        return Bit_get(Bit2->bit, index_helper(Bit2, cols, rows));
}

/* FUNCTION:  Bit2_put
 * Purpose:   changes the bit at (cols, rows) to bit_value
 * Arg:       T Bit2: pointer to Bit2 object
 *            int rows, col: represents the row and column index respectively
 *            int bit_value: desirable bit value at (cols, rows)
 * Returns:   original bit value at (cols, rows)
 * Effect:    N/A
 * Error:     Runtime error for Bit2 to be NULL
 *            Runtime error for out of bound cols and rows
 */
int Bit2_put(T Bit2, int cols, int rows, int bit_value)
{
        assert(Bit2 != NULL);
        assert(cols > -1 && rows > -1);
        assert(cols < Bit2->width && rows < Bit2->height);
        return Bit_put(Bit2->bit, index_helper(Bit2, cols, rows), bit_value);
}

/* FUNCTION:  Bit2_map_col_major
 * Purpose:   Calls apply function for every bit in bit2, 
 *            beginning at (0, 0)
 * Arg:       T Bit2: pointer to a bit2 object
 *            void apply: function pointer that gets called on every bit 
 *                       in bit2
 *            void *CL: supplied by client and its value will be changed
 *                      if necessary
 * Returns:   N/A
 * Effect:    Row indices vary more rapidly than column indices.
 *            Closure variable will be changed if necessary
 * Error:     Checked runtime error for bit2 to be null
 */
void Bit2_map_col_major(T Bit2, void apply(int cols, int rows, T a, 
                        int bit_value, void *cl), void *cl) {
        assert(Bit2 != NULL);
        for (int i = 0; i < Bit2->width; i++) {
                for (int j = 0; j < Bit2->height; j++) {
                        apply(i, j, Bit2, Bit2_get(Bit2, i, j), cl);
                }
        }
}

/* FUNCTION:  Bit2_map_row_major
 * Purpose:   Calls apply function for every bit in bit2, 
 *            beginning at (0, 0)
 * Arg:       T Bit2: pointer to bit2 object
 *            void apply: function pointer that gets called on every slot 
 *                       in bit2
 *            void *CL: supplied by client and its value will be changed
 *                      if neccessary
 * Returns:   N/A
 * Effect:    Column indices vary more rapidly than row indices.
 *            Closure variable will be changed if necessary
 * Error:     Checked runtime error for bit2 to be null
 */
void Bit2_map_row_major(T Bit2, void apply(int cols, int rows, T a, 
                        int bit_value, void *cl), void *cl) 
{
        assert(Bit2 != NULL);
        for (int i = 0; i < Bit2->height; i++) {
                for (int j = 0; j < Bit2->width; j++) {
                        apply(j, i, Bit2, Bit2_get(Bit2, j, i), cl);
                }
        }
}
