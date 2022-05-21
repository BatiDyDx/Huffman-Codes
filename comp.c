#include <string.h>

int compare(const void *_a, const void *_b) {
  int *a, *b; 
  a = (int *) _a;
  b = (int *) _b;
  return (*a - *b);
}

int* freq(char* cadena){
  int* freq = malloc(sizeof(int) * 255);
  for (int i = 0; i < strlen(cadena); i++)
    freq[(unsigned) cadena[i]] += 1;
  qsort(freq, 255, sizeof(int), &compare);
  return freq;
}