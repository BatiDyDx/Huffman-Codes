# Makefile para compresor y descompresor Huffman

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

# Archivos fuente
MAIN = src/main
COMPRESSOR = src/comp
DECOMPRESSOR = src/decomp
IO = src/io
BTREE = structures/btree
SGLIST = structures/sglist

SOURCE = $(COMPRESSOR).o $(DECOMPRESSOR).o $(IO).o
STRUCTURES = $(BTREE).o $(SGLIST).o
TESTS = tests/main.c 

# Compilar el programa
all: $(MAIN).o $(SOURCE) $(STRUCTURES)
	$(CC) $(CFLAGS) -o huff $^

# Crear .o desde archivos .c
%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Compilar para depuracion
.PHONY: debug
debug:
	$(CC) -g -o huff $(CFLAGS) $(MAIN).c $(COMPRESSOR).c \
	$(DECOMPRESSOR).c $(IO).c $(BTREE).c $(SGLIST).c

tests: $(SOURCE) $(STRUCTURES)
	$(CC) $(CFLAGS) -o huff_tests $^ $(TESTS)

# Remover archivos .o, ejecutables, etc
.PHONY: clean
clean:
	@-rm -r *.dSYM 2>/dev/null || true
	@-rm src/*.o structures/*.o 2>/dev/null || true
	@-rm huff huff_tests 2>/dev/null || true
