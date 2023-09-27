/*
 *      uarray2.c
 *      by Eric Zhao
 *      Project:    Arith
 *
 *      Summary:
 *              This is the private implementation of our 2 dimensional
 *      unboxed array data structure: UArray2. Our UArray2 is represented by
 *      one long single dimensional unboxed array whose indices correspond to
 *      different rows and columns. The UArray2 can be used to store a 2D array
 *      of any type of data that the client chooses to supply.
 */

#include <stdlib.h>
#include <assert.h>
#include "uarray2.h"
#include "uarray2rep.h"


/*
 * Description: This function makes a new UArray2 based on the given width,
 *              height and element size
 * Expected inputs:  
 *       width: non-negative int for the array width
 *       height: non-negative int for the array height
 *       elemSize: positive int for the bytes occupied by each element
 * Expected outputs: A new instance of the UArray2 struct
 */
UArray2_T UArray2_new(int width, int height, int elemSize)
{
        /* test to make sure valid inputs given */
        assert(width >= 0);
        assert(height >= 0);
        
        /* allocate memory for the UArray2 */
        UArray2_T tempArr;
        tempArr = malloc(sizeof(*tempArr));
        assert(tempArr != NULL);

        /* initialize the UArray2 private data members */
        tempArr->width = width;
        tempArr->height = height;
        tempArr->Uarray = UArray_new(width * height, elemSize);
        

        return tempArr;
}


/*
 * Description: This function returns the width of the given UArray2
 * Expected inputs:
 *       array: an initialized UArray2
 * Expected outputs: A positive int representing the width
 */
int UArray2_width(UArray2_T array)
{
        assert(array != NULL);
        return array->width;
}
 
/*
 * Description: This function returns the height of the given UArray2
 * Expected inputs:
 *       array: an initialized UArray2
 * Expected outputs: A positive int representing the height
 */
int UArray2_height(UArray2_T array)
{
        assert(array != NULL);
        return array->height;
}
 
/*
 * Description: This function returns the element size of the given UArray2
 * Expected inputs:
 *       array: an initialized UArray2
 * Expected outputs: A positive int representing the number of bytes
 *                   each element occupies
 */
int UArray2_size(UArray2_T array)
{
        assert(array != NULL);
        return UArray_size(array->Uarray);
}
 
/*
 * Description: This function returns the location of an element at the
 *              given column and row
 * Expected inputs: 
 *       array: an initialized UArray2
 *       col: non-negative int for the column in the array
 *       row: non-negative int for the row in the array
 * Expected outputs: A pointer to the corresponding location in the UArray2
 */
void *UArray2_at(UArray2_T array, int col, int row)
{
        /* test to make sure valid inputs are given */
        assert(array != NULL);
        assert(col < array->width && col >= 0);
        assert(row < array->height && row >= 0);

        /* calculate the array index from the row and column */
        int loc = row * array->width + col;

        /* return a void pointer to the corresponding location */
        return UArray_at(array->Uarray, loc);
}
 
/*
 * Description: This function calls a given function on every element of 
 *              the UArray2, incrementing rows faster than columns
 * Expected inputs:
 *       array: an initialized UArray2
 *       apply: a void function pointer to the function apply, which takes in
 *               col: non-negative int for the column in the array
 *               row: non-negative int for the row in the array
 *               array: an initialized UArray2
 *               loc: a void pointer to the corresponding location in the array
 *               cl: a void pointer to pass in values
 *       cl: a void pointer to pass in values
 * Expected outputs: Nothing from the function itself. Outputs depend on
 *                   void function that is passed in
 */
void UArray2_map_col_major(UArray2_T array, 
                           void apply(int col, int row, UArray2_T array, 
                                      void *loc, void *cl), 
                           void *cl) 
{
        /* test to make sure valid inputs given */
        assert(array != NULL);
        assert(apply != NULL);

        /* 
         * move through the array incrementing rows faster than columns 
         * and call the apply function on every element
         */
        int col, row;
        for (col = 0; col < array->width; col++) {
                for (row = 0; row < array->height; row++) {
                        apply(col, row, array, 
                        UArray2_at(array, col, row), cl);
                }
        }
} 

/*
 * Description:      This function calls a given function on every element of
 *                   the UArray2, incrementing columns faster than rows
 * Expected inputs:
 *       array: an initialized UArray2
 *       apply: a void function pointer to the function apply, which takes in
 *               col: non-negative int for the column in the array
 *               row: non-negative int for the row in the array
 *               array: an initialized UArray2
 *               loc: a void pointer to the corresponding location in the array
 *               cl: a void pointer to pass in values
 *       cl: a void pointer to pass in values
 * Expected outputs: Nothing from the function itself. Outputs depend on
 *                   void function that is passed in
 */
void UArray2_map_row_major(UArray2_T array, 
                           void apply(int col, int row, UArray2_T array,
                                     void *loc, void *cl),
                           void *cl)
{
        /* test to make sure valid inputs are given */
        assert(array != NULL);
        assert(apply != NULL);
        
        /* 
         * move through the array incrementing columns faster than rows 
         * and call the apply function on every element
         */
        int i;
        for (i = 0; i < (array->width * array->height); i++) {
                int col = i % array->width;
                int row = i / array->width;
                
                apply(col, row, array, UArray2_at(array, col, row), cl);
        }
}
 
/*
 * Description:      This function frees the memory associated with the given
 *                   UArray2
 * Expected inputs:  
 *      array: A pointer to an initialized UArray2
 * Expected outputs: Nothing
 */
void UArray2_free(UArray2_T *array) 
{
        /* test to make sure valid inputs are given */
        assert(array != NULL);
        assert(*array != NULL);

        /* free the memory associated with the UArray2 */
        UArray_free(&((*array)->Uarray));
        free(*array);

        (*array) = NULL;
}