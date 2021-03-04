#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct VectorL;

struct VectorL* createVector(unsigned int size, unsigned int elementSize, void* defaultElem);
void* getElement(struct VectorL* vector, unsigned int index);
void changeElement(struct VectorL* vector, unsigned int index, void* newValue);

struct VectorL
{
    void* pointerFirst;
    unsigned int size;
    unsigned int elementSize;
    void* defaultElem;
};

struct VectorL* createVector(unsigned int size, unsigned int elementSize, void* defaultElem)
{
    struct VectorL* newVector = (struct VectorL*) malloc(sizeof(struct VectorL));
    newVector->pointerFirst = malloc(size * elementSize);
    newVector->size = size;
    newVector->defaultElem = defaultElem;
    newVector->elementSize = elementSize;
    for (unsigned int i = 0; i < size; ++i) {
        changeElement(newVector, i, defaultElem);
    }
    return newVector;
}

void* getElement(struct VectorL* vector, unsigned int index)
{
    void* a = (char*)vector->pointerFirst + vector->elementSize * index;
    return a;
}

void changeElement(struct VectorL* vector, unsigned int index, void* newValue)
{
    void* to = getElement(vector, index);
    memcpy(to, newValue, vector->elementSize);
}