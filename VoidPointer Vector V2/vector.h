#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct VectorL;

struct VectorL* createVector(unsigned int size, unsigned int elementSize, void* defaultElem);
void* getElement(struct VectorL* vector, unsigned int index);
unsigned int getSize(struct VectorL* vector);
void changeElement(struct VectorL* vector, unsigned int index, void* newValue);
void insertElementAfterIndex(struct VectorL* vector, unsigned int index, void* Value);
void insertFirstElement(struct VectorL* vector, void* Value);
void insertLastElement(struct VectorL* vector, void* Value);
void removeElement(struct VectorL* vector, unsigned int index);
void resize(struct VectorL* vector, unsigned int newSize);
void clear(struct VectorL* vector);

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
    for (unsigned int i = 0; i < size; ++i) 
        changeElement(newVector, i, defaultElem);
    return newVector;
}

void* getElement(struct VectorL* vector, unsigned int index)
{
    return (char*)vector->pointerFirst + vector->elementSize * index;
}

unsigned int getSize(struct VectorL* vector)
{
    return vector->size;
}

void changeElement(struct VectorL* vector, unsigned int index, void* newValue)
{
    memcpy(getElement(vector, index), newValue, vector->elementSize);
}

void insertElementAfterIndex(struct VectorL* vector, unsigned int index, void* Value)
{
    void* newPointer = malloc((vector->size + 1) * vector->elementSize);
    void* to = newPointer;
    unsigned int i;
    for (i = 0; i <= index; ++i)
    {
        memcpy(to, getElement(vector, i), vector->elementSize);
        to = (char*)to + vector->elementSize;
    }
    memcpy(to, Value, vector->elementSize);
    for (to = (char*)to + vector->elementSize, i = index + 1; i < vector->size; ++i)
    {
        memcpy(to, getElement(vector, i), vector->elementSize);
        to = (char*)to + vector->elementSize;
    }
    vector->size++;
    vector->pointerFirst = newPointer;
}
void insertFirstElement(struct VectorL* vector, void* Value)
{
    void* newPointer = malloc((vector->size + 1) * vector->elementSize);
    void* to = newPointer;
    memcpy(to, Value, vector->elementSize);
    unsigned int i;
    for (to = (char*)to + vector->elementSize, i = 0; i < vector->size; i++)
    {
        memcpy(to, getElement(vector, i), vector->elementSize);
        to = (char*)to + vector->elementSize;
    }
    vector->pointerFirst = newPointer;
    vector->size++;
}
void insertLastElement(struct VectorL* vector, void* Value)
{
    void* newPointer = malloc((vector->size + 1) * vector->elementSize);
    void* to = newPointer;
    for (unsigned int i = 0; i < vector->size; ++i)
    {
        memcpy(to, getElement(vector, i), vector->elementSize);
        to = (char*)to + vector->elementSize;
    }
    memcpy(to, Value, vector->elementSize);
    vector->pointerFirst = newPointer;
    vector->size++;
}
void removeElement(struct VectorL* vector, unsigned int index)
{
    void* newPointer = malloc((vector->size - 1) * vector->elementSize);
    void* to = newPointer;
    for (unsigned int i = 0; i < index; ++i)
    {
        memcpy(to, getElement(vector, i), vector->elementSize);
        to = (char*)to + vector->elementSize;
    }
    for (unsigned int i = index + 1; i < vector->size; ++i)
    {
        memcpy(to, getElement(vector, i), vector->elementSize);
        to = (char*)to + vector->elementSize;
    }
    vector->pointerFirst = newPointer;
    vector->size++;
}
void resize(struct VectorL* vector, unsigned int newSize)
{
    if (newSize > vector->size)
    {
        vector->pointerFirst = realloc(vector->pointerFirst, newSize * vector->elementSize);
        for (unsigned int i = vector->size; i < newSize; i++)
            changeElement(vector, i, vector->defaultElem);
    }
    else if (newSize < vector->size)
        vector->pointerFirst = realloc(vector->pointerFirst, newSize * vector->elementSize);
    vector->size = newSize;
}
void clear(struct VectorL* vector)
{
    for (unsigned int i = 0; i < vector->size; i++)
        changeElement(vector, i, vector->defaultElem);
}