# Makefile for Huffman encoding and decoding programme

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

# Source files
MAIN = src/main
COMPRESSOR = src/comp
DECOMPRESSOR = src/decomp
IO = src/io
BTREE = structures/btree
SGLIST = structures/sglist

SOURCE = $(COMPRESSOR).o $(DECOMPRESSOR).o $(IO).o
STRUCTURES = $(BTREE).o $(SGLIST).o
TESTS = tests/main.c tests/test_comp.c tests/test_decomp.c

# Build the programme
all: $(MAIN).o $(SOURCE) $(STRUCTURES)
	$(CC) $(CFLAGS) -o huff $^

# Create .o files from .c source files
%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Compile source files with debug information
.PHONY: debug
debug:
	$(CC) -g -fsanitize=address -o huff $(CFLAGS) $(MAIN).c $(COMPRESSOR).c \
	$(DECOMPRESSOR).c $(IO).c $(BTREE).c $(SGLIST).c

tests: $(SOURCE) $(STRUCTURES)
	$(CC) $(CFLAGS) -o huff_tests $^ $(TESTS)

# Clean .o files, debug folder and executables
.PHONY: clean
clean:
	-rm -r *.dSYM
	-rm src/*.o structures/*.o
	-rm huff huff_tests
