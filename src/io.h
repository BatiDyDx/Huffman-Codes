#ifndef __HUFF_IO_H_
#define __HUFF_IO_H_

/*
 * Lee un archivo y devuelve sus contenidos en un buffer obtenido con
 * malloc(). Debe ser liberado luego. En [*len] se guarda la longitud de
 * todo lo leído.
 */
char* readfile(const char* path, int* len);

/*
 * Escribe un archivo con los contenidos del buffer [buf], de longitud
 * [len].
 */
void writefile(const char* path, const char* buf, int len);

/*
 * "Implota" un buffer [bits] compuesto de bits (caracteres ['0'] o
 * ['1']) a un buffer de chars, empacando 8 bits por cada byte (char).
 * Agrega un padding único al final para poder distinguir el punto
 * en el que terminaba la cadena de bits (que puede tener longitud no
 * múltiplo de 8).
 */
char* implode(char* bits, int len, int* nlen);

/*
 * "Explota" un buffer [buf] de longitud [len] a sus bits individuales.
 * Devuelve el buffer nuevo, compuesto solamente de caracteres ['0'] y
 * ['1']. La longitud del buffer devuelto se guarda en [nlen]. Es la
 * inversa de [implode].
 */
char* explode(char* buf, int len, int* nlen);

/*
 * Imprime el mensaje de uso del programa y termina con codigo de error
 */
void usage(void);

/**
 * Agrega una extension al path pasado
 */
char* add_ext(const char* path, const char* ext);

/*
 * Reemplaza la extension de path por una nueva dada. Se debe proveer
 * la longitud de la extension a ser reemplazada
*/
char* replace_ext(const char* path, const char* ext, unsigned old_ext_len);

#endif
