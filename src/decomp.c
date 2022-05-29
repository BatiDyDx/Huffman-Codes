#include "decomp.h"
#include "io.h"
#include <string.h>
#include <assert.h>

void null(void* __attribute((unused))p) { return; }

char* tree_path_from_file(const char* path) {
    // Size needed for the path is:
    // len(path.txt.tree) = len(path.txt.hf) + 2
    size_t path_len = strlen(path);
    char* tree_path = malloc(sizeof(char) * (path_len + 3));
    assert(tree_path != NULL);

    strncpy(tree_path, path, path_len - 2);
    strcat(tree_path, "tree");
    return tree_path;
}

BTree deserialize_tree(char** encoded_tree, char** encoded_values) {
    BTree node = btree_join(NULL, NULL, NULL);
    switch (*(*encoded_tree)++) {
        case '0':
            node->left = deserialize_tree(encoded_tree, encoded_values);
            node->right = deserialize_tree(encoded_tree, encoded_values);
            break;
        case '1':
            node->data = malloc(sizeof(char));
            assert(node->data != NULL);
            *((char*) node->data) = *(*encoded_values)++;
            break;
        case '\0':
            // Impossible
            assert(0);
            return NULL;
    }
    return node;
}

BTree tree_from_encoding(char* encoded_tree) {
    assert(encoded_tree != NULL);
    // chequear que no es el caracter nulo unicamente
    assert(*encoded_tree);

    char* encoded_values = encoded_tree;
    while (*encoded_values++);

    return deserialize_tree(&encoded_tree, &encoded_values);
}

char* decode_text(BTree root, char* encoded_text, const int encoded_len, int* decode_len) {
    int nchar = 0;
    // Approximate length of the decoded text.
    size_t size = sizeof(char) * (encoded_len);
    char* decoded_text = malloc(size); /* FIXME */
    assert(decoded_text != NULL);
    if (btree_empty(root))
        return NULL;
    BTree node = root;
    for (int i = 0; i < encoded_len; i++) {
        if ((size_t)nchar == size) {
            size *= 2;
            decoded_text = realloc(decoded_text, size);
        }
        if (btree_leaf(node)) {
            decoded_text[nchar++] = *((char*)node->data);
            node = root;
            continue;
        }
        switch (encoded_text[i]) {
            case '1':
                node = node->right;
                break;
            case '0':
                node = node->left;
                break;
            default:
                // Shouldnt reach here
                assert(0);
                break;
            }
    }
    *decode_len = nchar;
    return decoded_text;
}


/* REMOVE, ONLY TEMPORAL */
void f(void* c) {
    if (c)
        putchar(*((int*) c));
}
/*                      */

void decompress(const char* path) {
    int encode_len, encode_bin_len, tree_encode_len, decode_len;
    char* tree_path = tree_path_from_file(path);

    char* encoded_text = readfile(path, &encode_len);
    char* encoded_text_bin = explode(encoded_text, encode_len, &encode_bin_len);
    printf("Exploded: %s\n", encoded_text_bin);

    char* encoded_tree = readfile(tree_path, &tree_encode_len);

    BTree huff_tree = tree_from_encoding(encoded_tree);

    char* decoded_text = decode_text(huff_tree, encoded_text_bin, encode_bin_len, &decode_len);
    char* decoded_path = "f.txt.dec"; // TODO
    
    writefile(decoded_path, decoded_text, decode_len);

    free(decoded_text);
    free(encoded_text);
    free(encoded_text_bin);
    free(encoded_tree);
    free(tree_path);
    btree_destroy(huff_tree, null);
}
