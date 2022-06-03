#include "comp.h"
#include "io.h"
#include <string.h>
#include <stdio.h>

static inline void* id(void* p) { return p; }

inline int compare_freq(CharFreq* ch1, CharFreq* ch2) {
	return (*ch1)->freq - (*ch2)->freq;
}

void sort_freq(CharFreq* freq_array, int len) {
	qsort(freq_array, len, sizeof(CharFreq),
		(int (*)(const void*, const void*))compare_freq);
}

void create_frequencies(CharFreq* buf, int len) {
	CharFreq temp;
	for (int i = 0; i < len; i++) {
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

inline int compare_nodes_freq(BTree first_node, BTree second_node) {
	return compare_freq((CharFreq*) &(first_node->data), (CharFreq*) &(second_node->data));
}

SGList create_nodes(CharFreq* frequencies, size_t len) {
	sort_freq(frequencies, len);

	SGList nodes_list = sglist_init();
	// Dado que el arreglo esta ordenado, creamos la lista de atras para
	// adelante, insertando asi siempre en el primer lugar de la lista
	for (int i = len - 1; 0 <= i; i--) {
		BTree tmp = btree_join(frequencies[i], NULL, NULL, (CopyFunction) id);
		nodes_list = sglist_insert(nodes_list, tmp, id, (CompareFunction)compare_nodes_freq);
	}
	return nodes_list;
}

BTree create_huff_tree(CharFreq* frequencies, int nchars) {
	SGList nodes_list = create_nodes(frequencies, nchars);
	assert(nodes_list != NULL);
	BTree huff_tree = btree_init();

	while (nodes_list->next != NULL) {
		BTree first_node = (BTree)nodes_list->data;
		BTree second_node = (BTree)nodes_list->next->data;

		CharFreq new_freq = malloc(sizeof(struct _CharFreq));
		assert(new_freq != NULL);

		// The frequency of the parent is the sum of the frequencies of its children
		new_freq->freq = ((CharFreq)(first_node->data))->freq + 
						((CharFreq)(second_node->data))->freq ;

		// new_freq->c se deja como basura, ya que no nos va a importar acceder a este
		// a menos que sea una hoja.
		BTree parent_node = btree_join(new_freq, first_node, second_node, id);

		// Free the first two nodes_list, without freeing its data,
		// because the data is now referenced in parent_node
		SGList temp = nodes_list->next->next;
		free(nodes_list->next);
		free(nodes_list);

		nodes_list = sglist_insert(temp, parent_node, id,
							(CompareFunction)compare_nodes_freq);
	}

	huff_tree = (BTree)nodes_list->data;
	free(nodes_list); // La lista tiene un unico elemento
	return huff_tree;
}

void serialize_tree_and_nodes(BTree root, char* buf_tree, size_t *nnode, 
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
void char_code_from_tree(BTree root, char** chars_encoding,
								char* encoding, size_t depth) {
	// If the tree is a leaf, store the path in chars_encoding
	assert(!btree_empty(root));
	if (root->left == NULL && root->right == NULL) {
		UChar c = ((CharFreq)(root->data))->c;
		encoding[depth] = '\0';

		char* tmp = malloc(depth + 1);
		assert(tmp != NULL);

		memcpy(tmp, encoding, depth + 1);
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
	char* char_code = malloc(max_char_len);
	char_code_from_tree(huff_tree, chars_encoding, char_code, 0);

	free(char_code);
}

char* encode_tree(BTree huffman_tree, size_t nchars) {
	size_t nnode = 0, nleaf = 0;
	const size_t nnodes = 2 * nchars - 1;
	char* buf_leaves = malloc(nchars);
	char* buf_tree = malloc(nnodes);
	assert(buf_tree != NULL && buf_leaves != NULL);

	serialize_tree_and_nodes(huffman_tree, buf_tree, &nnode, buf_leaves, &nleaf);

	char* tree_encoding = malloc(nnodes + nchars);
	assert(tree_encoding != NULL);

	memcpy(tree_encoding, buf_tree, nnodes);
	memcpy(tree_encoding + nnodes, buf_leaves, nchars);

	free(buf_tree);
	free(buf_leaves);

	return tree_encoding;
}

void compress(const char* path, char* hf_path, char* tree_path) {
	int len = 0;
	char* file_content = readfile(path, &len);

	if (len == 0) {
		fprintf(stderr, "El archivo %s esta vacio, no se puede descomprimir\n",
				path);
		exit(EXIT_FAILURE);
	}

	CharFreq frequencies[NCHARS];
	create_frequencies(frequencies, NCHARS);
	calculate_freq(file_content, len, frequencies);

	int encoded_len, reduced_len;
	BTree huffman_tree = create_huff_tree(frequencies, NCHARS);

	// La maxima longitud de la codificacion de un caaracter es la altura del
	// arbol + 1 (para contar a '\0')
	int max_char_len = btree_height(huffman_tree) + 1;

	char* chars_encoding[NCHARS];
	encode_chars(huffman_tree, chars_encoding, max_char_len);
	char* encoded_text = encode_text(file_content, chars_encoding, len,
									max_char_len, &encoded_len);
	char* encoded_tree = encode_tree(huffman_tree, NCHARS);

	char* reduced_encoding = implode(encoded_text, encoded_len, &reduced_len);

	writefile(hf_path, reduced_encoding, reduced_len);
	writefile(tree_path, encoded_tree, NNODES + NCHARS);

	free(file_content);
	free(encoded_text);
	free(encoded_tree);
	free(reduced_encoding);
	for (int i = 0; i < NCHARS; i++)
		free(chars_encoding[i]);
	btree_destroy(huffman_tree, free);
}
