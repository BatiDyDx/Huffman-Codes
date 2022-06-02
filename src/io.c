#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

static inline void quit(const char* s) {
	perror(s);
	exit(1);
}

char* readfile(const char* path, int* len) {
	int sz = 1024;
	char* buf = malloc(sz);
	FILE* f = fopen(path, "rb");
	int c, i = 0;

	if (f == NULL)
		quit("readfile.fopen");

	while ((c = getc(f)) != EOF) {
		if (i == sz) {
			sz = sz * 2;
			buf = realloc(buf, sz);
		}
		buf[i++] = c;
	}

	fclose(f);

	*len = i;
	return buf;
}

void writefile(const char* path, char* buf, int len) {
	FILE* f = fopen(path, "wb");
	int i;

	if (f == NULL)
		quit("writefile.fopen");

	for (i = 0; i < len; i++)
		fputc(buf[i], f);

	fclose(f);
}

static char charbit(int i) {
	if (i)
		return '1';
	else
		return '0';
}

static inline int __attribute__((unused))div_up(int m, int n) {
	return (m + (n - 1)) / n;
}

char* implode(char* bits, int len, int* nlen) {
	int i, j;

	/*
	 * Buffer de retorno, con longitud len/8 más un byte extra (por
	 * padding).
	 */
	char* buf = malloc(len / CHAR_BIT + 1);
	*nlen = len / CHAR_BIT + 1;

	/*
	 * Para cada grupo de 8 bits en [bits], agregamos un caracter a
	 * [buf].
	 */
	for (i = 0, j = 0; i < (len & ~7); j++) {
		char c = 0;
		int k;
		for (k = 0; k < CHAR_BIT; k++)
			c = (c << 1) | (bits[i++] == '1');
		buf[j] = c;
	}

	/*
	 * Padding: agregamos un 1 y tantos ceros como hagan falta para
	 * llegar a un múltiplo de 8
	 */
	{
		/* Nota: [i] contiene la próxima posición de [bits] a leer */
		int k;
		char c = 0;
		int n_left = len - i;
		/* Los bits que faltaban de [bits] */
		for (k = 0; k < n_left; k++)
			c = (c << 1) | (bits[i++] == '1');
		/* Un único 1 */
		c = (c << 1) | 1;
		/* Tantos ceros como hagan falta llenar el byte */
		c = c << (CHAR_BIT - 1 - n_left);
		buf[j++] = c;
	}

	assert(j == *nlen);

	return buf;
}

char* explode(char* buf, int len, int* nlen) {
	int i, j;
	char* bits = malloc(len * CHAR_BIT);

	/* Para cada char en [buf], escribimos 8 bits en el arreglo */
	for (i = 0, j = 0; i < len; i++) {
		int k;
		for (k = CHAR_BIT - 1; k >= 0; k--)
			bits[j++] = charbit(buf[i] & (1 << k));
	}

	/* Borrar el padding que estaba en buf */
	{
		j--;
		/* Buscar hasta el 1 */
		while (bits[j] != '1')
			j--;

		/* Ponerlo en 0 (no '0'), así se puede usar como string */
		bits[j] = 0;
	}

	*nlen = j;

	return bits;
}

void usage(void) {
	puts("Usage:");
	puts("To compress text in file f.txt:");
	puts("huff C f.txt");
	puts("To decompress from a file f.txt.hf, with the corresponding huffman tree in file f.txt.tree:");
	puts("huff D f.txt.hf f.txt.tree");
	exit(EXIT_FAILURE);
}

char* add_ext(const char* path, const char* ext){
	// Agregamos un +1 para agregar el caracter '\0'
	char* new_path = malloc(sizeof(char) * (strlen(path) + strlen(ext) + 1));
	strcpy(new_path, path); // strcat: str1 str2
	strcat(new_path, ext);
	return new_path;
}

char* replace_ext(const char* path, const char* new_ext, unsigned old_ext_len) {
	// Por ejemplo, si se quiere reemplazar el sufijo .txt de por .c en
	// "directorio/archivo.txt" se tendra "directorio/archivo.c", cuya longitud
	// es len("directorio/archivo") + len(".c") - len(".txt") + 1 (para el '\0')
	size_t new_path_len = strlen(path) + strlen(new_ext) - old_ext_len + 1;
	char* new_path = malloc(sizeof(char) * new_path_len);
	assert(new_path != NULL);

	memcpy(new_path, path, strlen(path) - old_ext_len);
	new_path[strlen(path) - old_ext_len] = '\0';
	strcat(new_path, new_ext);
	return new_path;
}