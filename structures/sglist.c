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

void sglist_visit(SGList list, VisitFunction f) {
    for (GNode* node = list; node != NULL; node = node->next)
        f(node->data);
}

SGList sglist_insert(SGList list, void *data, CopyFunction copy, CompareFunction cmp) {
    GNode* head;
    if (list == NULL || cmp(data, list->data) < 0)
        head = glist_append_start(list, data, copy);
    else {
        GNode *node;
        for (node = list; list->next != NULL; list = list->next)
            if (cmp(data, list->next->data) >= 0)
                break;
        list->next = glist_append_start(list->next, data, copy);
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
            break;
    }
    return 0;
}

SGList sglist_from_array(void **arr, size_t len, CopyFunction copy, CompareFunction cmp) {
    SGList list = sglist_init();
    for (int i = 0; i < len; i++)
        list = sglist_insert(list, arr[i], copy, cmp);
    return list;
}
