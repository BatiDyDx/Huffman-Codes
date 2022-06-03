#include "sglist.h"

SGList sglist_init() { return NULL; }

void sglist_free(SGList list, DestroyFunction destroy) {
	SGList node_to_delete;
	while (list != NULL) {
		node_to_delete = list;
		list = node_to_delete->next;
		destroy(node_to_delete->data);
		free(node_to_delete);
	}
}

int sglist_empty(SGList list) { return list == NULL; }

SGList sglist_append_start(SGList list, void *data, CopyFunction copy) {
	SGList new_node = malloc(sizeof(struct _SGNode));
	assert(new_node != NULL);
	new_node->next = list;
	new_node->data = copy(data);
	return new_node;
}

SGList sglist_insert(SGList list, void *data, CopyFunction copy,
											CompareFunction cmp) {
	if (sglist_empty(list) || cmp(data, list->data) <= 0)
		list = sglist_append_start(list, data, copy);
	else {
				SGList node = list;
				while (node->next != NULL && cmp(node->next->data, data) < 0)
						node = node->next;
				node->next = sglist_append_start(node->next, data, copy);
	}
	return list;
}
