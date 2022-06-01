#include "decomp.h"
#include "io.h"
#include <assert.h>

static void null(void* __attribute__((unused)) p) { return; }

BTree deserialize_tree(char** encoded_tree, char** encoded_values) {
    BTree node = btree_join(NULL, NULL, NULL);
    // Leer de la secuencia de nodos y avanzar
    switch (*(*encoded_tree)++) {
        // Si es un 0, se crea un nodo y sus hijos se crean recursivamente
        case '0':
            node->left = deserialize_tree(encoded_tree, encoded_values);
            node->right = deserialize_tree(encoded_tree, encoded_values);
            break;
        // Si es un 1, se lee de la cadena de valores de hojas y se avanza
        // el puntero al siguiente caracter. Se asigna el caracter al nodo
        case '1':
            node->data = malloc(sizeof(char));
            assert(node->data != NULL);
            *((char*) node->data) = *(*encoded_values)++;
            break;
        // No deberia suceder
        default:
            assert(0);
            break;
    }
    return node;
}

BTree tree_from_encoding(char* encoded_tree) {
    assert(encoded_tree != NULL);
    // Chequear que no es el caracter nulo unicamente
    assert(*encoded_tree);

    char* encoded_values = encoded_tree;
    // Avanzar hasta que comienzan los valores de las hojas
    while (*encoded_values++);

    return deserialize_tree(&encoded_tree, &encoded_values);
}

char* decode_text(BTree root, char* encoded_text, const int encoded_len,
                    int* decode_len) {

    size_t nchar = 0;
    // Longitud aproximada del texto decodifcado.
    // Si no es suficiente, se amplía.
    size_t size = encoded_len;
    char* decoded_text = malloc(size);
    assert(decoded_text != NULL);

    if (btree_empty(root))
        return NULL;

    BTree node = root;

    for (int i = 0; i < encoded_len; i++) {
        // Siguiente nodo es el derecho
        if (encoded_text[i] == '1')
            node = node->right;
        // Siguiente nodo es el izquierdo
        else if (encoded_text[i] == '0')
            node = node->left;
        else // No deberia suceder
            assert(0);

        // Si el nodo es una hoja, se agrega al texto decoficado el caracter
        // en el nodo, y se comienza desde la raiz del arbol de nuevo
        if (node->left == NULL && node->right == NULL) {
            // Si no hay suficiente espacio, se amplia al doble
            if (nchar == size) {
                size *= 2;
                decoded_text = realloc(decoded_text, size);
            }
            decoded_text[nchar++] = *((char*)node->data);
            node = root;
        }
    }
    // Guardamos la longitud del texto decodificado
    *decode_len = nchar;
    return decoded_text;
}

void decompress(const char* path) {
    int encode_len, encode_bin_len, tree_encode_len, decode_len;
    char* tree_path = replace_ext(path, ".tree", 3); // len(".hf") = 3

    char* encoded_text = readfile(path, &encode_len);
    char* encoded_text_bin = explode(encoded_text, encode_len, &encode_bin_len);

    char* encoded_tree = readfile(tree_path, &tree_encode_len);

    BTree huff_tree = tree_from_encoding(encoded_tree);

    char* decoded_text = decode_text(huff_tree, encoded_text_bin,
                                    encode_bin_len, &decode_len);
    char* decoded_path = replace_ext(path, ".dec", 3);

    writefile(decoded_path, decoded_text, decode_len);

    free(decoded_text);
    free(encoded_text);
    free(encoded_text_bin);
    free(encoded_tree);
    free(tree_path);
    btree_destroy(huff_tree, null);
}
