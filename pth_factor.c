#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int* data; //   Points to the dynamic array
    size_t size; // Helps us to track the size
    size_t capacity; // Helps us to monitor the capacity of our array
} Vector;

//  Create vector
Vector* create_vector() {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 1;
    return vector;
}

//  Checks if the value v exists in the vector.
bool exists(Vector* vector, int v) {
    for (int i = 0; i < vector->size; i++) {
        if (vector->data[i] == v) {
            return true;
        }
    }

    return false;
}

/**
 * An simple implementation of push_back.
 * Features:
 * 1. Initialize vector if it's NULL
 * 2. When we reach capacity, we double it.
 * 3. It keeps track of size increase
 */
void push(Vector* vector, int value) {
    if (vector->data == NULL) {
        vector->data = (int*)malloc(sizeof(int));
        vector->capacity = 1;
    } else if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->data = (int*)realloc(vector->data, vector->capacity * sizeof(int));
    }

    vector->data[vector->size] = value;
    vector->size++;
}

/**
 * Given an index, get the value inside the array
 */
int findAtIndex(Vector *vector, size_t i) {
    if (i > vector->size) {
        fprintf(stderr, "Index out of bounds.\n");
        exit(1);
    }

    return vector->data[i];
}

/**
 * Compare algorithm reference: https://cplusplus.com/reference/cstdlib/qsort/
 */
int compare(const void *a, const void *b) {
    return ( *(int*) a - *(int*)b );
}

/**
 * Algorithm:
 * Given a number n, the facts are:
 *  Max factor is n/2 into maxFactor
 * 
 * initialize vector factors to hold all the factors
 * for i in range maxFactor
 *      if n % i == 0
 *          factors.push(i, n/i)
 * 
 * sort factors
 * return factors[p]
 */
long pthFactor(long n, long p) {
    long maxFactor = n / 2;

    Vector* vector = create_vector();
    for (int i = 1; i < maxFactor; i++) {
        if (n % i == 0) {
            if (exists(vector, i)) {
                break;
            }

            push(vector, i);
            push(vector, n/i);
        }
    }

    qsort(vector->data, vector->size, sizeof(int), compare);
    return findAtIndex(vector, p - 1);
}
