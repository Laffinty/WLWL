#ifndef WL_DYN_ARRAY_H
#define WL_DYN_ARRAY_H

#include <stdlib.h>
#include <string.h>

typedef struct DynArray {
    void* items;
    int count;
    int capacity;
    size_t item_size;
} DynArray;

#define DA_INIT_CAPACITY 8

DynArray* da_create(size_t item_size);
void da_free(DynArray* arr);

void da_init(DynArray* arr, size_t item_size);
void da_clear(DynArray* arr);

void da_push(DynArray* arr, const void* item);
void* da_get(DynArray* arr, int index);
int da_count(const DynArray* arr);

#endif