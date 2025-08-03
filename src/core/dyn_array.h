#ifndef WL_DYN_ARRAY_H
#define WL_DYN_ARRAY_H

#include <stdlib.h>
#include <string.h>

// Define a proper struct for the dynamic array.
// This avoids the issues with the previous macro-based approach.
typedef struct DynArray {
    void* items;     // Pointer to the data
    int count;       // Current number of elements
    int capacity;    // Total capacity of the array
    size_t item_size; // Size of each element
} DynArray;

// Initial capacity for new dynamic arrays
#define DA_INIT_CAPACITY 8

// Function declarations for dynamic array operations
DynArray* da_create(size_t item_size);
void da_free(DynArray* arr);

void da_init(DynArray* arr, size_t item_size);
void da_clear(DynArray* arr);

void da_push(DynArray* arr, const void* item);
void* da_get(DynArray* arr, int index);
int da_count(const DynArray* arr);


#endif // WL_DYN_ARRAY_H
