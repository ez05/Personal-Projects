/*****************************************************************************
 *
 *                                 40image.c
 *
 *     Assignment: Arith
 *     Authors:    Eric Zhao, Leo Kim
 *     Date:       October 26, 2022
 *
 *     Summary:
 *     This is the main function for our executable program 40image. 40image
 *     allows the user to compress a PPM image to a binary representation, and
 *     to decompress a compressed binary representation to a PPM image.
 *     In compression, this program discards information not easily seen by the
 *     human eye (this is lossy compression).
 * 
 *
 ****************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"

static void (*compress_or_decompress)(FILE *input) = compress40;

/* main that handles command line arguments. Calls either compression or
   decompression upon client's request */
int main(int argc, char *argv[])
{
        int i;

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-c") == 0) {
                        compress_or_decompress = compress40;
                } else if (strcmp(argv[i], "-d") == 0) {
                        compress_or_decompress = decompress40;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n",
                                argv[0], argv[i]);
                        exit(1);
                } else if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s -d [filename]\n"
                                "       %s -c [filename]\n",
                                argv[0], argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        
        /* ensure that there is at most one file on the command line */
        assert(argc - i <= 1);

        /* open the file and call compress or decompress depending on
           what the user requested */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                compress_or_decompress(fp);
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }

        return EXIT_SUCCESS; 
}