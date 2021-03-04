#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef int ListElemType;

struct List;
struct Node;

struct List* createList();

struct Node* insertAfterIndex(struct List* list, int index, ListElemType value);
struct Node* insertFirst(struct List* list, ListElemType value);
struct Node* insertLast(struct List* list, ListElemType value);

void deleteElement(struct List* list, int index); /// delete
void deleteFirst(struct List* list);
void deleteLast(struct List* list);

struct Node* getElement(struct List* list, int index);
struct Node* changeElement(struct List* list, int index, ListElemType newNal);

int getSize(struct List* list);

void clear(struct List* list);

void print(struct List* list);

void QuickSortList(struct Node* pLeft, struct Node* pRight);

struct List
{
    struct Node* pFirst;
    struct Node* pLast;
    int size;
};

struct Node
{
    ListElemType value;
    struct Node* pNext;
    struct Node* pPrev;
};

struct List* createList()
{
    struct List* newList = (struct List*) malloc(sizeof(struct List) * 1);
    if (newList == NULL)
        return NULL;
    newList->pFirst = NULL;
    newList->pLast = NULL;
    newList->size = 0;
    return newList;
};

struct Node* insertAfterIndex(struct List* list, int index, ListElemType value)
{
    struct Node* newNode = NULL;
    if (index >= list->size || index < 0)
        return NULL;
    if (index == list->size - 1)
    {
        newNode = insertLast(list, value);
        return newNode;
    }
    struct Node* node = list->pFirst;
    for (int i = 0; i != index; i++)
        node = node->pNext;
    newNode = (struct Node*) malloc(sizeof(struct Node) * 1);
    if (newNode == NULL)
        return NULL;
    newNode->value = value;
    newNode->pPrev = node;
    newNode->pNext = node->pNext;
    node->pNext->pPrev = newNode;
    node->pNext = newNode;
    list->size++;
    return newNode;
}

struct Node* insertFirst(struct List* list, ListElemType value)
{
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node) * 1);
    if (newNode == NULL)
        return NULL;
    newNode->value = value;
    if (list->size == 0)
    {
        newNode->pPrev = NULL;
        newNode->pNext = NULL;
        list->pFirst = newNode;
        list->pLast = newNode;
    }
    else
    {
        newNode->pNext = list->pFirst;
        newNode->pPrev = NULL;
        list->pFirst->pPrev = newNode;
        list->pFirst = newNode;
    }
    list->size++;
    return newNode;
}

struct Node* insertLast(struct List* list, ListElemType value)
{
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node) * 1);
    if (newNode == NULL)
        return NULL;
    newNode->value = value;
    if (list->size == 0)
    {
        newNode->pPrev = NULL;
        newNode->pNext = NULL;
        list->pFirst = newNode;
        list->pLast = newNode;
    }
    else
    {
        newNode->pNext = NULL;
        newNode->pPrev = list->pLast;
        list->pLast->pNext = newNode;
        list->pLast = newNode;
    }
    list->size++;
    return newNode;
}

struct Node* getElement(struct List* list, int index)
{
    if (index >= list->size || index < 0)
        return NULL;
    if (index >= list->size / 2)
    {
        struct Node* node = list->pLast;
        for (int i = list->size - 1; i > index; i--)
            node = node->pPrev;
        return node;
    }
    else
    {
        struct Node* node = list->pFirst;
        for (int i = 0; i < index; i++)
            node = node->pNext;
        return node;
    }
    return NULL;
}

struct Node* changeElement(struct List* list, int index, ListElemType newVal)
{
    if (index < 0 || index >= list->size)
        return NULL;
    struct Node* node = list->pFirst;
    for (int i = 0; i < index; i++)
        node = node->pNext;
    node->value = newVal;
    return node;
};

void deleteElement(struct List* list, int index)
{
    if (index < 0 || index >= list->size)
        return;
    if (index == 0)
    {
        deleteFirst(list);
        return;
    }
    if (index == list->size - 1)
    {
        deleteLast(list);
        return;
    }
    struct Node* node = list->pFirst;
    for (int i = 0; i < index; i++)
        node = node->pNext;
    node->pPrev->pNext = node->pNext;
    node->pNext->pPrev = node->pPrev;
    list->size--;
    free(node);
}

void deleteFirst(struct List* list)
{
    if (list->size == 0)
        return;
    if (list->size == 1)
    {
        struct Node* node = list->pFirst;
        list->pFirst = NULL;
        list->pLast = NULL;
        list->size = 0;
        free(node);
        return;
    }
    struct Node* node = list->pFirst;
    list->pFirst = node->pNext;
    list->pFirst->pPrev = NULL;
    list->size--;
    free(node);
}

void deleteLast(struct List* list)
{
    if (list->size == 0 || list->size == 1)
    {
        deleteFirst(list);
        return;
    }
    struct Node* node = list->pLast;
    list->pLast = node->pPrev;
    list->pLast->pNext = NULL;
    list->size--;
    free(node);
}

int getSize(struct List* list)
{
    return list->size;
}

void clear(struct List* list)
{
    struct Node* node = list->pFirst;
    for (int i = 0; i < list->size; i++)
    {
        struct Node* nextNode = node;
        free(node);
        node = nextNode;
    }
    list->size = 0;
    list->pFirst = NULL;
    list->pLast = NULL;
}

void print(struct List* list)
{
    struct Node* ptr = list->pFirst;
    while (ptr != nullptr)
    {
        printf("%d ", ptr->value);
        ptr = ptr->pNext;
    }
    printf("\n");
}

void QuickSortList(struct Node* pLeft, struct Node* pRight)
{
    struct Node* pStart;
    struct Node* pCurrent;
    int nCopyInteger;

    // сортировка окончена - выход
    if (pLeft == pRight) return;

    // установка двух рабочих указателей - Start и Current
    pStart = pLeft;
    pCurrent = pStart->pNext;

    // итерация по списку слева направо
    while (1)
    {
        // элемент с максимальным значением помещается в начало списка
        if (pStart->value < pCurrent->value)
        {
            nCopyInteger = pCurrent->value;
            pCurrent->value = pStart->value;
            pStart->value = nCopyInteger;
        }

        if (pCurrent == pRight) break;
        pCurrent = pCurrent->pNext;
    }

    // переключение First и Current - максимум попадает в правый конец списка
    nCopyInteger = pLeft->value;
    pLeft->value = pCurrent->value;
    pCurrent->value = nCopyInteger;

    // сохранение Current
    struct Node* pOldCurrent = pCurrent;

    // рекурсия
    pCurrent = pCurrent->pPrev;
    if (pCurrent != NULL)
    {
        if ((pLeft->pPrev != pCurrent) && (pCurrent->pNext != pLeft))
            QuickSortList(pLeft, pCurrent);
    }

    pCurrent = pOldCurrent;
    pCurrent = pCurrent->pNext;
    if (pCurrent != NULL)
    {
        if ((pCurrent->pPrev != pRight) && (pRight->pNext != pCurrent))
            QuickSortList(pCurrent, pRight);
    }
}