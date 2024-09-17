#pragma once

// a thought for collection of auto destroyable objects. 

#define AUTO_DESTROYABLE(TYPE) \
struct TYPE##AutoDestroyable { \
    TYPE##AutoDestroy (*new_tag)(); \
    TYPE (*add)(AutoDestroyable ad, TYPE object); \
    void (*destroyAll)(AutoDestroyable adAddr); \
} TYPE##AutoDestroyable;
typedef struct AutoDestroyableStruct* AutoDestroyable;