#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <windows.h>

#define BACK_SPACE 8
#define ENTER 13
#define ESC 27
#define UP 72   // 224 72
#define DOWN 80 // 224 80
#define LEFT 75 // 224 75
#define RIGHT 77// 224 77

#define getch _getch


struct List;
struct Node;
struct Iterator;

struct List
{
    struct Node* pFirst;
    struct Node* pLast;
    int size;
};

struct Node
{
    int value;
    struct Node* pNext;
    struct Node* pPrev;
};

struct Iterator
{
    struct Node* node;
    struct List* list;
};

struct List* createList();

struct Node* insertAfterIndex(struct List* list, int index, int value);
struct Node* insertFront(struct List* list, int value);
struct Node* insertBack(struct List* list, int value);

void eraseElement(struct List* list, int index); /// delete
void eraseFront(struct List* list);
void eraseBack(struct List* list);

struct Node* getElement(struct List* list, int index);
struct Node* changeElement(struct List* list, int index, int newNal);

int getSize(struct List* list);
void clear(struct List* list);
void print(struct List* list);

struct Iterator* createFrontIterator(struct List* list);
struct Iterator* createBackIterator(struct List* list);
struct Iterator* relocateIterator(struct Iterator* it, int relocateOn);
struct Iterator* erase(struct Iterator* it);
void changeElementIterator(struct Iterator* it, int newNal);
struct Iterator* insertAfterIterator(struct Iterator* it, int value);

void rebuildList(struct Iterator* it);

int getNumOfNode(struct Iterator* it);

struct Iterator* insertChainAfterIterator(struct Iterator* it, char* elements);
struct Iterator* eraseChain(struct List* list, int firstIndex, int lastIndex);


int strToNum(const char* str);

struct List* createList()
{
    /// сдедать проверку на null
    struct List* newList = (struct List*) malloc(sizeof(struct List) * 1);
    if (newList == NULL)
        return NULL;

    newList->pFirst = NULL;
    newList->pLast = NULL;
    newList->size = 0;
    return newList;
};

struct Node* insertAfterIndex(struct List* list, int index, int value)
{
    struct Node* newNode = NULL;
    if (index >= list->size || index < 0)
        return NULL;

    if (index == list->size - 1)
    {
        newNode = insertBack(list, value);
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

struct Node* insertFront(struct List* list, int value)
{
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node) * 1);
    if (newNode == NULL)
        return NULL;

    newNode->value = value;

    /// create first node
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

struct Node* insertBack(struct List* list, int value)
{
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node) * 1);
    if (newNode == NULL)
        return NULL;

    newNode->value = value;

    /// create first node
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
        /// движемся с конца в начало
        struct Node* node = list->pLast;
        for (int i = list->size - 1; i > index; i--)
            node = node->pPrev;

        return node;
    }
    else
    {
        /// движемся из начала в конец
        struct Node* node = list->pFirst;

        for (int i = 0; i < index; i++)
            node = node->pNext;

        return node;
    }
    return NULL;
}

struct Node* changeElement(struct List* list, int index, int newVal)
{
    if (index < 0 || index >= list->size)
        return NULL;

    struct Node* node = list->pFirst;
    for (int i = 0; i < index; i++)
        node = node->pNext;

    node->value = newVal;
    return node;
};

void eraseElement(struct List* list, int index)
{
    if (index < 0 || index >= list->size)
        return;

    if (index == 0)
    {
        eraseFront(list);
        return;
    }

    if (index == list->size - 1)
    {
        eraseBack(list);
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

void eraseFront(struct List* list)
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

void eraseBack(struct List* list)
{
    if (list->size == 0 || list->size == 1)
    {
        eraseFront(list);
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
    if (list->size == 0)
    {
        printf("\n");
        return;
    }

    struct Node* node = list->pFirst;
    for (int i = 0; i < list->size; i++)
    {
        printf("%5d|", node->value);
        node = node->pNext;
    }
    printf("\n");
}

struct Iterator* createFrontIterator(struct List* list)
{
    if (list == NULL)
        return 0;

    if (list->size == 0)
        return NULL;

    struct Iterator* it = (struct Iterator*)malloc(sizeof(struct Iterator));
    if (it == NULL)
        return NULL;

    it->node = list->pFirst;
    it->list = list;
    return it;
};

struct Iterator* createBackIterator(struct List* list)
{
    if (list == NULL)
        return 0;

    if (list->size == 0)
        return NULL;

    struct Iterator* it = (struct Iterator*)malloc(sizeof(struct Iterator));
    if (it == NULL)
        return NULL;

    it->node = list->pLast;
    it->list = list;
    return it;
};

struct Iterator* relocateIterator(struct Iterator* it, int relocateOn)
{
    if (it == NULL)
        return NULL;

    if (relocateOn == 0)
        return it;

    if (relocateOn > 0)
    {
        for (int i = 0; i < relocateOn; i++)
        {
            if (it->node->pNext == NULL)
            {
                it->node = it->list->pFirst;
            }
            else
            {
                it->node = it->node->pNext;
            }
        }
    }
    else
    {
        for (int i = relocateOn; i < 0; i++)
        {
            if (it->node->pPrev == NULL)
            {
                it->node = it->list->pLast;
            }
            else
            {
                it->node = it->node->pPrev;
            }
        }
    }
    return it;
};

struct Iterator* erase(struct Iterator* it)
{
    if (it->list->pFirst == NULL)
        return NULL;

    if (it->node->pNext == NULL)
    {
        eraseBack(it->list);
        it->node = it->list->pLast;
        return it;
    }
    if (it->node->pPrev == NULL)
    {
        eraseFront(it->list);
        it->node = it->list->pFirst;
        return it;
    }

    it->node->pPrev->pNext = it->node->pNext;
    it->node->pNext->pPrev = it->node->pPrev;
    struct Node* deleteNode = it->node;
    it->node = it->node->pPrev;
    free(deleteNode);
    it->list->size--;
}

void changeElementIterator(struct Iterator* it, int newNal)
{
    it->node->value = newNal;
}

struct Iterator* insertAfterIterator(struct Iterator* it, int value)
{
    if (it->node->pNext == NULL)
    {
        insertBack(it->list, value);
        it->node = it->list->pLast;
        return it;
    }

    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->value = value;

    n->pPrev = it->node;
    n->pNext = it->node->pNext;
    it->node->pNext->pPrev = n;
    it->node->pNext = n;
    it->node = n;
    it->list->size++;
    return it;
};

void rebuildList(struct Iterator* it)
{
    it->list->pLast->pNext = it->list->pFirst;
    it->list->pFirst->pPrev = it->list->pLast;

    it->list->pFirst = it->node;
    it->list->pLast = it->node->pPrev;

    it->node->pPrev->pNext = NULL;
    it->node->pPrev = NULL;
}

int getNumOfNode(struct Iterator* it)
{
    int index = 0;
    for (struct Node* n = it->list->pFirst; n != it->node; n = n->pNext)
        index++;

    return index;
}

struct Iterator* insertChainAfterIterator(struct Iterator* it, char* elements)
{
    int startOfNumIndex = 0;

    int j = 0;
    for (; elements[j] != '\0'; j++)
        ;

    for (; elements[j] == ' ' || elements[j] == '\0'; j--)
        elements[j] = '\0';

    j++;
    elements[j] = ' ';

    for (int i = 0; elements[i] != '\0'; i++)
    {
        if (elements[i] == ' ')
        {
            elements[i] = '\0';
            int newNumber = strToNum(elements + startOfNumIndex);
            startOfNumIndex = i + 1;

            if (it->list->size == 0)
                it->node = insertFront(it->list, newNumber);
            else
                insertAfterIterator(it, newNumber);
        }
    }

    return it;
}

struct Iterator* eraseChain(struct List* list, int firstIndex, int lastIndex)
{
    struct Iterator* it = createFrontIterator(list);
    relocateIterator(it, lastIndex);

    for (int i = lastIndex; i >= firstIndex; i--)
        if (NULL == erase(it))
            break;

    return it;
};


const int SIZE_OF_MENU = 9;

int strToNum(const char* str)
{
    int result = 0;
    for (int i = 0; str[i]; i++)
        if (str[i] >= '0' && str[i] <= '9')
        {
            result *= 10;
            result += str[i] - '0';
        }
    if (str[0] == '-')
        result *= -1;

    return result;
}

const char* numToStr(int num)
{
    char* str = (char*)malloc(sizeof(char) * 100);
    for (int i = 0; i < 100; i++)
        str[i] = 0;

    int sign = 0;
    if (num < 0)
    {
        sign = 1;
        num *= -1;
    }

    while (num)
    {
        for (int i = 100 - 1; i > 0; i--)
            str[i] = str[i - 1];

        str[0] = num % 10 + '0';
        num /= 10;
    }
    if (sign)
    {
        for (int i = 100 - 1; i > 0; i--)
            str[i] = str[i - 1];

        str[0] = '-';
    }

    if (str[0] == 0)
        str[0] = '0';
    return str;
}

int getItEnd(const char* str)
{
    int it = 0;
    for (; str[it]; it++)
        ;

    return it;
}

int getFirstSizeWithGetch(const char* message)
{
    unsigned char byteFirst = -1;
    unsigned char byteSecond = -1;
    char nStr[100] = { 0 };
    int it = -1;
    for (;;)
    {
        system("cls");
        printf(message);
        printf(nStr);

        byteFirst = getch();
        if (byteFirst == 224 || byteFirst == 0)
            byteSecond = getch();

        if (byteFirst >= '0' && byteFirst <= '9')
        {
            it++;
            nStr[it] = byteFirst;
        }
        if (byteFirst == BACK_SPACE)
        {
            nStr[it] = 0;
            if (it != -1)
                it--;
        }
        if (byteFirst == ENTER)
        {
            int flag = 1;
            for (int i = 0; nStr[i]; i++)
                if (nStr[i] != '0')
                    flag = 0;

            if (flag == 0)
                break;
        }
    }
    return strToNum(nStr);
}

int getNumberWithGetch(const char* message)
{
    unsigned char byteFirst = -1;
    unsigned char byteSecond = -1;
    char nStr[100] = { 0 };
    int it = -1;
    for (;;)
    {
        system("cls");
        printf(message);
        printf(nStr);

        byteFirst = getch();
        if (byteFirst == 224 || byteFirst == 0)
            byteSecond = getch();

        if (byteFirst >= '0' && byteFirst <= '9' || it == -1 && byteFirst == '-')
        {
            it++;
            nStr[it] = byteFirst;
        }
        if (byteFirst == BACK_SPACE)
        {
            nStr[it] = 0;
            if (it != -1)
                it--;
        }
        if (byteFirst == ENTER)
        {
            if (nStr[0] != 0)
                break;
        }
    }
    return strToNum(nStr);
}

char* getChainStrWithGetch(const char* message)
{
    unsigned char byteFirst = -1;
    unsigned char byteSecond = -1;

    char* nStr = (char*)malloc(sizeof(char) * 500);
    for (int i = 0; i < 500; i++)
        nStr[i] = 0;

    int it = -1;

    for (;;)
    {
        system("cls");
        printf(message);
        printf(nStr);

        byteFirst = getch();
        if (byteFirst == 224 || byteFirst == 0)
            byteSecond = getch();

        if (byteFirst >= '0' && byteFirst <= '9' ||
            (it == -1 || nStr[it] == ' ') && byteFirst == '-' ||
            (it != -1 && nStr[it] != ' ') && byteFirst == ' ')
        {
            it++;
            nStr[it] = byteFirst;
        }
        if (byteFirst == BACK_SPACE)
        {
            nStr[it] = 0;
            if (it != -1)
                it--;
        }
        if (byteFirst == ENTER)
        {
            if (nStr[0] != 0)
                break;
        }
    }
    return nStr;
}

struct Iterator* eraseChainWithGetch(struct Iterator* iterator)
{
    int firstIndex;
    int secondIndex;

    for (;;) // get first index
    {
        system("cls");
        firstIndex = getNumberWithGetch("Ведите первый индекс: ");
        if (!(firstIndex < 0 || firstIndex >= iterator->list->size))
            break;
        else
        {
            printf("\n\nИндекс не верный, введите заново");
            Sleep(2000);
            system("cls");
        }
    }

    for (;;) // get second index
    {
        system("cls");
        secondIndex = getNumberWithGetch("Ведите второй индекс: ");
        if (!(secondIndex < 0 || secondIndex >= iterator->list->size || secondIndex < firstIndex))
            break;
        else
        {
            printf("\n\nИндекс не верный, введите заново");
            Sleep(2000);
            system("cls");
        }
    }

    iterator = eraseChain(iterator->list, firstIndex, secondIndex);
    return iterator;
}

void initialiseList(struct List* list)
{
    int numberOfMembers = getFirstSizeWithGetch("Ведите количество элементов списка:  ");
    if (numberOfMembers < 0)
        numberOfMembers *= -1;

    const char* numberOfMembersStr = numToStr(numberOfMembers);
    for (int i = 0; i < numberOfMembers; i++)
    {
        const char* numOfMemStr = numToStr(i);
        char message[300] = { 'В','в','е','д','и','т','е',' ','э','л','е','м','е','н','т','(' }; // 13
        strcpy(message + getItEnd(message), numOfMemStr);
        strcpy(message + getItEnd(message), "-й, всего ");
        strcpy(message + getItEnd(message), numberOfMembersStr);
        strcpy(message + getItEnd(message), "): ");

        int newMember = getNumberWithGetch(message);
        insertBack(list, newMember);
    }
    system("cls");
}

void printMenu(int state)
{
    printf("1) Добавить элемент после указанного");     if (state == 1) printf(" <-"); printf("\n");
    printf("2) Добавить первый элемент");               if (state == 2) printf(" <-"); printf("\n");
    printf("3) Изменить элемент");                      if (state == 3) printf(" <-"); printf("\n");
    printf("4) Удалить элемент");                       if (state == 4) printf(" <-"); printf("\n");
    printf("5) Добавить цепочку элементов");            if (state == 5) printf(" <-"); printf("\n");
    printf("6) Удалить цепочку элементов");             if (state == 6) printf(" <-"); printf("\n");
    printf("7) Записать цепочку в файл");               if (state == 7) printf(" <-"); printf("\n");
    printf("8) Добавить цепочку из файла");             if (state == 8) printf(" <-"); printf("\n");
    printf("9) Перестроить список");                    if (state == 9) printf(" <-"); printf("\n");
}

void changeState(int* state, char byteFirst, char byteSecond)
{
    if (byteSecond == DOWN)
        if (*state == SIZE_OF_MENU)
            *state = 1;
        else
            (*state)++;

    if (byteSecond == UP)
        if (*state == 1)
            *state = SIZE_OF_MENU;
        else
            (*state)--;

    if (byteFirst >= '1' && byteFirst <= '9')
        (*state) = byteFirst - '0';
}

void writeChainToFile(struct List* list)
{
    int firstIndex;
    int secondIndex;

    for (;;) // get first index
    {
        system("cls");
        firstIndex = getNumberWithGetch("Ведите первый индекс: ");
        if (!(firstIndex < 0 || firstIndex >= list->size))
            break;
        else
        {
            printf("\n\nИндекс не верный, введите заново");
            Sleep(2000);
            system("cls");
        }
    }

    for (;;) // get second index
    {
        system("cls");
        secondIndex = getNumberWithGetch("Ведите второй индекс: ");
        if (!(secondIndex < 0 || secondIndex >= list->size || secondIndex < firstIndex))
            break;
        else
        {
            printf("\n\nИндекс не верный, введите заново");
            Sleep(2000);
            system("cls");
        }
    }

    struct FILE* f = NULL;
    char nameOfFile[200] = { 0 };
    for (;;)
    {
        system("cls");
        printf("Введите имя файла: ");
        scanf("%s", nameOfFile);
        f = fopen(nameOfFile, "wt");
        if (!(f == NULL))
            break;
        else
            for (int i = 0; i < 200; i++)
                nameOfFile[i] = '\0';
    }

    struct Iterator* it = createFrontIterator(list);
    relocateIterator(it, firstIndex);

    for (int i = firstIndex; i <= secondIndex; i++)
    {
        fprintf(f, "%d ", it->node->value);
        relocateIterator(it, 1);
    }

    fclose(f);
}

struct Iterator* readChainFromFile(struct Iterator* it)
{
    struct FILE* f = NULL;
    char nameOfFile[200] = { 0 };
    for (;;)
    {
        system("cls");
        printf("Введите имя файла: ");
        scanf("%s", nameOfFile);
        f = fopen(nameOfFile, "rt");
        if (!(f == NULL))
            break;
        else
            for (int i = 0; i < 200; i++)
                nameOfFile[i] = '\0';
    }

    char elements[500] = { 0 };
    fgets(elements, 498, f);
    it = insertChainAfterIterator(it, elements);
    return it;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    struct List* list = createList();
    if (list == NULL)
    {
        printf("Memory Error");
        return;
    }

    initialiseList(list);
    struct Iterator* iterator = createFrontIterator(list);

    unsigned char byteFirst = -1;
    unsigned char byteSecond = -1;

    int state = 1;
    int enterPressed = 0;

    for (;;)
    {
        system("cls");
        printf("list: \n");
        print(list);
        for (int i = 0; i < getNumOfNode(iterator); i++)
            printf("     |");

        printf("  ");
        printf("*  |");

        for (int i = 0; i < iterator->list->size - getNumOfNode(iterator) - 1; i++)
            printf("     |");

        printf("\n");

        printMenu(state);

        byteFirst = getch();
        if (byteFirst == 224 || byteFirst == 0)
            byteSecond = getch();
        else
            byteSecond = 0;

        changeState(&state, byteFirst, byteSecond);
        if (byteFirst == 224 && byteSecond == LEFT)
        {
            relocateIterator(iterator, -1);
        }
        if (byteFirst == 224 && byteSecond == RIGHT)
        {
            relocateIterator(iterator, 1);
        }
        if (byteFirst == ESC)
        {
            return 0;
        }
        if (byteFirst == ENTER)
        {
            enterPressed = 1;
        }

        if (enterPressed == 1)
        {
            if (state == 1) // insert element after index
            {
                if (list->size != 0)
                {
                    int insertNumber = getNumberWithGetch("Введите значение элемента: ");
                    insertAfterIterator(iterator, insertNumber);
                }
            }

            if (state == 2) // insert first element
            {
                int insertNumber = getNumberWithGetch("Введите значение первого элемента: ");
                if (iterator->list->size == 0)
                {
                    iterator->node = insertFront(iterator->list, insertNumber);
                }
                else
                {
                    insertFront(list, insertNumber);
                }
            }

            if (state == 3) // change element
            {
                int insertNumber = getNumberWithGetch("Введите значение элемента: ");
                changeElementIterator(iterator, insertNumber);
            }

            if (state == 4) // delete element
            {
                erase(iterator);
            }
            if (state == 5) // add a chain of elements
            {
                insertChainAfterIterator(iterator, getChainStrWithGetch("Введите значение элементов цепочки: "));
            }
            if (state == 6) // erase a chain of elements
            {
                iterator = eraseChainWithGetch(iterator);
            }
            if (state == 7) // write to file
            {
                writeChainToFile(iterator->list);
            }
            if (state == 8) // add a chain of elements
            {
                iterator = readChainFromFile(iterator);
            }
            if (state == 9) // rebuild
            {
                if (iterator->list->size != 0)
                    rebuildList(iterator);
            }

            enterPressed = 0;
        }
    }
}
