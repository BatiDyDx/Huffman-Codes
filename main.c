#include "comp.h"
#include "decomp.h"
#include "io.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char* const argv[]) {

    // If argument count is incorrect, or second argument
    // is not a C or D, print usage and exit
    if (argc != 3 || (argv[1] != 'C' && argv[1] != 'D')) {
        usage();
        exit(1);
    }

    // Call corresponding compressing or decompressing functions


    return 0;
}