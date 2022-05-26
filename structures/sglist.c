#include "sglist.h"

SGList sglist_init() { return NULL; }

void sglist_free(SGList list, DestroyFunction destroy) {
  GNode* node_to_delete;
  while (list != NULL) {
    node_to_delete = list;
    list = node_to_delete->next;
    destroy(node_to_delete->data);
    free(node_to_delete);
  }
}

int sglist_empty(SGList list) { return list == NULL; }

SGList sglist_insert(SGList list, void *data, CopyFunction copy, CompareFunction cmp) {
  GNode *head;
  if (sglist_empty(list) || cmp(data, list->data) < 0)
    head = glist_append_start(list, data, copy);
  else {
    GNode *node;
    for (node = list; node->next != NULL; node = node->next)
      if (cmp(data, node->next->data) < 0)
        break;
    node->next = glist_append_start(node->next, data, copy);
    head = list;
  }
  return head;
}

int sglist_search(SGList list, void *data, CompareFunction cmp) {
  int cmp_flag;
  for (GNode* node = list; node != NULL; node = node->next) {
    cmp_flag = cmp(data, node->data);
    if (cmp_flag == 0)
      return 1;
    else if (cmp_flag > 0)
      return 0;
  }
  return 0;
}
