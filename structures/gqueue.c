#include "gqueue.h"

GQueue gqueue_init() {
    GQueue q = malloc(sizeof(struct _GQueue));
    assert(q != NULL);
    q->last = q->first = NULL;
    return q;
}

void gqueue_free(GQueue q, DestroyFunction destroy) {
    
    while (!gqueue_empty(q)) {
        GNode *tmp = q->first;
        q->first = q->first->next;
        destroy(tmp->data);
        free(tmp);
    }
    free(q);
}

int gqueue_empty(GQueue q) { return q->first == NULL; }

void* gqueue_start(GQueue q, CopyFunction copy) { return copy(q->first->data); }

void gqueue_push(GQueue q, void *data, CopyFunction copy) {
    GNode *new_node = malloc(sizeof(GNode));
    assert(new_node != NULL);
    new_node->data = copy(data);
    new_node->next = NULL;
    
    q->last->next = new_node;
    q->last = new_node;
    if (q->first == NULL)
        q->first = new_node;
}

void gqueue_pop(GQueue q, DestroyFunction destroy) {
    if (gqueue_empty(q)) return ;
    GNode *node_to_remove = q->first;
    
    q->first = q->first->next;
    if (q->first == NULL)
        q->last = NULL;
    
    destroy(node_to_remove->data);
    free(node_to_remove);
}

void gqueue_print(GQueue q, VisitFunction print) {
    for (GNode *tmp = q->first; tmp != NULL; tmp = tmp->next)
        print(tmp->data);
}
