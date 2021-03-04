#define CRT_SECURE_NO_WARNINGS

#include "vector.h"
#include "my_std.h"
#include <limits.h>

struct IntArr
{
    int* buffer;
    int size;
};

int vecDefaultElem = 0;
struct IntArr matrixDefaultElem;

int findMaxRow(struct VectorL* matrix)
{
    int resIndex = 0;
    long long maxVal = LLONG_MIN;
    for (unsigned int i = 0; i < matrix->size; i++)
    {
        int sum = 0;
        struct IntArr* curRow = (struct IntArr*)getElement(matrix, i);
        for (int j = 0; j < curRow->size; j++)
            sum += curRow->buffer[j];
        if (sum > maxVal)
            maxVal = sum,resIndex = i;
    }
    return resIndex;
}

void printArr(struct IntArr* arr)
{
    if (arr->size <= 0)
    {
        printf("\narray is empty\n");
        return;
    }
    printf("\n");
    for (int i = 0; i < arr->size; i++)
        printf(" %d ", arr->buffer[i]);
    printf("\n");
}

void clearRow(struct IntArr* arr)
{
    free(arr->buffer);
    free(arr);
}

void clearMatr(struct VectorL** matrix)
{
    struct IntArr* curRow = (struct IntArr*)getElement(*matrix, 0);
    clearRow(curRow);
    free(*matrix);
    *matrix = NULL;
}

void QuickSort(struct VectorL* matrix, unsigned int index)
{
    struct IntArr* curRow = (struct IntArr*)getElement(matrix, index);
    quickSort(curRow->buffer, 0, curRow->size - 1);
}

void printMatr(struct VectorL* matrix)
{
    printf("Исходная матрица: \n");
    for (unsigned int i = 0; i < matrix->size; i++)
        printArr((struct IntArr*)getElement(matrix, i));
}

void copyRow(struct VectorL* to, struct VectorL* from,int i)
{
    struct IntArr* elemFrom = (struct IntArr*)getElement(from, i);
    struct IntArr* elemTo = (struct IntArr*)getElement(to, i);
    elemTo->buffer = (int*)malloc(elemFrom->size * sizeof(int));
    memcpy(elemTo->buffer, elemFrom->buffer, sizeof(int) * elemFrom->size);
    elemTo->size = elemFrom->size;
}

void copyMatrix(struct VectorL* to, struct VectorL* from)
{
    to->size = from->size;
    to->defaultElem = from->defaultElem;
    to->elementSize = from->elementSize;
    for (unsigned int i = 0; i < from->size; i++)
        copyRow(to, from, i);
}

void FillRow(struct IntArr* arr)
{
    arr->buffer = (int*)malloc(arr->size * sizeof(int));
    printf("Введите элементы строки:\n");
    for (int j = 0; j < arr->size; j++)
    {
        int elem = vecDefaultElem;
        while (!getInt(&elem));
        arr->buffer[j] = elem;
    }
}

void RowCopy(struct IntArr* arr, unsigned int index, struct VectorL* matrix)
{
    struct IntArr* curRow = (struct IntArr*)getElement(matrix, index);
    arr->buffer = (int*)malloc(curRow->size * sizeof(int));
    memcpy(arr->buffer, curRow->buffer, curRow->size * sizeof(int));
    arr->size = curRow->size;
}

void FillMatrix(struct VectorL* matrix)
{
    for (unsigned int i = 0; i < matrix->size; i++)
    {
        printf("%s %d%s", "Введите кол-во столбцов в", i, "-ой строчке: ");
        int n = 0;
        while (!getInt(&n));
        struct IntArr* curRow = (struct IntArr*)getElement(matrix, i);
        curRow->size = n;
        FillRow(curRow);
    }
}

struct VectorL* CreateMatrix()
{
    printf("Введите кол-во строчек матрицы: ");
    int m = 0;
    while (!getInt(&m));
    struct VectorL* matrix = createVector(m, sizeof(struct IntArr), &matrixDefaultElem);
    return matrix;
}

struct IntArr* CreateMaxRowVec(struct VectorL* matrix)
{
    int index = findMaxRow(matrix);
    struct IntArr* vector = (struct IntArr*)malloc(sizeof(struct IntArr));
    RowCopy(vector, index, matrix);
    return vector;
}

struct IntArr* SortedArr(struct IntArr* arr)
{
    quickSort(arr->buffer, 0, arr->size - 1);
    return arr;
}

int main()
{
    int a = 1;
    char s[] = "Hello";
    SetRusLang();
    struct VectorL* IntVector = createVector(10, sizeof(int), &a);
    struct VectorL* StrVector = createVector(10, sizeof(char*), &s);
    printf("Полученный вектор: \n");
}