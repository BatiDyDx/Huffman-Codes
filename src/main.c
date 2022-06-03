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
	if (strcmp(cmd, "C") == 0) {
		char* hf_path = add_ext(path, ".hf");
		char* tree_path = add_ext(path, ".tree");
		compress(path, hf_path, tree_path);
		free(hf_path);
		free(tree_path);
		printf("Compresion finalizada");
	} else if (strcmp(cmd, "D") == 0) {
		char* tree_path = replace_ext(path, ".tree", 3); // len(".hf") = 3
		char* decoded_path = replace_ext(path, ".dec", 3);
		decompress(path, tree_path, decoded_path);
		free(tree_path);
		free(decoded_path);
		printf("Descompresion finalizada");
	} else
		usage();

	return 0;
}