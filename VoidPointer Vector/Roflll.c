#define _CRT_SECURE_NO_WARNINGS
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

struct IntArr
{
    int* buffer;
    unsigned int size;
};

void FillMatrix(struct VectorL* matrix)
{
    int n;
    scanf("%d", &n);
    struct IntArr* curRaw = (struct IntArr*)getElement(matrix, 2);
    curRaw->buffer = (int*)malloc(n * sizeof(int));
    curRaw->size = n;
    printf("\n%d\n", matrix->elementSize);
    printf("\n%p\n", curRaw);
    curRaw = (struct IntArr*)getElement(matrix, 3);
    printf("\n%p\n", curRaw);
    curRaw->buffer = (int*)malloc(n * sizeof(int));
    curRaw->size = n;
    /*for (int i = 0; i < matrix->size; i++)
    {
        printf("%s %d%s", "Введите кол-во столбцов в", i, "-ой строчке: ");
        int n;
        scanf("%d", &n);
        struct IntArr* curRaw = (struct IntArr*)getElement(matrix, i);
        curRaw->buffer = (int*)malloc(n * sizeof(int));
        curRaw->size = n;
        /*for (int j = 0; j < n; j++)
        {
            printf("Введите элемент строки: ");
            int elem = 0;
            scanf("%d", &n);
            curRaw->buffer[j] = elem;
        }
    }*/
}

int main()
{
    system("chcp 1251");
    system("cls");
    struct IntArr defaultElem;
    struct VectorL* matrix = createVector(5,sizeof(struct IntArr),&defaultElem);
    FillMatrix(matrix);
    struct IntArr* g = (struct IntArr*)getElement(matrix, 2);
    printf("%d", g->size);
}

