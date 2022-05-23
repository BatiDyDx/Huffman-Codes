#include "comp.h"
#include "io.h"
#include <assert.h>
#include <stdlib.h>

int compare_freq(CharFreq ch1, CharFreq ch2){
  return ch1->freq - ch2->freq;
}

void sort_freq(CharFreq* freq_array, int len) {
  qsort(freq_array, len, sizeof(CharFreq), (int (*)(const void*, const void*))compare_freq);
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

CharFreq* calculate_freq(char* file_content, int len) {
  CharFreq* frequencies = create_frequencies();
  for (int j = 0; j < len; j++)
    frequencies[(UChar) file_content[j]]->freq++;
  return frequencies;
}

SGList create_nodes_from_array(CharFreq* frequencies){
  return frequencies;
}

BTree create_huff_tree(CharFreq* frequencies) {
  SGList nodes = create_nodes_from_array(frequencies);
  BTree huff_tree = btree_init();
  while (nodes->next != NULL){
    BTree tree1 = (BTree)nodes->data;
    BTree tree2 = (BTree)nodes->next->data;
    CharFreq new_freq = malloc(sizeof(struct _CharFreq));
    assert(new_freq != NULL);
    // creamos la nueva frecuencia, que es la suma de las dos frecuencias anteriores
    new_freq->freq = ((CharFreq)(tree1->data))->freq + ((CharFreq)(tree2->data))->freq;
    // new_freq->c se deja como basura, ya que no nos va a importar acceder a este
    // a menos que sea una hoja.
    huff_tree = btree_join(new_freq, tree1, tree2);
    SGList temp = nodes->next->next;
    free(nodes->next);
    free(nodes);
    nodes = temp;
    nodes = sglist_insert(nodes, huff_tree, id, compare_nodes_freq);
  }
  huff_tree = nodes->data;
  free(nodes);
  return huff_tree;
}

void compress(const char *filename) {
  int len = 0;
  char* file_content = readfile(filename, &len);

  if (len == 0) // TODO: Caso de archivo vacio
    assert(0);

  CharFreq* frequencies = calculate_freq(file_content, len);
  sort_freq(frequencies, CHARS);

  BTree huffman_tree = create_huff_tree(frequencies);

  free_frequencies(frequencies);
  btree_destroy(huffman_tree);
}