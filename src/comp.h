#ifndef __COMP_H__
#define __COMP_H__

#include "../structures/btree.h"
#include "../structures/sglist.h"

#define CHARS 256

typedef unsigned char UChar;

struct _CharFreq {
    UChar c;
    size_t freq;
};

typedef struct _CharFreq *CharFreq;

/**
 * Funcion identidad, toma un dato y retorna este mismo.
 */
void* id(void* data);

/**
 * Compara las frecuencias de datos de tipo CharFreq.
 * Retorna un entero positivo si el primer argumento es mayor,
 * 0 si son iguales o un entero negativo si es menor.
 */
int compare_freq(CharFreq* ch1, CharFreq* ch2);

/**
 * Ordena un array de CharFreq de menor a mayor, usando el algoritmo
 * de ordenamiento qsort.
 */
void sort_freq(CharFreq* freq_array, int len);

/**
 * Retorna una copia de una estructura de tipo CharFreq.
 */
CharFreq copy_charfreq(CharFreq ch);

/**
 * Crea un array de estructuras de CharFreq, donde cada caracter esta
 * en la posicion [caracter] de este array.
 */
void create_frequencies(CharFreq* buf, int len);

/**
 * Retorna un array de CharFreq donde cada elemento
 * corresponde a la estructura, conteniendo un caracter y
 * el numero de veces que aparece en el string pasado.
 */
void calculate_freq(char* str, int len, CharFreq frequencies[CHARS]);

/**
 * Similar a compare_freq, solo que toma argumentos del
 * tipo BTree.
 */
int compare_nodes_freq(BTree node1, BTree node2);

/**
 * Toma un array de frecuencias y su longitud y crea una SGList
 * con todos los elementos del array, donde cada nodo contiene un 
 * dato de tipo BTree, que a la vez este BTree contiene un CharFreq.
 */
SGList create_nodes_from_array(CharFreq* frequencies, size_t len);


/**
 * Toma un arbol de Huffman y codifica el arbol en una 
 * cadena de 0s y 1s. Escribe 0 si la raiz actual es un
 * nodo intermedio y 1 si es una hoja.
 */
void serialize_tree_and_nodes(BTree root, char* tree_repr, size_t *nnode, 
                                     char* buf_leaves, size_t *nleaf);

/**
 * Dado un array de CharFreq, se crea el arbol de Huffman
 * correspondiente a la frecuencia de caracteres en el archivo
 * a comprimir.
 */
BTree create_huff_tree(CharFreq* frequencies, size_t nchars);

/**
 * Toma un arbol de huffman, 
void encode_chars(BTree huff_tree, char* chars_encoding[CHARS],
                    int max_char_len);

char* encode_text(char* text, char** chars_encoding, size_t text_len,
                    int max_char_len, int* len);

void char_code_from_tree(BTree root, char** chars_encoding,
                                char* encoding, size_t depth);                

*/
char* encode_tree(BTree huffman_tree, size_t nchars, int* encode_tree);


/*
Takes a path to a file to be compressed using Huffman
algorithm, and writes the result to the file path.hf, and the
corresponding tree used to encode it to the file path.hf.tree
*/
void compress(const char *path);

#endif /* __COMP_H__ */