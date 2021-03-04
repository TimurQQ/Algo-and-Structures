#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define MAX_SIZE 100
#define TRUE 1
#define FALSE 0

typedef int bool;
typedef char* string;

typedef struct Item 
{
    int key;
    string text;
} Item;

#define T 2

typedef struct Node 
{
    int n;
    struct Node* ptr[2 * T];
    Item* item[2 * T - 1];
} Node;

Item table[MAX_SIZE];

int getInt(int* a);
int hash(int k) { return k % MAX_SIZE; }
void AddItem(Item* inf);
Item* CreateItem(int key, string text);
Node* CreateNode(void);

int dialog(const string msgs[], int N);
void Table_Print_Menu(Node**);
void Tree_Search_Menu(Node**);
void Tree_Insert_Menu(Node**);
void Tree_Print_Menu(Node**);
void Tree_Delete_Menu(Node**);
void Tree_Clear_Menu(Node**);
void ClearConsole(Node** nothing) { system("cls"); }

const string msgs[] = {
    "0. Выйти",
    "1. Добавить новый элемент в дерево",
    "2. Просмотреть кэш-буфер",
    "3. Найти в дереве",
    "4. Вывести дерево в консоль",
    "5. Удалить элемент из дерева",
    "6. Очистить дерево",
    "7. Очистить консольный вывод"
};

void(*fptr[])(Node**) = {
    NULL,
    Tree_Insert_Menu,
    Table_Print_Menu,
    Tree_Search_Menu,
    Tree_Print_Menu,
    Tree_Delete_Menu,
    Tree_Clear_Menu,
    ClearConsole
};

Item* B_Tree_Search(Node*, int key);
int B_Tree_Insert(Node**, Item** item);
int findPos(Node*, int key);
bool isLeaf(Node*);
Node* nodeLshift(Node*, int);
Node* nodeRshift(Node*, int);
Item* getMin(Node*);
Item* getMax(Node*);
int splitNode(Node*, int);
int B_Insert_Nonfull(Node*, Item* info);
int B_Tree_Delete(Node**, int key);
void B_Tree_Clear(Node**);
Node* mergeNode(Node*, int);
Node* reallocLeftToRight(Node*, int);
Node* reallocRightToLeft(Node*, int);

string randStr(int len);
void PrintNode(Node* node, int level);
void SetRusLang(void) { system("chcp 1251 && cls"); }

int main(void)
{
    SetRusLang();
    int rc;
    Node* tree = CreateNode();
    while (rc = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]))) {
        fptr[rc](&tree);
    }
}

int getInt(int* a)
{
    int n, k;
    do {
        n = scanf("%d", a);
        if (n < 0) return 0;
        if (!n)
            printf("Error. Repeat input\n");
        k = scanf("%*[^\n]");
        k = scanf("%*c");
    } while (n == 0);
    return 1;
}

int dialog(const string msgs[], int N)
{
    string errmsg = "";
    int rc, i, n;
    do {
        printf("%s", errmsg);
        errmsg = "Неверное значение. Повторите ввод!\n";
        printf("\n");
        for (i = 0; i < N; printf("%s\n", msgs[i]), i++);
        printf("Выберите пункт меню: --> ");
        n = getInt(&rc);
        if (!n) rc = 0;
    } while (rc < 0 || rc >= N);
    return rc;
}

void Table_Print_Menu(Node** x)
{
    printf("№\tkey\ttext\n");
    for (int i = 0; i < MAX_SIZE; i++) {
        if (table[i].text == NULL)
            printf("%d\t\tNULL\n", i);
        else
            printf("%d\t%d\t%s\n", i, table[i].key, table[i].text);
    }
}

void Tree_Search_Menu(Node** x)
{
    int key;
    if (!(*x)->n) 
    {
        printf("Дерево пустое!\n");
        return;
    }
    while (printf("Введите ключ: ")) 
    {
        if (!getInt(&key)) break;
        int p = hash(key);
        Item* res = B_Tree_Search(*x, key);
        if (res)
            printf("Элемент с ключом %d найден!\nИнформация: %s\n", res->key, res->text);
        else
            printf("Элемент с ключом %d не найден!\n", key);
    }
}

void Tree_Insert_Menu(Node** x) 
{
    int key, k;
    string text = (string)malloc(sizeof(char));
    while (printf("Введите ключ: "))
    {
        if (!getInt(&key)) break;
        printf("Введите информацию: ");
        if (scanf("%80[^\n]", text) < 0) break;
        k = scanf("%*c");
        Item* inf = CreateItem(key, text);
        if (!B_Tree_Insert(x, &inf))
        {
            printf("Запись добавлена!\n");
            AddItem(inf);
        }
        else
            printf("Ошибка: Дублирование ключей!\n");
    }
}

void Tree_Print_Menu(Node** x)
{
    if ((*x)->n)
        PrintNode(*x, 0);
    else
        printf("Дерево пустое!");
}

void Tree_Delete_Menu(Node** x)
{
    int key;
    printf("Введите ключ: ");
    if (!getInt(&key)) return;
    B_Tree_Delete(x, key);
}

void Tree_Clear_Menu(Node** x)
{
    B_Tree_Clear(x);
    printf("Дерево очищено!\n");
}

void AddItem(Item* inf)
{
    int pos = hash(inf->key);
    table[pos] = *inf;
}

string randStr(int len)
{ 
    string str = (string)malloc(len);
    if (str == NULL) return NULL;
    int x, i = 0;
    char a;
    for (i = 0; i < len; i++)
        str[i] = a = x = 65 + rand() % 25;
    str[len] = '\0';
    return str;
}

int B_Tree_Insert(Node** x, Item** item)
{
    if ((*x)->n == 2 * T - 1) 
    {
        Node* r = CreateNode();
        r->ptr[0] = *x;
        splitNode(r, 0);
        *x = r;
    }
    return B_Insert_Nonfull(*x, *item);
}

int B_Insert_Nonfull(Node* x, Item* info)
{
    int i;
    int p = hash(info->key);
    while (x->ptr[0])
    {
        i = findPos(x, info->key);
        if (i < x->n && x->item[i]->key == info->key)
            return 1;
        Node* pt = x->ptr[i];
        if (pt->n == 2 * T - 1) 
        {
            splitNode(x, i);
            if (info->key > x->item[i]->key)
                pt = x->ptr[i + 1];
        }
        x = pt;
    }
    i = findPos(x, info->key);
    if (i < x->n && info->key == x->item[i]->key)
        return -1;
    for (int j = x->n - 1; j >= i; j--)
        x->item[j + 1] = x->item[j];
    x->item[i] = info;
    x->n++;
    table[p] = *info;
    return 0;
}

int splitNode(Node* x, int i)
{
    int j;
    Node* y = x->ptr[i];
    for (j = x->n - 1; j >= i; j--)
    {
        x->ptr[j + 2] = x->ptr[j + 1];
        x->item[j + 1] = x->item[j];
    }
    Node* z = CreateNode();
    for (j = y->n / 2 + 1, z->n = 0; j < y->n; j++, z->n++)
    {
        z->ptr[z->n] = y->ptr[j];
        z->item[z->n] = y->item[j];
    }
    y->n /= 2;
    x->item[i] = y->item[y->n];
    z->ptr[z->n] = y->ptr[j];
    for (int i = 2 * T - 1; i > y->n; i--)
        y->item[i - 1] = NULL;
    x->ptr[i + 1] = z;
    x->n++;
    return 0;
}

void PrintNode(Node* node, int level) 
{
    if (node) 
    {
        for (int i = node->n; i; i--) 
        {
            PrintNode(node->ptr[i], level + 1);
            for (int j = 0; j < level; j++)
                printf("  ");
            printf("%2d\n", node->item[i - 1]->key);
        }
        PrintNode(node->ptr[0], level + 1);
    }
}

int findPos(Node* x, int key)
{
    int i = 0;
    for (; i < x->n && key > x->item[i]->key; i++);
    return i;
}

bool isLeaf(Node* x)
{
    for (int i = 0; i < 2 * T; i++)
        if (*(x->ptr + i) != NULL) return FALSE;
    return TRUE;
}

Node* nodeLshift(Node* x, int i)
{
    for (int j = i; j < x->n; j++) 
    {
        x->item[j - 1] = x->item[j];
        x->ptr[j] = x->ptr[j + 1];
    }
    x->n--;
    return x;
}

Node* nodeRshift(Node* x, int i)
{
    x->ptr[x->n + 1] = x->ptr[x->n];
    for (int j = x->n; j > i; j--) 
    {
        x->item[j] = x->item[j - 1];
        x->ptr[j] = x->ptr[j - 1];
    }
    x->n++;
    return x;
}

Item* getMin(Node* x)
{
    while (x->ptr[0])
        x = x->ptr[0];
    return x->item[0];
}

Item* getMax(Node* x)
{
    while (x->ptr[x->n])
        x = x->ptr[x->n];
    return x->item[x->n - 1];
}

Node* CreateNode(void)
{
    Node* r = (Node*)malloc(sizeof(Node));
    if (r == NULL) return NULL;
    int i;
    for (i = 0, r->n = 0; i < 2 * T; *(r->ptr + i) = NULL, i++);
    return r;
}

Item* CreateItem(int key, string text)
{
    Item* r = (Item*)malloc(sizeof(Item));
    if (r == NULL) return NULL;
    r->key = key;
    r->text = _strdup(text);
    return r;
}

int B_Tree_Delete(Node** r, int key)
{
    int i;
    Node* x = NULL, * lch = NULL,
        * rch = NULL, * y = NULL;
    Item* tmp;
    if ((*r)->ptr[0] && (*r)->n == 1 && (*r)->ptr[0]->n == T - 1 && (*r)->ptr[1]->n == T - 1)
    {
        lch = mergeNode(*r, 1);
        free(*r);
        *r = lch;
    }
    x = *r;
    while (x->ptr[0])
    {
        i = findPos(x, key);
        if (i < x->n && x->item[i]->key == key)
        {
            lch = x->ptr[i];
            rch = x->ptr[i + 1];
            if (lch->n >= T)
            {
                tmp = getMax(lch);
                key = tmp->key;
                x->item[i] = tmp;
                x = lch;
            }
            else if (rch->n >= T)
            {
                tmp = getMin(rch);
                key = tmp->key;
                x->item[i] = tmp;
                x = rch;
            }
            else
                x = lch = mergeNode(x, i + 1);
        }
        else
        { //key not Found
            y = x->ptr[i];
            if (y->n < T)
            {
                lch = rch = NULL;
                if (i > 0 && x->ptr[i - 1]->n >= T)
                { // Left Neighbor
                    lch = x->ptr[i - 1];
                    reallocLeftToRight(x, i - 1);
                }
                else if (i < x->n && x->ptr[i + 1]->n >= T)
                { // Right Neighbor
                    rch = x->ptr[i + 1];
                    reallocRightToLeft(x, i);
                }
                else
                    y = mergeNode(x, (rch) ? i + 1 : i);
            }
            x = y;
        }
    }
    i = findPos(x, key);
    if (i < x->n && x->item[i]->key == key)
    {
        nodeLshift(x, i + 1);
        return 0;
    }
    return -1;
}

Item* B_Tree_Search(Node* x, int key)
{
    int i;
    int p = hash(key);
    if (table[p].key == key) return &table[p];
    printf("В кэше нет элемента с ключом %d! Ищем в дереве\n", key);
    while (x)
    {
        i = findPos(x, key);
        if (x->item[i - 1] && x->item[i - 1]->key == key)
        {
            table[p] = *x->item[i];
            return x->item[i];
        }
        else
            x = x->ptr[i];
    }
    return NULL;
}

void B_Tree_Clear(Node** p)
{
    if (*p)
    {
        for (int i = 0; i <= (*p)->n; B_Tree_Clear(&((*p)->ptr[i])), i++);
        free(*p);
    }
    *p = CreateNode();
}

Node* mergeNode(Node* x, int i) {
    int j;
    Node* left = x->ptr[i - 1],
        * right = x->ptr[i];
    printf("[merge]");
    left->item[left->n++] = x->item[i - 1];
    nodeLshift(x, i);
    for (j = 0; j < right->n; j++, left->n++)
    {
        left->item[left->n] = right->item[j];
        left->ptr[left->n] = right->ptr[j];
    }
    left->ptr[left->n] = right->ptr[right->n];
    free(right);
    return left;
}

// справа налево
Node* reallocRightToLeft(Node* x, int i)
{
    printf("Realloc R -> L");
    Node* y = x->ptr[i];
    Node* z = x->ptr[i + 1];
    y->item[y->n] = x->item[i];
    ++y->n;
    y->ptr[y->n] = z->ptr[0];
    x->item[i] = z->item[0];
    z->ptr[0] = z->ptr[1];
    nodeLshift(z, 1);
    return x;
}

// слева направо
Node* reallocLeftToRight(Node* x, int i)
{
    printf("Realloc L -> R");
    Node* y = x->ptr[i];
    Node* z = x->ptr[i + 1];
    nodeRshift(z, 0);
    z->item[0] = x->item[i];
    z->ptr[0] = y->ptr[y->n];
    x->item[i] = y->item[y->n - 1];
    --y->n;
    return x;
}