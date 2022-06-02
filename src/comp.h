#ifndef __COMP_H__
#define __COMP_H__

#include "../common.h"
#include "../structures/btree.h"
#include "../structures/sglist.h"

typedef unsigned char UChar;

struct _CharFreq {
    UChar c;
    size_t freq;
};

typedef struct _CharFreq *CharFreq;

/*
 * Retorna una SGList donde cada nodo contiene un puntero a un
 * nodo BTree, que formará parte del arbol de la codificaciñon de Huffman.
*/
SGList create_nodes(CharFreq* frequencies, size_t len);

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
 * en la posicion [caracter] de este array, y su frecuencia es 0.
 */
void create_frequencies(CharFreq* buf, int len);

/**
 * Retorna un array de CharFreq donde cada elemento
 * corresponde a la estructura, conteniendo un caracter y
 * el numero de veces que aparece en el string pasado.
 */
void calculate_freq(char* str, int len, CharFreq frequencies[NCHARS]);

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
BTree create_huff_tree(CharFreq* frequencies, int nchars);

/*
 * Computa la cadena de 1s y 0s que codifica cada caracter dado el arbol
 * como argumento, y lo almacena en el buffer. max_char_len será la
 * longitud maxima usada para codificar un caracter (altura(arbol) + 1)
*/
void encode_chars(BTree huff_tree, char* chars_encoding[NCHARS],
                    int max_char_len);

/*
 * Dado una cadena de caracters y un buffer que almacena la representación
 * de cada caracter, se codifica text en 1s y 0s. Toma un puntero para a int
 * almacenar la longitud de la codificacion.
*/
char* encode_text(char* text, char** chars_encoding, size_t text_len,
                    int max_char_len, int* len);

/*
 * Devuelve la codificacion de un arbol de Huffman. El parametro nchars
 * permite codificar arboles de distinta cantidad de hojas.
*/
char* encode_tree(BTree huffman_tree, size_t nchars);

void char_code_from_tree(BTree root, char** chars_encoding,
                                char* encoding, size_t depth);                

/*
 * Comprime el archivo dado por path usando el algoritmo de Huffman.
 * El archivo descomprimido y el arbol codificado usado para la compresión
 * son escritos en hf_path y tree_path respectivamente.
*/
void compress(const char *path, char *hf_path, char *tree_path);

#endif /* __COMP_H__ */