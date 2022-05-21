# Makefile for Huffman encoding and decoding programme

# Compiler and flags
CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c99

# Source files
MAIN = src/main
COMPRESSOR = src/comp
DECOMPRESSOR = src/decomp
IO = src/io
BTREE = structures/btree
GLIST = structures/glist
SGLIST = structures/sglist

SOURCE = $(COMPRESSOR).o $(DECOMPRESSOR).o $(IO).o
STRUCTURES = $(BTREE).o $(GLIST).o $(SGLIST).o
TESTS = tests/main.c tests/test_comp.c tests/test_decomp.c

# Build the programme
# If DEBUG flag is set, it compiles
# with the debugging flags
all: $(MAIN).o $(SOURCE) $(STRUCTURES)
ifdef DEBUG
	$(CC) -g $(FLAGS) -o huff $^
else	
	$(CC) $(FLAGS) -o huff $^
endif

# Create .o files from .c source files
%.o: %.c
	$(CC) -c $(FLAGS) $^ -o $@

tests: $(SOURCE) $(STRUCTURES)
	$(CC) $(FLAGS) -o huff_test $^ $(TESTS)


# Clean .o files and debug folder
.PHONY: clean
clean:
	-rm -r *.dSYM
	-rm src/*.o structures/*.o
	-rm huff huff_test