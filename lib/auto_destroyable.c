#include "auto_destroyable.h"
#include "../include/list.h"
#include "macro_error.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//* STRUCT DEFINITION *********************************************************

typedef struct AutoDestroyableStruct {
    List list;
    void (*destroy)(void* data);
} AutoDestroyableStruct;

//* FUNCTION PROTOTYPES *******************************************************

AutoDestroyable create();
void destroy(AutoDestroyable* adAddr);
void destroyAll(AutoDestroyable* adAddr);
void add(AutoDestroyable ad);

//* INTERFACE INITIALIZATION **************************************************

const struct AutoDestroyableInterface AutoDestroyableOps = {
    .create = create,
    .destroy = destroy,
    .destroyAll = destroyAll,
    .add = add
};

//* FUNCTION DEFINITIONS ******************************************************

AutoDestroyable create()
{
    AutoDestroyable ad;

    ad = malloc(sizeof(AutoDestroyableStruct));
    if (ad == NULL) {
        MAL_ERR();
        return NULL;
    }

    ad->list = ListOps.create();
    if (ad->list == NULL) {
        free(ad);
        return NULL;
    }

    return ad;
}

void destroy(AutoDestroyable* adAddr)
{
    AutoDestroyable ad;

    if (adAddr == NULL) {
        return;
    }

    ad = *adAddr;
    if (ad) {
        destroyAll(ad);
        ListOps.destroy(&ad->list);
        free(ad);
    }

    *adAddr = NULL;
}

void destroyAll()
{
    AutoDestroyable ad;
    void* data;

    while (!ListOps.isEmpty(ad->list)) {
        ListOps.pop(ad->list, &data);
        ad->destroy(data);
    }
}

void add(AutoDestroyable ad)
{
    if (ad == NULL) {
        PRINT_ERR("NULL pointer exception!");
        return;
    }

    ListOps.push(ad->list, ad);
}
