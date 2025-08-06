#include "dyn_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DynArray* da_create(size_t item_size) {
    DynArray* arr = (DynArray*)malloc(sizeof(DynArray));
    if (!arr) {
        return NULL;
    }
    da_init(arr, item_size);
    return arr;
}

void da_free(DynArray* arr) {
    if (arr) {
        da_clear(arr);
        free(arr);
    }
}

void da_init(DynArray* arr, size_t item_size) {
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
    arr->item_size = item_size;
}

void da_clear(DynArray* arr) {
    if (arr && arr->items) {
        free(arr->items);
        arr->items = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

static void da_resize(DynArray* arr) {
    if (arr->count >= arr->capacity) {
        arr->capacity = (arr->capacity == 0) ? DA_INIT_CAPACITY : arr->capacity * 2;
        void* new_items = realloc(arr->items, arr->capacity * arr->item_size);
        if (!new_items) {
            fprintf(stderr, "Failed to resize dynamic array\n");
            exit(1);
        }
        arr->items = new_items;
    }
}

void da_push(DynArray* arr, const void* item) {
    da_resize(arr);
    memcpy((char*)arr->items + arr->count * arr->item_size, item, arr->item_size);
    arr->count++;
}

void* da_get(DynArray* arr, int index) {
    if (index < 0 || index >= arr->count) {
        return NULL;
    }
    return (char*)arr->items + index * arr->item_size;
}

int da_count(const DynArray* arr) {
    return arr->count;
}