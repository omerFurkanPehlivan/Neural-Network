#include "../include/list.h"
#include "../lib/macro_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//* STRUCT DEFINITION *********************************************************

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct ListStruct {
    Node* head, * tail;
    size_t size;
} ListStruct;

//* FUNCTION PROTOTYPES *******************************************************

List create();
void destroy_node(Node* node);
void destroy(List* listAddr);
int push(List list, void* data);
int pop(List list, void** data);
int peek(List list, void** data);
size_t size(List list);
int isEmpty(List list);
int next(Node* node);

//* INTERFACE INITIALIZATION **************************************************

const struct ListInterface ListOps = {
    .create = create,
    .destroy = destroy,
    .push = push,
    .pop = pop,
    .peek = peek,
    .size = size,
    .isEmpty = isEmpty
};

//* FUNCTION DEFINITIONS ******************************************************

List create()
{
    List list;

    list = malloc(sizeof(ListStruct));
    if (list == NULL) {
        MAL_ERR();
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void destroy_node(Node* node)
{
    if (node == NULL) {
        return;
    }

    destroy_node(node->next);
    free(node);
}

void destroy(List* listAddr)
{
    Node* node;
    Node* next;
    List list;

    if (listAddr == NULL) {
        return;
    }

    list = *listAddr;
    if (list) {
        destroy_node(list->head);
        free(list);
    }

    *listAddr = NULL;
}

int push(List list, void* data)
{
    Node* node;
    Node* current;

    if (list == NULL) {
        PRINT_ERR("NULL pointer exception!");
        return -1;
    }

    node = malloc(sizeof(Node));
    if (node == NULL) {
        MAL_ERR();
        return -1;
    }

    node->data = data;
    node->next = NULL;

    if (isEmpty(list)) {
        list->head = node;
        list->tail = node;
    }
    else {
        current = list->tail;
        current->next = node;
        list->tail = node;
    }

    list->size++;

    return 0;
}

int pop(List list, void** data)
{
    Node* node;
    Node* prev;

    if (list == NULL) {
        PRINT_ERR("NULL pointer exception!");
        return -1;
    }

    if (isEmpty(list)) {
        return -1;
    }

    node = list->head;
    prev = NULL;

    while (node->next != NULL) {
        prev = node;
        node = node->next;
    }

    if (prev == NULL) {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        prev->next = NULL;
        list->tail = prev;
    }

    *data = node->data;
    free(node);

    list->size--;

    return 0;
}

int peek(List list, void** data)
{
    Node* node;

    if (list == NULL) {
        PRINT_ERR("NULL pointer exception!");
        return -1;
    }

    if (isEmpty(list)) {
        return -1;
    }

    node = list->tail;
    *data = node->data;

    return 0;
}

size_t size(List list)
{
    if (list == NULL) {
        PRINT_ERR("NULL pointer exception!");
        return 0;
    }

    return list->size;
}

int isEmpty(List list)
{
    if (list == NULL) {
        PRINT_ERR("NULL pointer exception!");
        return -1;
    }

    return list->size == 0;
}
