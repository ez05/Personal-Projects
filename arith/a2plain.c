/*
 *      a2plain.c
 *      by Eric Zhao, October 2022
 *
 *      Summary:
 *      This is the private methods suite of our 2 dimensional
 *      unboxed array data structure: UArray2. Our UArray2 is represented by
 *      one long single dimensional unboxed array whose indices correspond to
 *      different rows and columns. The UArray2 can be used to store a 2D array
 *      of any type of data that the client chooses to supply. This methods
 *      suite is of type A2Methods_T.
 */

#include <string.h>
#include <assert.h>

#include <a2plain.h>
#include "uarray2.h"


/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/

/*
 * Description: This function makes a new UArray2 based on the given width,
 *              height and element size
 * Expected inputs:  
 *       width: non-negative int for the array width
 *       height: non-negative int for the array height
 *       size: positive int for the bytes occupied by each element
 * Expected outputs: A new instance of the UArray2 struct
 */
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}

/*
 * Description: This function makes a new UArray2 based on the given width,
 *              height and element size. Because this is an unblocked array,
 *              the blocksize goes unused.
 * Expected inputs:  
 *       width: non-negative int for the array width
 *       height: non-negative int for the array height
 *       size: positive int for the bytes occupied by each element
 *       blocksize: unused
 * Expected outputs: A new instance of the UArray2 struct
 */
static A2Methods_UArray2 new_with_blocksize(int width, int height,
                                            int size, int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/*
 * Description:      This function frees the memory associated with the given
 *                   UArray2
 * Expected inputs:  
 *      array2p: A pointer to an initialized UArray2
 * Expected outputs: Nothing
 */
static void a2free(A2Methods_UArray2 *array2p) 
{
        UArray2_free((UArray2_T *) array2p);
}

/*
 * Description: This function returns the width of the given UArray2
 * Expected inputs:
 *       array2: an initialized UArray2
 * Expected outputs: A positive int representing the width
 */
static int width(A2Methods_UArray2 array2)
{
        return UArray2_width(array2);
}

/*
 * Description: This function returns the height of the given UArray2
 * Expected inputs:
 *       array2: an initialized UArray2
 * Expected outputs: A positive int representing the height
 */
static int height(A2Methods_UArray2 array2)
{
        return UArray2_height(array2);
}

/*
 * Description: This function returns the element size of the given UArray2
 * Expected inputs:
 *       array2: an initialized UArray2
 * Expected outputs: A positive int representing the number of bytes
 *                   each element occupies
 */
static int size(A2Methods_UArray2 array2) 
{
        return UArray2_size(array2);
}

/*
 * Description: This function returns 1 because there is no blocksize
 * Expected inputs:
 *       array2: an initialized UArray2
 * Expected outputs: 1, because the UArray2 is unblocked
 */
static int blocksize(A2Methods_UArray2 array2) 
{
        assert(array2 != NULL);
        (void)array2;
        return 1;
}

/*
 * Description: This function returns the location of an element at the
 *              given column and row
 * Expected inputs: 
 *       array2: an initialized UArray2
 *       col: non-negative int for the column in the UArray2
 *       row: non-negative int for the row in the UArray2
 * Expected outputs: A pointer to the corresponding location in the UArray2
 */
static A2Methods_Object *at(A2Methods_UArray2 array2, int col, int row)
{
        return UArray2_at(array2, col, row);
}

/*
 * Description:      This function calls a given function on every element of
 *                   the UArray2, incrementing columns faster than rows
 * Expected inputs:
 *       array: an initialized UArray2
 *       apply: a void function pointer to an apply function
 *       cl: a void pointer to pass in values
 * Expected outputs: Nothing from the function itself. Outputs depend on
 *                   void function that is passed in
 */
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}

/*
 * Description: This function calls a given function on every element of 
 *              the UArray2, incrementing rows faster than columns
 * Expected inputs:
 *       array: an initialized UArray2
 *       apply: a void function pointer to an apply function
 *       cl: a void pointer to pass in values
 * Expected outputs: Nothing from the function itself. Outputs depend on
 *                   void function that is passed in
 */
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}

struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/* 
 * Struct defining which static function to use for each function in the
 * method suite
 */
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,
        at,
        map_row_major,          
        map_col_major,          
        NULL,                   /* map_block_major */
        map_row_major,          /* map_default */
        small_map_row_major,
        small_map_col_major,
        NULL,                   /* small_map_block_major */
        small_map_row_major,    /* small_map_default */
};

/* Here is the exported pointer to the struct */
A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;