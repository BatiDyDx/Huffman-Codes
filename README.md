# Compresor Huffman
**Alumnos: Bautista Peirone, Angelo Alvarez**

Este programa es una implementación en C de un compresor utilizando
el algoritmo de codificación de Huffman.

## Build
Para compilar el programa:

```
$ make
```
o
```
$ gcc src/comp.c src/decomp.c src/io.c src/main.c structures/btree.c
structures/sglist.c -o huff
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

## Script para testing
El archivo `rcd.sh` es un script pensado para facilitar la compresión,
descompresión y comparación de archivos para facilitar el testeo del programa.

El script recibe una serie de archivos, compila el codigo el programa, comprime
cada archivo, descomprime y compara las diferencias entre el archivo original y
el decodificado. (Para los procesos de compresión y descompresión, toma el
tiempo de cada uno con el programa `time` de bash). Finalmente, limpia el
directorio. Su uso es como sigue, suponemos que queremos probar el programa
sobre los siguientes tres archivos: `f.txt` `g.pdf` `h.png`, luego ejecutamos

```
$ ./rcd.sh f.txt g.pdf h.png
```
