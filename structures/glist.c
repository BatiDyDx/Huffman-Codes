#include "glist.h"

GList glist_init() { return NULL; }

int glist_empty(GList list) { return (list == NULL); }

void glist_free(GList list, DestroyFunction destroy) {
  GNode *node_to_delete;
  while (list != NULL) {
    node_to_delete = list;
    list = list->next;
    destroy(node_to_delete->data);
    free(node_to_delete);
  }
}

GList glist_append_start(GList list, void *data, CopyFunction copy) {
  GNode *new_node = malloc(sizeof(GNode));
  assert(new_node != NULL);
  new_node->next = list;
  new_node->data = copy(data);
  return new_node;
}

GList glist_remove_start(GList list, DestroyFunction destroy) {
  GNode *tmp;
  if (glist_empty(list))
    return NULL;
  tmp = list;
  list = list->next;
  destroy(tmp->data);
  free(tmp);
  return list;
}
