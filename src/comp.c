#include "comp.h"
#include "io.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void* id(void* data) { return data; }

int compare_freq(CharFreq ch1, CharFreq ch2){
  return ch1->freq - ch2->freq;
}

void sort_freq(CharFreq* freq_array, int len) {
  qsort(freq_array, len, sizeof(CharFreq), (int (*)(const void*, const void*))compare_freq);
}

CharFreq copy_charfreq(CharFreq ch) {
  CharFreq copy = malloc(sizeof(struct _CharFreq));
  assert(copy != NULL);
  copy->c = ch->c;
  copy->freq = ch->freq;
  return copy;
}

CharFreq* create_frequencies() {
  CharFreq* frequencies = malloc(sizeof(CharFreq) * CHARS);
  assert(frequencies != NULL);
  CharFreq temp;
  // char* archivo;
  // Incializa el array, con todas las frecuencias en 0
  for (int i = 0; i < CHARS; i++) {
    temp = malloc(sizeof(struct _CharFreq));
    assert(temp != NULL);
    temp->c = (UChar) i;
    temp->freq = 0;
    frequencies[i] = temp;
  }

  return frequencies;
}

void free_frequencies(CharFreq* frequencies, size_t len) {
  for (size_t i = 0; i < len; i++)
    free(frequencies[i]);
  free(frequencies);
}

CharFreq* calculate_freq(char* str, int len) {
  CharFreq* frequencies = create_frequencies();
  for (int j = 0; j < len; j++)
    frequencies[(UChar) str[j]]->freq++;
  return frequencies;
}

int compare_nodes_freq(BTree node1, BTree node2) {
  return compare_freq(node1->data, node2->data);
}

SGList create_nodes_from_array(CharFreq* frequencies, size_t len) {
  
  sort_freq(frequencies, len);

  SGList nodes = sglist_init();
  for (int i = len - 1; 0 <= i; i--) {
    BTree tmp = btree_join(copy_charfreq(frequencies[i]), NULL, NULL);
    // Since frequencies is a sorted array, inserting backwards
    // is equivalent to inserting always on the first node, so
    // this insertion is O(1)
    nodes = sglist_insert(nodes, tmp, id, (CompareFunction)compare_nodes_freq);
  }
  return nodes;
}

void serialize_tree_and_nodes(BTree root, char* tree_repr, size_t *nnode, char* buf_leaves, size_t *nleaf) {
  if (btree_empty(root))
    return ;
  else if (!btree_leaf(root)) {
    tree_repr[(*nnode)++] = '0';
    serialize_tree_and_nodes(root->left, tree_repr, nnode, buf_leaves, nleaf);
    serialize_tree_and_nodes(root->right, tree_repr, nnode, buf_leaves, nleaf);
  } else {
    tree_repr[(*nnode)++] = '1';
    buf_leaves[(*nleaf)++] = ((CharFreq)(root->data))->c;
  }
}

BTree create_huff_tree(CharFreq* frequencies, size_t nchars) {
  // TODO: change variable names
  // nodes refers to elements in the list, while
  // node1 and node2 are of type BTree
  SGList nodes = create_nodes_from_array(frequencies, nchars);
  assert(nodes != NULL);

  BTree huff_tree = btree_init();
  while (nodes->next != NULL) {
    BTree node1 = (BTree)nodes->data;
    BTree node2 = (BTree)nodes->next->data;

    CharFreq new_freq = malloc(sizeof(struct _CharFreq));
    assert(new_freq != NULL);

    // The frequency of the parent is the sum of the frequencies of its children
    new_freq->freq = ((CharFreq)(node1->data))->freq + ((CharFreq)(node2->data))->freq;

    // new_freq->c se deja como basura, ya que no nos va a importar acceder a este
    // a menos que sea una hoja.
    BTree parent_node = btree_join(new_freq, node1, node2);
    
    // Free the first two nodes, without freeing its data,
    // because the data is now referenced in 
    SGList temp = nodes->next->next;
    free(nodes->next);
    free(nodes);

    nodes = sglist_insert(temp, parent_node, id, (CompareFunction)compare_nodes_freq);
  }
  huff_tree = (BTree)nodes->data;
  free(nodes); // List has only one element
  return huff_tree;
}

char* encode_text(const char* path, char** chars_encoding, BTree huff_tree) {
  int path_len = strlen(path);
  int len_max_code_char = btree_height(huff_tree) + 1;
  char* coded_text = malloc(sizeof(char) * path_len * len_max_code_char);
  for (int i = 0; i < path_len; i++)
    // for (char c = chars_encoding[])
    strcat(coded_text, chars_encoding[(UChar)path[i]]);
  return coded_text;
}

/*
// TODO: La funcion no se encarga de evitar embordamientos, implementarlo
unsigned char* concatenar_char_a_cadena(unsigned char c, unsigned char* string){
  unsigned char string_temporal[2];
  string_temporal[0] = c;
  string_temporal[0] = '\0';
  return (unsigned char*)strcat((char*)string, (char*)string_temporal);
}
*/

// Iterate over the huffman tree to get each character codification in binary
void char_code_from_tree(BTree root, char** chars_encoding, char* encoding, size_t depth) {
  // If the tree is a leaf, store the path in chars_encoding
  if (btree_leaf(root)) {
    UChar c = ((CharFreq)(root->data))->c;
    encoding[depth] = '\0';

    char* tmp = malloc(sizeof(UChar) * depth);
    assert(tmp != NULL);

    strcpy(tmp, encoding);
    chars_encoding[(UChar) c] = tmp;

    return;
  }
  // Recorre el subarbol izquierdo, agregando un '0' al recorrido actual
  encoding[depth] = '0';
  char_code_from_tree(root->left, chars_encoding, encoding, depth + 1);
  // Recorre el subarbol derecho, agregando un '1' al recorrido actual
  encoding[depth] = '1';
  char_code_from_tree(root->right, chars_encoding, encoding, depth + 1);
}

char** chars_encoding(BTree huff_tree) {
  // Max len of a character codification is the height of the tree
  // (+ 1 to count '\0')
  size_t max_len_encoding = (btree_height(huff_tree) + 1);
  char** chars_encoding = malloc(sizeof(char*) * CHARS);
  char* char_code = malloc(sizeof(char) * max_len_encoding);
  char_code_from_tree(huff_tree, chars_encoding, char_code, 0);
  
  free(char_code);
  return chars_encoding;
}

char* encode_tree(BTree huffman_tree, size_t nchars, int* encode_tree) {
  size_t nnode = 0, nleaf = 0;
  size_t nnodes = btree_nnodes(huffman_tree);
  char* buf_tree = malloc(sizeof(char) * nnodes);
  char* buf_leaves = malloc(sizeof(char) * nchars);
  assert(buf_tree != NULL && buf_leaves != NULL);

  serialize_tree_and_nodes(huffman_tree, buf_tree, &nnode, buf_leaves, &nleaf);

  char* tree_encoding = malloc(sizeof(char) * (nnodes + nchars + 2));
  assert(tree_encoding != NULL);
  strncpy(tree_encoding, buf_tree, nnodes);
  strncpy(tree_encoding + nnodes + 1, buf_leaves, nchars);
  tree_encoding[nnodes] = '\0';
  tree_encoding[nnodes + nchars + 1] = '\0';
  
  free(buf_tree);
  free(buf_leaves);

  return tree_encoding;
}

void compress(const char *path) {
  int len = 0;
  char* file_content = readfile(path, &len);

  if (len == 0) {
    fprintf(stderr, "File %s is empty, could not compress its content", path);
    exit(EXIT_FAILURE);
  }

  CharFreq* frequencies = calculate_freq(file_content, len);

  int encoded_len, reduced_len, tree_len;
  BTree huffman_tree = create_huff_tree(frequencies, CHARS);
  char* __attribute__((unused)) encoded_text = encode_text(path, huffman_tree, &encoded_len);
  char* __attribute__((unused)) encoded_tree = encode_tree(huffman_tree, CHARS, &tree_len);
  
  char* reduced_encoding = implode(encoded_text, encoded_len, &reduced_len);
  // writefile(); path.hf, reduced_encoding, reduded_len
  // writefile(); path.tree, encoded_tree, tree_len
  
  free(file_content);
  free_frequencies(frequencies, CHARS);
  free(encoded_text);
  free(encoded_tree);
  free(reduced_encoding);
  btree_destroy(huffman_tree, free);
}
