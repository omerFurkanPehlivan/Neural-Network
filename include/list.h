#pragma once

#include <stddef.h>

typedef struct ListStruct* List;
typedef struct Node* Node;

extern const struct ListInterface{
    List (*create)();
    void (*destroy)(List* listAddr);
    Node (*head)(List list);
    int (*push)(List list, void* data);
    int (*pop)(List list, void** data);
    int (*peek)(List list, void** data);
    size_t (*size)(List list);
    int (*isEmpty)(List list);
} ListOps;

extern const struct NodeInterface{
    int (*next)(Node* node);
    int (*get)(Node node, void** data);
} NodeOps;