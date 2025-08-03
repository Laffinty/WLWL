#include "dyn_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Creates and allocates a new DynArray on the heap.
DynArray* da_create(size_t item_size) {
    DynArray* arr = (DynArray*)malloc(sizeof(DynArray));
    if (!arr) {
        // Handle allocation failure
        return NULL;
    }
    da_init(arr, item_size);
    return arr;
}

// Frees a DynArray that was created with da_create.
void da_free(DynArray* arr) {
    if (arr) {
        da_clear(arr);
        free(arr);
    }
}

// Initializes a DynArray struct (that is not a pointer).
void da_init(DynArray* arr, size_t item_size) {
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
    arr->item_size = item_size;
}

// Clears the contents of a DynArray, freeing the internal buffer.
void da_clear(DynArray* arr) {
    if (arr && arr->items) {
        free(arr->items);
        arr->items = NULL;
        arr->count = 0;
        arr->capacity = 0;
    }
}

// Internal function to resize the array when it's full.
static void da_resize(DynArray* arr) {
    if (arr->count >= arr->capacity) {
        arr->capacity = (arr->capacity == 0) ? DA_INIT_CAPACITY : arr->capacity * 2;
        void* new_items = realloc(arr->items, arr->capacity * arr->item_size);
        if (!new_items) {
            // Handle realloc failure, this is a critical error
            fprintf(stderr, "Failed to resize dynamic array\n");
            exit(1);
        }
        arr->items = new_items;
    }
}

// Pushes a new item onto the end of the array.
void da_push(DynArray* arr, const void* item) {
    da_resize(arr);
    // Copy the item into the array
    memcpy((char*)arr->items + arr->count * arr->item_size, item, arr->item_size);
    arr->count++;
}

// Gets a pointer to the item at a specific index.
void* da_get(DynArray* arr, int index) {
    if (index < 0 || index >= arr->count) {
        return NULL; // Out of bounds
    }
    return (char*)arr->items + index * arr->item_size;
}

// Returns the number of items in the array.
int da_count(const DynArray* arr) {
    return arr->count;
}
