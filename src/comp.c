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

BTree create_huff_tree(CharFreq* __attribute__((unused))frequencies) {
  // TODO: implement huffman tree creation algorithm
  return NULL;
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