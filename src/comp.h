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

/*
 * Inicializa el buffer pasado con estructuras CharFreq, cada elemento
 * con su correspodiente caracter y frecuencia 0.
*/
void create_frequencies(CharFreq buf[NCHARS]);

/*
Llena el buffer de frequencias con las frequencias de cada
caracter en str. El buffer ya debe estar inicializado.
*/
void calculate_freq(char* str, int len, CharFreq frequencies[NCHARS]);

/*
Dado un arreglo de frecuencias ya computadas en un texto, crea el arbol
de Huffman para la codificacion de dichos caracteres.
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

/*
 * Comprime el archivo dado por path usando el algoritmo de Huffman.
 * El archivo descomprimido y el arbol codificado usado para la compresión
 * son escritos en hf_path y tree_path respectivamente.
*/
void compress(const char *path, char *hf_path, char *tree_path);

#endif /* __COMP_H__ */