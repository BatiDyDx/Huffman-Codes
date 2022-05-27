#include "comp.h"
#include "decomp.h"
#include "io.h"
#include <stdio.h>
#include <string.h>

int main(int argc, const char** argv) {
  // If argument count is incorrect, or second argument
  // is not a C or D, print usage and exit
  
  const char *cmd, *path;

  if (argc != 3)
    usage();

  cmd = argv[1];
  path = argv[2];
  // Call corresponding compressing or decompressing functions
  if (strcmp(cmd, "C") == 0) { // Compress
    compress(path);
    printf("Compression succeeded");
  } //else if (strcmp(cmd, "D") == 0) {
  //   decompress(path);
  //   printf("Decompression succeeded");
  // } else
  //   usage();

  return 0;
}