#include "comp.h"
#include "io.h"
#include <assert.h>
#include <stdlib.h>

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

void free_frequencies(CharFreq* frequencies) {
  for (int i = 0; i < CHARS; i++)
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

SGList create_nodes_from_array(CharFreq* frequencies) {
  SGList nodes = sglist_init();
  for (int i = CHARS - 1; 0 <= i; i--) {
    BTree tmp = btree_join(copy_charfreq(frequencies[i]), NULL, NULL);
    // Since frequencies is a sorted array, inserting backwards
    // is equivalent to inserting always on the first node, so
    // this insertion is O(1)
    nodes = sglist_insert(nodes, tmp, id, (CompareFunction)compare_nodes_freq);
  }
  return nodes;
}

BTree create_huff_tree(CharFreq* frequencies) {
  // TODO: change variable names
  // nodes refers to elements in the list, while
  // node1 and node2 are of type BTree
  SGList nodes = create_nodes_from_array(frequencies);
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
    nodes = temp;
    nodes = sglist_insert(nodes, parent_node, id, (CompareFunction)compare_nodes_freq);
  }
  huff_tree = (BTree)nodes->data;
  free(nodes); // List has only one element
  return huff_tree;
}

char* __attribute__((unused)) encode(
          const char* __attribute__((unused))path,
          BTree __attribute__((unused))huffman_tree)
{
  return NULL;
}

char* __attribute__((unused)) encode_tree(BTree __attribute__((unused))huffman_tree) {
  return NULL;
}

void compress(const char *path) {
  int len = 0;
  char* file_content = readfile(path, &len);

  if (len == 0) {
    fprintf(stderr, "File %s is empty, could not compress its content", path);
    exit(EXIT_FAILURE);
  }

  CharFreq* frequencies = calculate_freq(file_content, len);
  sort_freq(frequencies, CHARS);

  BTree huffman_tree = create_huff_tree(frequencies);
  char* __attribute__((unused)) encoded_string = encode(path, huffman_tree);
  char* __attribute__((unused)) encoded_tree = encode_tree(huffman_tree);
  
  // implode(encoded_string, );
  // writefile(path);

  free(file_content);
  free_frequencies(frequencies);
  btree_destroy(huffman_tree, free);
}
