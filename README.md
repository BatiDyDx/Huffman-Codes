# Huffman Compressor/Decompressor

This program is a C implementation of a text compressor for
Huffman encodings.

## Build
To build the program:
```
$ make
```
or
```
gcc -Wall -Wextra -Werror -std=c99 src/comp.c src/decomp.o src/io.o
src/main.c structures/btree.c structures/glist.c structures/sglist.c -o huff
```

## Compress text
To compress text a text file, like f.txt:
```
$ ./huff C f.txt
```

The encoded output will be stored in the file f.txt.hf, and
the serialized tree for the decompression is stored in the file
f.txt.tree

## Decompress text
To decompress a text file, like f.txt.hf:
```
$ ./huff C f.txt
```
Note that f.txt.tree must be present for the decompression to
be possible. The programme will output a file f.txt.dec with
the original content of f.txt

## Testing

To compile tests and run tests:
```
$ make tests && ./huff_tests
```
