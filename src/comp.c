#include "comp.h"
#include <stdlib.h>

int compare_freq(CharFreq ch1, CharFreq ch2){
  if (ch1->freq < ch2->freq)
    return -1;
  if (ch1->freq == ch2->freq)
    return 0;
  else
    return 1;
}

CharFreq* calculate_freq(FILE* fp){
  CharFreq* Frequencies = malloc(sizeof(CharFreq) * CHARS);
  CharFreq temp;
  // char* archivo;
  // Incializa el array, con todas las frecuencias en 0
  for (int i = 0; i < CHARS; i++){
    temp->c = NULL;
    temp->freq = 0;
    Frequencies[i] = temp;
  }
  // computar las frecuencias
  // for (int j = 0; j < strlen(archivo); j++){
  //   Frequencies[(unsigned) archivo[j]]->freq += 1;
  //   Frequencies[(unsigned) archivo[j]]->c = archivo[j];
  // }

  // ordenar el array

  return Frequencies;

}