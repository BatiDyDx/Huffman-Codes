# Compresor Huffman

Este programa es una implementación en C de un compresor utilizando
el algoritmo de codificación de Huffman.

## Build
Para compilar el programa:

```
$ make
```
o
```
gcc -Wall -Wextra -Werror -std=c99 src/comp.c src/decomp.c src/io.c
src/main.c structures/btree.c structures/sglist.c -o huff
```

## Compresión
Para comprimir un archivo dir/path:
```
$ ./huff C dir/path
```

La compresión se hará en el archivo dir/path.hf, y el arbol
serializado necesario para la descompresión en el archivo dir/path.tree

## Descompresión
Para descomprimir un archivo dir/path.hf
```
$ ./huff D dir/path.hf
```

Notar que el archivo dir/path.tree debe estar presente en el directorio
del archivo a descomprimir para que el proceso sea posible.

El contenido descomprimido se almacenará en el archivo dir/path.dec

## Testing

Para compilar y correr las pruebas:
```
$ make tests && ./huff_tests
```
