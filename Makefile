# Makefile for Huffman encoding and decoding programme

# Compiler and flags
CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c99

# Source files
MAIN = src/main
COMPRESSOR = src/comp
DECOMPRESSOR = src/decomp
IO = src/io

# Build the programme
# If DEBUG flag is set, it compiles
# with the debugging flags
all: $(MAIN).o $(COMPRESSOR).o $(DECOMPRESSOR).o $(IO).o
ifdef DEBUG
	$(CC) -g $(FLAGS) -o huff $^
else	
	$(CC) $(FLAGS) -o huff $^
endif

# Create .o files from .c source files
%.o: %.c
	$(CC) -c $(FLAGS) $^ -o $@


# Clean .o files and debug folder
.PHONY: clean
clean:
	rm -r *.dSYM *.o