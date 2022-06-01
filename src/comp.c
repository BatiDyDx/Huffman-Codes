#include "comp.h"
#include "io.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

static void* id(void* data) { return data; }

static inline int compare_freq(CharFreq* ch1, CharFreq* ch2) {
  return (*ch1)->freq - (*ch2)->freq;
}

void sort_freq(CharFreq* freq_array, int len) {
  qsort(freq_array, len, sizeof(CharFreq),
      (int (*)(const void*, const void*))compare_freq);
}

void create_frequencies(CharFreq buf[NCHARS]) {
  CharFreq temp;
  for (int i = 0; i < NCHARS; i++) {
    temp = malloc(sizeof(struct _CharFreq));
    assert(temp != NULL);
    temp->c = (UChar) i;
    temp->freq = 0;
    buf[i] = temp;
  }
}

void calculate_freq(char* str, int len, CharFreq frequencies[NCHARS]) {
  for (int j = 0; j < len; j++)
    frequencies[(UChar) str[j]]->freq++;
}

static inline int compare_nodes_freq(BTree node1, BTree node2) {
  return compare_freq((CharFreq*) &(node1->data), (CharFreq*) &(node2->data));
}

/*
 * Retorna una SGList donde cada nodo contiene un puntero a un
 * nodo BTree, que formará parte del arbol de la codificaciñon de Huffman.
*/
SGList create_nodes(CharFreq* frequencies, size_t len) {
  sort_freq(frequencies, len);

  SGList nodes = sglist_init();
  // Dado que el arreglo esta ordenado, creamos la lista de atras para
  // adelante, insertando asi siempre en el primer lugar de la lista
  for (int i = len - 1; 0 <= i; i--) {
    BTree tmp = btree_join(frequencies[i], NULL, NULL);
    nodes = sglist_insert(nodes, tmp, id, (CompareFunction)compare_nodes_freq);
  }
  return nodes;
}

BTree create_huff_tree(CharFreq* frequencies, int nchars) {
  // TODO: change variable names
  // nodes refers to elements in the list, while
  // node1 and node2 are of type BTree
  SGList nodes = create_nodes(frequencies, nchars);
  assert(nodes != NULL);
  BTree huff_tree = btree_init();

  while (nodes->next != NULL) {
    BTree node1 = (BTree)nodes->data;
    BTree node2 = (BTree)nodes->next->data;

    CharFreq new_freq = malloc(sizeof(struct _CharFreq));
    assert(new_freq != NULL);

    // The frequency of the parent is the sum of the frequencies of its children
    new_freq->freq = ((CharFreq)(node1->data))->freq + 
                     ((CharFreq)(node2->data))->freq ;

    // new_freq->c se deja como basura, ya que no nos va a importar acceder a este
    // a menos que sea una hoja.
    BTree parent_node = btree_join(new_freq, node1, node2);
    
    // Free the first two nodes, without freeing its data,
    // because the data is now referenced in parent_node
    SGList temp = nodes->next->next;
    free(nodes->next);
    free(nodes);

    nodes = sglist_insert(temp, parent_node, id,
                          (CompareFunction)compare_nodes_freq);
  }

  huff_tree = (BTree)nodes->data;
  free(nodes); // La lista tiene un unico elemento
  return huff_tree;
}

static void serialize_tree_and_nodes(BTree root, char* buf_tree, size_t *nnode, 
                                     char* buf_leaves, size_t *nleaf) {
  if (btree_empty(root))
    return ;
  // Si el nodo no es una hoja, se agrega un '0' a la codificacion de los nodos
  // y se continua recursivamente con los subarboles
  else if (root->left != NULL || root->right != NULL) {
    buf_tree[(*nnode)++] = '0';
    serialize_tree_and_nodes(root->left, buf_tree, nnode, buf_leaves, nleaf);
    serialize_tree_and_nodes(root->right, buf_tree, nnode, buf_leaves, nleaf);
  } else {
    // Si es una hoja, se agrega un '1' a la codificacion de los nodos
    // y el valor de la hoja a la codificacion de las hojas
    buf_tree[(*nnode)++] = '1';
    buf_leaves[(*nleaf)++] = ((CharFreq)(root->data))->c;
  }
}

char* encode_text(char* text, char** chars_encoding, size_t text_len,
                  int max_char_len, int *len) {
  size_t nchar = 0;
  char* coded_text = malloc(text_len * max_char_len);

  for (size_t i = 0; i < text_len; i++)
    for (char* s = chars_encoding[(UChar) text[i]]; *s != '\0'; s++)
      coded_text[nchar++] = *s;

  *len = nchar;
  return coded_text;
}

// Iterate over the huffman tree to get each character codification in binary
static void char_code_from_tree(BTree root, char** chars_encoding,
                                char* encoding, size_t depth) {
  // If the tree is a leaf, store the path in chars_encoding
  assert(!btree_empty(root));
  if (root->left == NULL && root->right == NULL) {
    UChar c = ((CharFreq)(root->data))->c;
    encoding[depth] = '\0';

    char* tmp = malloc(sizeof(char) * (depth + 1));
    assert(tmp != NULL);

    memcpy(tmp, encoding, sizeof(char) * (depth + 1));
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

void encode_chars(BTree huff_tree, char* chars_encoding[NCHARS],
                  int max_char_len) {
  char* char_code = malloc(sizeof(char) * max_char_len);
  char_code_from_tree(huff_tree, chars_encoding, char_code, 0);
  
  free(char_code);
}

char* encode_tree(BTree huffman_tree, size_t nchars, int *tree_len) {
  size_t nnode = 0, nleaf = 0;
  size_t nnodes = btree_nnodes(huffman_tree);
  char* buf_tree = malloc(sizeof(char) * nnodes);
  char* buf_leaves = malloc(sizeof(char) * nchars);
  assert(buf_tree != NULL && buf_leaves != NULL);

  serialize_tree_and_nodes(huffman_tree, buf_tree, &nnode, buf_leaves, &nleaf);

  char* tree_encoding = malloc(sizeof(char) * (nnodes + nchars + 1));
  assert(tree_encoding != NULL);
  
  memcpy(tree_encoding, buf_tree, nnodes);
  tree_encoding[nnodes] = '\0';
  memcpy(tree_encoding + nnodes + 1, buf_leaves, sizeof(char) * nchars);
  
  free(buf_tree);
  free(buf_leaves);

  *tree_len = nnodes + nchars + 1;

  return tree_encoding;
}

void compress(const char* path, char* hf_path, char* tree_path) {
  int len = 0;
  char* file_content = readfile(path, &len);

  if (len == 0) {
    fprintf(stderr, "File %s is empty, could not compress its content", path);
    exit(EXIT_FAILURE);
  }

  CharFreq frequencies[NCHARS];
  create_frequencies(frequencies);
  calculate_freq(file_content, len, frequencies);

  int encoded_len, reduced_len, tree_len;
  BTree huffman_tree = create_huff_tree(frequencies, NCHARS);

  // Max len of a character codification is the height of the tree
  // (+ 1 to count '\0')
  int max_char_len = btree_height(huffman_tree) + 1;

  char* chars_encoding[NCHARS];
  encode_chars(huffman_tree, chars_encoding, max_char_len);
  char* encoded_text = encode_text(file_content, chars_encoding, len,
                                    max_char_len, &encoded_len);
  char* encoded_tree = encode_tree(huffman_tree, NCHARS, &tree_len);

  char* reduced_encoding = implode(encoded_text, encoded_len, &reduced_len);
  
  writefile(hf_path, reduced_encoding, reduced_len);
  writefile(tree_path, encoded_tree, tree_len);
  
  free(file_content);
  free(encoded_text);
  free(encoded_tree);
  free(reduced_encoding);
  for (int i = 0; i < NCHARS; i++)
    free(chars_encoding[i]);
  btree_destroy(huffman_tree, free);
}
