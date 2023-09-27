/**************************************************************
 *
 *                     sudoku.c
 *
 *     Project:  iii
 *     Authors:  Eric Zhao
 *     Date:     October 3, 2022
 *
 *     Summary:
 *     The purpose of sudoku.c is to implement an algorithm
 *     that if the graymap file (file provided) represents a 
 *     solved sudoku puzzle, the program exits with a code of 0. 
 *     Otherwise it must exit with a code of 1. 
 *     
 *
 **************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <uarray.h>
#include <mem.h>
#include <pnmrdr.h>
#include <set.h>
#include "uarray2.h"

/* a struct contains a set,
 * a boolean indicating whether the set is valid or not, 
 * a boolean indicating whether a function applies by columns or rows
 */
struct Flag {
        Set_T set;
        bool is_valid;
        bool is_col;
};

typedef unsigned number;

const int SUCCESS = 0;
const int FAILURE = 1;
const unsigned int DIM1 = 9;
const unsigned int DIM2 = 9;
const unsigned int DENOMINATOR = 9;
const int ELEMENT_SIZE = sizeof(number);

int sudoku(FILE *f);
T read_file(FILE *f);
bool check_pnm(Pnmrdr_mapdata data);
void read_apply(int cols, int rows, T a, void *val, void *cl);
int solve(T uarray2);
int check_row_or_col (T u, bool col_or_row);
int check_3by3(T u);
void apply_row_or_col(int cols, int rows, UArray2_T a, void *val, void *cl);
void apply_3by3(int cols, int rows, UArray2_T a, void *val, void *cl);
unsigned hash(const void *x);
int cmp(const void *x, const void *y);

/* FUNCTION:  main
 * Purpose:   main function that runs the sudoku program 
 * Arg:       argc: an integer representing the number of arguments in command
 *            argv: an array of arguments  
 * Returns:   An integer indicating whether the problem is solved or not
 * Effect:    N/A
 * Error:     Runtime error for more than 2 arguments
 *            Runtime error for unsuccessful file read
 */
int main(int argc, char *argv[])
{
        assert(argc < 3);
        int predicates = 1;
        if (argc == 1) { 
                /* Reads from stdin. */
                predicates = sudoku(stdin);
        } else { 
                /* Opens and reads from file, check for erroneous reading */
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);

                /* Calls sudoku with file */
                predicates = sudoku(fp);
                
                /* Closes file, sets file pointer to NULL */
                fclose(fp);
                fp = NULL;
        }

        if (predicates == 1) {
                exit(FAILURE);
        } else {
                exit(SUCCESS);
        }
}

/* FUNCTION:  sudoku
 * Purpose:   Determines whether a sudoku problem is solved or not 
 * Arg:       FILE *f: a pointer to a stream that reads from an input file
 * Returns:   An integer indicating whether the sudoku is solved or not
 * Effect:    N/A
 * Error:     Runtime error for file stream to be null
 */
int sudoku(FILE *f)
{
        assert(f != NULL);
        T uarray2 = read_file(f);
        if (uarray2 == NULL) {
                /* incorrect dimensions --> unsolve sudoku */
                return 1;
        } else {
                /* correct file, with correct dimensions and maxval */
                return solve(uarray2);
        }
}

/* FUNCTION:  check_pnm
 * Purpose:   check whether Pnmrdr reads in the correct input file  
 * Arg:       A Pnmrdr_mapdata object that stores data
 * Returns:   N/A
 * Effect:    N/A
 * Error:     Runtime error for not a pgm file
 */
bool check_pnm(Pnmrdr_mapdata data)
{
        assert(data.type == 2);
        /* deem "unsolved sudoku" if width, height, and denominator
           are not correct */
        if (data.width != DIM1 || data.height != DIM2 || 
                data.denominator != DENOMINATOR) {
                return false;
        } else {
                return true;
        }
}

/* FUNCTION:  read_file
 * Purpose:   Read the given file and store it into an uarray2 instance
 * Arg:       FILE *f: a pointer to a stream that reads from an input file
 * Returns:   A pointer to an uarray2 instance
 * Effect:    Allocate memory for a pnmrdr object and an uarray2 object
 * Error:     Runtime error for not a pgm file
 *            Runtime error for incorrect dimension or denominator
 *            Runtime error for bad formatted pgm file
 *            Runtime error to call Pnmrdr_new twice on the same file handle
 */
T read_file(FILE *f)
{       
        Pnmrdr_T rdr = Pnmrdr_new(f);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        bool check = check_pnm(data);
        if (check == true) {
                /* correct file provided with right dimensions and maxval,
                   proceeds... */
                T uarray2 = UArray2_new(DIM1, DIM2, ELEMENT_SIZE);
                UArray2_map_row_major(uarray2, read_apply, &rdr);
                Pnmrdr_free(&rdr);
                return uarray2;
        } else {
                /* incorrect file provided with wrong dimensions and maxval,
                   return NULL... */
                Pnmrdr_free(&rdr);
                return NULL;
        }
}

/* FUNCTION:  read_apply
 * Purpose:   store each element value into an uarray2 instance
 * Arg:       int cols, rows: integers representing the indices of the element
 *            T a: an Uarray2 object
 *            void *val: a void pointer pointing to the value of the element
 *                       at indices (cols, rows)
 *            void *cl: a void pointer pointing to a Pnmrdr reader
 * Returns:   N/A
 * Effect:    N/A
 * Error:     Runtime error when reading more data than promised
 */
void read_apply(int cols, int rows, T a, void *val, void *cl)
{
        (void) cols;
        (void) rows;
        (void) a;
        *((number *)val) = Pnmrdr_get(*((Pnmrdr_T *)cl));
}

/* FUNCTION:  solve
 * Purpose:   solve function returns 0 for solved sudoku 
 *            and 1 for unsolved sudoku
 * Arg:       T uarray2: an pointer to an uarray2 object 
 * Returns:   An integer indicating whether the problem is solved or not
 * Effect:    Reallocates memory for uarray2
 * Error:     Runtime error for uarray2 to be null
 */
int solve(T uarray2)
{
        assert(uarray2 != NULL);
        int solve = 1;
        /* 0 for check row and 1 for check col */
        if (check_row_or_col(uarray2, 0) == 0 && 
            check_row_or_col(uarray2, 1) == 0 &&
            check_3by3(uarray2) == 0) {
                /* if all functions returns 0, 
                   it is an solved sudoku */
                solve = 0;
        }
        UArray2_free(&uarray2);

        return solve;
}

/* FUNCTION:  check_row_or_col
 * Purpose:   check if each column/row contains numbers from 1-9 distinctively
 * Arg:       T uarray2: an pointer to an uarray2 object
 *            bool col_or_row: boolean indicating if it's checking col or row
 * Returns:   return 0 if all columns/rows are valid, return 1 otherwise
 * Effect:    Allocates and deallocate a struct that includes a new set
 *            and a boolean indicating the validity
 * Error:     Runtime error for uarray2 to be null
 */
int check_row_or_col(T u, bool col_or_row)
{
        assert(u != NULL);
        struct Flag *flag = malloc(sizeof(struct Flag));
        flag->set = Set_new(1, cmp, hash);
        flag->is_valid = true;

        if (col_or_row == 0) { /* row major */
                flag->is_col = false;
                UArray2_map_row_major(u, apply_row_or_col, flag);
        } else { /* col major */
                flag->is_col = true;
                UArray2_map_col_major(u, apply_row_or_col, flag);
        }

        Set_free(&(flag->set));

        if (flag->is_valid) {
                /* is a valid row/col */
                free(flag);
                return 0;
        } else {
                /* is not valid row/col */
                free(flag);
                return 1;
        }
}

/* FUNCTION:  check_3by3
 * Purpose:   check if each 3by3 block contains numbers from 1-9 distinctively
 * Arg:       T u: an pointer to an uarray2 object
 * Returns:   return 0 if all 9 blocks are valid, return 1 otherwise
 * Effect:    Allocates and deallocates an array of 9 sets 
 * Error:     Runtime error for uarray2 pointer to be null
 *            Runtime error for unsuccessful memory allocation
 */
int check_3by3(T u)
{
        assert(u != NULL);
        Set_T *array_set = malloc(9 * sizeof(Set_T));
        assert(array_set != NULL);

        /* initialise an array of 9 sets */
        for (unsigned i = 0; i < DIM1; i++) {
                array_set[i] = Set_new(1, cmp, hash);
        }

        /* fill each set with a distinct 3 by 3 block of data */
        UArray2_map_row_major(u, apply_3by3, array_set);

        /* check length of every set */
        int valid = 1; /* 1 for true */
        for (unsigned i = 0; i < DIM1; i++) {
                if (Set_length(array_set[i]) < 9) {
                        valid = 0; /* 0 for false */
                        break;
                }
        }
        for (unsigned i = 0; i < DIM1; i++) {
                Set_free(&(array_set[i]));
        }
        free(array_set);

        if (valid == 0) { /* is not valid */
                return 1;
        } else { /* is valid */
                return 0;
        }
}

/* FUNCTION:  apply_3by3
 * Purpose:   reads each element into the respective set
 * Arg:       int cols, rows: integers representing the indices of the element
 *            UArray2_T a: a pointer to Uarray2 instance
 *            void *val: a void pointer pointing to the value of the element
 *                       at indices (cols, rows)
 *            void *cl: a void pointer pointing to an array of sets
 * Returns:   N/A
 * Effect:    N/A 
 * Error:     N/A
 */
void apply_3by3(int cols, int rows, UArray2_T a, void *val, void *cl)
{
        (void) a;
        int index = ((rows / 3) * 3) + (cols / 3);
        Set_put(((Set_T *)cl)[index], val);
}

/* FUNCTION:  apply_row_or_col
 * Purpose:   determine if a col/row meets the requirement for a solved sudoku
 * Arg:       int cols, rows: integers representing the indices of the element
 *            UArray2_T a: a pointer to Uarray2 instance
 *            void *val: a void pointer pointing to the value of the element
 *                       at indices (cols, rows)
 *            void *cl: a void pointer pointing to an array of sets
 * Returns:   N/A
 * Effect:    Allocates and reallocates a set
 * Error:     N/A
 */
void apply_row_or_col(int cols, int rows, UArray2_T a, void *val, void *cl)
{       
        int num;
        (void) a;

        Set_T curr_set = ((struct Flag *)cl)->set;
        bool *curr_valid = &((struct Flag *)cl)->is_valid;

        if (((struct Flag *)cl)->is_col == false) { /* apply row */
                (void) rows;
                num = cols;
        } else { /* apply column */
                (void) cols;
                num = rows;
        }

        /* for value other than 1-9, set the boolean to false */
        if ((*(int *)val) < 1 || (*(int *)val) > 9) {
                *curr_valid = false;
        }

        /* reaches the end of a row */
        if (num == 8) {
                Set_put(curr_set, val);
                if (Set_length(curr_set) < 9) {
                        *curr_valid = false;
                }
                Set_free(&curr_set);
                Set_T set = Set_new(1, cmp, hash);
                ((struct Flag *)cl)->set = set;
        } else {
                Set_put(curr_set, val);
        }

}

/* FUNCTION:  cmp
 * Purpose:   compare if two numbers have the same value
 * Arg:       two void pointers, each pointing to an integer
 * Returns:   return -1 if n1 is less than n2;
 *            return 1 if n1 is greater
 *            return 0 if equal
 * Effect:    N/A 
 * Error:     N/A
 */
int cmp(const void *x, const void *y)
{
        number n1 = *(number *)x;
        number n2 = *(number *)y;
        if (n1 < n2) {
                return -1;
        } else if (n1 > n2) {
                return 1;
        } else {
                return 0;
        }
}

/* FUNCTION:  hash
 * Purpose:   cast a void pointer to an unsigned value
 * Arg:       a void pointer
 * Returns:   an unsigned number
 * Effect:    N/A 
 * Error:     N/A
 */
unsigned hash(const void *x)
{
        unsigned val;
        val = *(number *)x;
        return val;
}
