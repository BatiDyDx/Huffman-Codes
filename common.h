#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>
#include <assert.h>

#define NCHARS 256
#define NNODES 511 // 2 * NCHARS - 1


static inline void* id(void* p) { return p; }

static inline int max(int x, int y) {
    return (x > y ? x : y);
}

/*
 * Tipo de function que retorna un puntero a una copia del dato
*/
typedef void* (*CopyFunction)(void* data);

/*
 * Retorna 0 si los argumentos son iguales, un entero negativo
 * si el primer argumento es menor al segundo, o un entero positivo
 * si el primer argumento es mayor al segundo.
*/
typedef int (*CompareFunction)(void*, void*);

/*
 * Funcion que libera la memoria de data
*/
typedef void (*DestroyFunction)(void* data);


#endif /* __COMMON_H__ */