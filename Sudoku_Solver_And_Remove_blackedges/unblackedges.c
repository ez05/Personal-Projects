/**************************************************************
 *
 *                     unblackedges.c
 *
 *     Project:  iii
 *     Authors:  Eric Zhao
 *     Date:     October 3, 2022
 *
 *     Summary:
 *     The purpose of unblackedges.c is to implement an algorithm
 *     that prints on standard output, a plain (P1) format 
 *     portable bitmap file which has width, height and pixel 
 *     values identical to the original file except that all black
 *     edge pixels are changed to white. 
 *     
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bit.h>
#include <mem.h>
#include <seq.h>
#include <pnmrdr.h>
#include "bit2.h"

typedef int number;

/* A struct contains two integer indices,
   col_idx indicates the column indice of an element
   row_idx indicates the row indice of an element
 */
struct Indices {
        int col_idx;
        int row_idx;
};

void unblackedges(FILE *f);
T read_file(FILE *f);
void read_apply(int cols, int rows, T b, int bit_value, void *cl);
void print(T b);
void solve(T b);
void apply_solve(int cols, int rows, T a, int bit_value, void *cl);
void printApply(int cols, int rows, T a, int bit_value, void *cl);
void check_neighbors(T b, int cols, int rows, Seq_T seq);


/* FUNCTION:  main
 * Purpose:   main function that runs the unblackedges program 
 * Arg:       argc: an integer representing the number of arguments in command
 *            argv: an array of arguments  
 * Returns:   An integer indicating the end of the program
 * Effect:    N/A
 * Error:     Runtime error for more than 2 arguments
 *            Runtime error for unsuccessful file read
 */
int main(int argc, char *argv[])
{
        assert(argc < 3);
        if (argc == 1) { 
                /* Reads from stdin. */
                unblackedges(stdin);
        } else { 
                /* Reads from file.
                 * Opens file and check for erroneous reading */
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);
                
                unblackedges(fp);
                
                /* Closes file, sets file pointer to NULL */
                fclose(fp);
                fp = NULL;
        }
        return 0;
}


/* FUNCTION:  unblackedges
 * Purpose:   remove blackedges and their their neighbours given a pbm image
 * Arg:       FILE *f: a pointer to a stream that reads from an input file 
 * Returns:   N/A
 * Effect:    N/A
 * Error:     Runtime error for file stream to be null
 */
void unblackedges(FILE *f)
{
        assert(f != NULL);
        T bit2 = read_file(f);
        solve(bit2);
        print(bit2);
        Bit2_free(&bit2);
}


/* FUNCTION:  print
 * Purpose:   print a plain (P1) format portable bitmap file which has width, 
 *            height and pixel values identical to the original file except 
 *            that all black edge pixels are changed to white
 * Arg:       T b: an pointer to a bit2 object
 * Returns:   N/A
 * Effect:    N/A
 * Error:     Runtime error for bit2 pointer to be null
 */
void print(T b)
{
        assert(b != NULL);
        printf("P1\n");
        printf("%d %d\n", Bit2_width(b), Bit2_height(b));
        Bit2_map_row_major(b, printApply, NULL);
}


/* FUNCTION:  solve
 * Purpose:   turn a plain (P1) format portable bitmap file which has width, 
 *            height and pixel values identical to the original file to a
 *            identical P1 except that all black edge pixels are changed 
 *            to white
 * Arg:       T b: a pointer to a bit2 object
 * Returns:   N/A
 * Effect:    allocates and deallocates a sequence
 * Error:     Runtime error for bit2 pointer to be null
 */
void solve(T b)
{
        assert(b != NULL);
        /* create a sequence */
        int dimension = Bit2_width(b) * Bit2_height(b);
        Seq_T stack = Seq_new(dimension);
        /* handle top bottom left right neighbours */ 
        Bit2_map_row_major(b, apply_solve, stack);
        Seq_free(&stack);
}


/* FUNCTION:  apply_solve
 * Purpose:   turn a blackedge to white, push itself onto the stack,
 *            and checks its neighbours
 * Arg:       int cols, rows: integers representing the indices of an element
 *            T a: a pointer to Bit2 object
 *            int bit_value: integer indicating the value of the element at
 *                           (cols, rows)
 *            void *cl: a void pointer pointing to a Hanson's sequence
 * Returns:   N/A
 * Effect:    Allocates and deallocates a struct indices that holds
 *            a column and a row index
 * Error:     Runtime error for uarray2 pointer to be null
 *            Runtime error for unsuccessful memory allocation for struct
 */
void apply_solve(int cols, int rows, T a, int bit_value, void *cl)
{       
        (void) bit_value;
        /* check if the current bit is at the edge */
        if ((rows != 0) && (cols != 0) && (rows != Bit2_height(a) - 1) 
                && (cols != Bit2_width(a) - 1)) {
                return;
        }
        int edge_bit =  Bit2_get(a, cols, rows);
        if (edge_bit == 1) {
                /* change curr to white */
                Bit2_put(a, cols, rows, 0);

                /* push the indices of the current bit to the stack */
                struct Indices *idx_pair = malloc(sizeof(struct Indices));
                assert(idx_pair != NULL);
                idx_pair->col_idx = cols;
                idx_pair->row_idx = rows;
                Seq_addhi(((Seq_T)cl), idx_pair);
                
                /* clear the stack by checking the neighbours 
                   of the elements in the stack */
                while (Seq_length(((Seq_T)cl)) != 0) {
                        struct Indices* curr_bit = 
                                (struct Indices *)Seq_remhi(cl);
                        int j = curr_bit->col_idx;
                        int i = curr_bit->row_idx;
                        free(curr_bit);

                        /* change curr to white */
                        Bit2_put(a, j, i, 0);
                        /* check_neighbors */
                        check_neighbors(a, j, i - 1, ((Seq_T)cl));
                        check_neighbors(a, j + 1, i, ((Seq_T)cl));
                        check_neighbors(a, j, i + 1, ((Seq_T)cl));
                        check_neighbors(a, j - 1, i, ((Seq_T)cl));
                }
        }
}


/* FUNCTION:  check_neighbors
 * Purpose:   checks the neighbours of a pixel; 
 *            if so, push to the stack; otherwise, move on
 * Arg:       int cols, rows: integers representing the indices of an element
 *            T b: pointer to Bit2 object
 *            Seq_T seq: pointer to a Hanson's sequence
 * Returns:   N/A
 * Effect:    Allocates and deallocates a struct indices that holds
 *            a column and a row index
 * Error:     Runtime error for the sequence pointer to be null
 *            Runtime error for unsuccessful memory allocation for struct
 */
void check_neighbors(T b, int cols, int rows, Seq_T seq) 
{
        assert(seq != NULL);
        /* Out of bound */
        if (cols < 0 || rows < 0) {
                return;
        }
        /* Out of bound */
        if ((cols > Bit2_width(b) - 1) || (rows > Bit2_height(b) - 1)){
                return;
        }
        int curr_bit =  Bit2_get(b, cols, rows);
        /* if the current bit is black... */
        if (curr_bit == 1) {
                struct Indices *idx_pair = malloc(sizeof(struct Indices));
                assert(idx_pair != NULL);
                idx_pair->col_idx = cols;
                idx_pair->row_idx = rows;
                /* pushes itself onto the stack */
                Seq_addhi(seq, idx_pair);
                Bit2_put(b, cols, rows, 0);
        }
}


/* FUNCTION:  read_file
 * Purpose:   leveraging the interface of pnmrdr, read the file,
 *            and write the data from the file to a bit2 object
 * Arg:       FILE *f: a pointer to a stream that reads from an input file
 * Returns:   Pointer to a Bit2 instance
 * Effect:    Allocates and deallocates memory for a pnmrdr object
 *            Allocates memory for a Bit2 objet
 * Error:     Runtime error for providing wrong type of file
 *            Runtime error for out of bound dimensions
 */
T read_file(FILE *f)
{       
        Pnmrdr_T rdr = Pnmrdr_new(f);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);

        /* check for correct file and dimensions */
        assert(data.type == 1);
        assert(data.width != 0 && data.height != 0);

        T bit2 = Bit2_new(data.width, data.height);
        Bit2_map_row_major(bit2, read_apply, &rdr);
        Pnmrdr_free(&rdr);
        
        return bit2;
}


/* FUNCTION:  read_apply
 * Purpose:   store a bit into an Bit2 object
 * Arg:       int cols, rows: integers representing the indices of the bit
 *            T a: a Bit2 object
 *            int bit_value: integer indicating the value of the bit at
 *                           (cols, rows)
 *            void *cl: void pointer of a pnmrdr object
 * Returns:   N/A
 * Effect:    N/A
 * Error:     Runtime error when the bit has a value other than 0 or 1
 */
void read_apply(int cols, int rows, T a, int bit_value, void *cl)
{
        (void) bit_value;
        unsigned int bit = Pnmrdr_get(*((Pnmrdr_T *)cl));
        assert(bit == 0 || bit == 1);
        Bit2_put(a, cols, rows, bit);
}


/* FUNCTION:  printApply
 * Purpose:   print an unblackedged image
 * Arg:       int cols, rows: integers representing the indices of the bit
 *            T a: a Bit2 object
 *            int bit_value: integer indicating the value of the bit at
 *                           (cols, rows)
 *            void *cl: pointer to a closure, it is NULL in this case
 * Returns:   N/A
 * Effect:    N/A
 * Error:     N/A
 */
void printApply(int cols, int rows, T a, int bit_value, void *cl)
{
        (void) rows;
        (void) cl;
        printf("%d", bit_value);
        if (cols == Bit2_width(a) - 1) {
                printf("\n");
        }
}