#ifndef __DECOMP_H__
#define __DECOMP_H__

#include "../structures/sglist.h"
#include "../structures/btree.h"

/*
 * Construye un arbol a partir de una cadena con la codificacion de sus nodos y
 * los valores de sus hojas.
*/
BTree tree_from_encoding(char* encoded_tree);

/*
 * Reconstruye el texto a partir del arbol de Huffman y la secuencia de 1s y 0s
 * que se encuentran en el archivo a descomprimir luego de hacer explode sobre
 * este. Recibe tambien la longitud de esta secuencia, y un puntero para
 * almacenar la longitud del texto decodificado. Retorna un puntero a la
 * cadena decodificada.
*/
char* decode_text(BTree root, char* encoded_text, int encoded_len,
                    int* decode_len);

/*
 * 
 * 
 * 
*/
void decompress(const char* hf_path, char* tree_path, char* dec_path);

#endif /* __DECOMP_H__ */