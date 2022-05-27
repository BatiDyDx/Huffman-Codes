#include "decomp.h"
#include "io.h"
#include <string.h>
#include <assert.h>

void null(void* __attribute__((unused))p) { return; }

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

BTree deserialize_tree(char* encoded_tree, int len) {
    return NULL;
}

BTree tree_from_encoding(
    char*  __attribute__((unused))encoded_tree,
    int  __attribute__((unused))len)
{
    while (1);
    
    return NULL;
}

char* decode_text(BTree root, char* encoded_text, const int encoded_len) {
    // Approximate length of the decoded text, if its not enough resize it
    int nchar = 0;
    char* decoded_text = malloc(sizeof(char) * encoded_len);
    assert(decoded_text != NULL);
    if (btree_empty(root))
        return NULL;
    BTree node = root;
    // TODO: resize decoded_text when out of memory
    for (int i = 0; i < encoded_len; i++){
        if (btree_leaf(node)) {
            decoded_text[nchar++] = *((char*)node->data);
            node = root;
            continue;
        }
        switch (encoded_text[i]) {
            case '1':
                node = root->right;
                break;
            case '0':
                node = root->left;
                break;
            default:
                // Shouldnt reach here
                assert(0);
                break;
            }
    }
    return decoded_text;
}

void decompress(const char* path){
    int text_len, tree_len;
    char* tree_path = tree_path_from_file(path);

    char* encoded_text = readfile(path, &text_len);
    char* encoded_tree = readfile(tree_path, &tree_len);   

    BTree huff_tree = tree_from_encoding(encoded_tree, tree_len);
    char* decoded_text = decode_text(huff_tree, encoded_text, text_len);

    free(decoded_text);
    free(encoded_text);
    free(encoded_tree);
    free(tree_path);
    btree_destroy(huff_tree, null);

    return ;
}
