#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef char* string;

typedef struct Item
{
	int index;
	string value;
	struct Item* next;
	struct Item* prev;
} Item;

typedef struct Node
{
	int key;
	Item* info;
	struct Node* nextNode;
	struct Node* prevNode;
} Node;

typedef struct Table
{
	int size;
	Node* pFirst;
} Table;

const string msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5.ClearConsole"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

void SetRusLang()
{
	system("chcp 1251");
	system("cls");
}

void ignoreConsoleBuf()
{
	int n;
	n = scanf("%*[^\n]");
	n = scanf("%*c");
}

int _getInt(int* a)
{
	int n;
	n = scanf("%d", a);
	if (n != -1)ignoreConsoleBuf();
	return n < 0 ? 0 : 1;
}

int getInt(int* a)
{
	int n;
	do{
		n = scanf("%d", a);
		if (n == 0)
		{
			puts("You are wrong. Repeate, please!");
			ignoreConsoleBuf();
		}
	} while (n == 0);
	return n < 0 ? 0 : 1;
}

int dialog(const string msgs[], int N)
{
	string errmsg = "";
	int rc, i, n;
	do{
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";
		for (i = 0; i < N; ++i)
			puts(msgs[i]);
		printf("Make your choice: --> ");
		n = _getInt(&rc);
		if (n == 0) rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}

Table* createTable()
{
	Table* newTable = malloc(sizeof(Table));
	if (newTable == NULL) return NULL;
	newTable->pFirst = NULL;
	newTable->size = 0;
	return newTable;
}

string getStr(void)
{
	printf("Введите строку чисел или Конец Файла: ");
	string ptr = (string)malloc(1);
	if (ptr == NULL) return NULL;
	char buf[21];
	buf[20] = '\0';
	int n, len = 0;
	*ptr = '\0';
	do{
		n = scanf_s("%20[^\n]", buf, 21);
		if (n < 0)
			ptr = NULL;
		else if (n == 0)
			scanf_s("%*c");
		else
		{
			len += strlen(buf);
			string tmp = ptr;
			ptr = (string)realloc(ptr, len + 2);
			if (ptr == NULL) ptr = tmp;
			strcat(ptr, buf);
		}
	} while (n > 0);
	return ptr;
}

int D_Add(Table*), D_Find(Table*), D_Delete(Table*), D_Show(Table*);

int ClearConsole(Table* ptab)
{
	system("cls");
	return 1;
}

int (*fptr[])(Table*) = { NULL, D_Add, D_Find, D_Delete, D_Show, ClearConsole };

Table* delTable(Table* ptab)
{
	Node* ptr = ptab->pFirst;
	if (ptr == NULL) return NULL;
	for (; ptab->size != 0; ptab->size--)
	{
		Node* tmp = ptr;
		ptr = ptr->nextNode;
		free(tmp);
	}
	free(ptab);
	return NULL;
}
//////////////////////////////////
int main(void)
{
	SetRusLang();
	Table* table = createTable();
	int rc;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](table))
			break;
	puts("That's all. Bye!");
	table = delTable(table);
	return 0;
}
///////////////////////////////////
const string errmsgs[] = { "Ok", "This key doesn't exist", "Item doesn't exist"};

Node* findKey(Table* ptab, int key)
{
	Node* ptr = ptab->pFirst;
	for (int i = 0; i < ptab->size; ++i)
	{
		if (ptr->key == key)
			return ptr;
		ptr = ptr->nextNode;
	}
	return NULL;
}

Item* createItem(string newValue , int index)
{
	Item* newItem = malloc(sizeof(Item));
	if (newItem == NULL) return NULL;
	newItem->value = newValue;
	newItem->next = newItem->prev = newItem;
	newItem->index = index;
	return newItem;
}

int addItem(Item* infList, string newValue)
{
	int rc = 0;
	Item* pLast = infList->prev;
	Item* newpLast = pLast->next = createItem(newValue, pLast->index + 1);
	infList->prev = newpLast;
	newpLast->next = infList;
	newpLast->prev = pLast;
	return rc;
}

Node* createNode(int key, string newValue)
{
	Node* newNode = malloc(sizeof(Node));
	if (newNode == NULL) return NULL;
	newNode->key = key;
	newNode->nextNode = newNode->prevNode = newNode;
	newNode->info = createItem(newValue, 0);
	return newNode;
}

int addNode(Table* ptab, int key, string newValue)
{
	int rc = 0;
	if (ptab->pFirst == NULL)
		ptab->pFirst = createNode(key, newValue);
	else
	{
		Node* pLast = ptab->pFirst->prevNode;
		Node* newpLast = pLast->nextNode = createNode(key, newValue);
		ptab->pFirst->prevNode = newpLast;
		newpLast->nextNode = ptab->pFirst;
		newpLast->prevNode = pLast;
	}
	ptab->size++;
	return rc;
}

int insert(Table* ptab, int key, string newValue)
{
	int rc = 0;
	Node* keyNode = findKey(ptab, key);
	rc = (keyNode == NULL) ? addNode(ptab, key, newValue) : addItem(keyNode->info, newValue);
	return rc;
}

int D_Add(Table* ptab)
{
	int k, rc, n;
	char* info = NULL;
	printf("Enter key: -->");
	n = getInt(&k);
	ignoreConsoleBuf();
	if (n == 0) return 0;
	puts("Enter info:");
	info = getStr();
	if (info == NULL) return 0;
	rc = insert(ptab, k, info);
	printf("%s: %d\n", errmsgs[rc], k);
	return 1;
}

Table* findItem(Table* ptab, int key, int index)
{
	Node* pNode = findKey(ptab, key);
	Table* res = createTable();
	if (pNode == NULL) return res;
	Item* pFirst = pNode->info;
	Item* ptr = pFirst;
	do {
		if (ptr->index == index)
			insert(res, key, ptr->value);
		ptr = ptr->next;
	} while (ptr != pFirst);
	return res;
}

Table* findNode(Table* ptab, int key)
{
	Node* pNode = findKey(ptab, key);
	Table* res = createTable();
	if (pNode == NULL) return res;
	Item* pFirst = pNode->info;
	Item* ptr = pFirst;
	do{
		insert(res, key, ptr->value);
		ptr = ptr->next;
	} while (ptr != pFirst);
	return res;
}

int variousInput(int* pK, int* pVariant, int* pV)
{
	int k, rc = 0, n, variant, v = -1;
	string info = NULL;
	printf("Enter key: -->");
	n = getInt(&k);
	ignoreConsoleBuf();
	if (n == 0) return 0;
	puts("Хотите уточнить версию?\n1. Да\n2. Нет");
	do {
		n = getInt(&variant);
		ignoreConsoleBuf();
		if (variant == 1)
		{
			printf("\nВведите версию : ");
			n = getInt(&v);
			ignoreConsoleBuf();
			break;
		}
		else if (variant == 2)
			break;
		else
			printf("You are wrong. Repeate, please!\n");
	} while (1);
	*pK = k, * pVariant = variant, * pV = v;
	return 1;
}

int D_Find(Table* ptab)
{
	int k, rc = 0, n, variant, v;
	n = variousInput(&k, &variant, &v);
	if (n == 0) return 0;
	if (variant == 1)
	{
		Table* tab = findItem(ptab, k, v);
		if (tab->size == 0) rc = 2;
		D_Show(tab);
	}
	else
	{
		Table* tab = findNode(ptab, k);
		if (tab->size == 0) rc = 1;
		D_Show(tab);
	}
	printf("%s\n", errmsgs[rc]);
	return 1;
}

int deleteNode(Table* ptab, int key)
{
	Node* ptr = findKey(ptab, key);
	if (ptr == NULL) return 1;
	ptr->nextNode->prevNode = ptr->prevNode;
	ptr->prevNode->nextNode = ptr->nextNode;
	if (ptr == ptab->pFirst)
		ptab->pFirst = ptr->nextNode;
	free(ptr);
	if (--(ptab->size) == 0) ptab->pFirst = NULL;
	return 0;
}

void reCount(Node* pNode)
{
	Item* pFirst = pNode->info;
	Item* ptr = pFirst;
	int i = 0;
	do {
		ptr->index = i++;
		ptr = ptr->next;
	} while (ptr != pFirst);
}

int deleteItem(Table* ptab, int key, int index)
{
	Node* ptr = findKey(ptab, key);
	if (ptr == NULL) return 1;
	Item* pItem = ptr->info;
	do
	{
		if (pItem->index == index)
			break;
		pItem = pItem->next;
	} while (pItem != ptr->info);
	if (pItem ->index != index) return 2;
	pItem->next->prev = pItem->prev;
	pItem->prev->next = pItem->next;
	reCount(ptr);
	if (pItem == ptr->info && pItem->next == pItem)
		deleteNode(ptab, key);
	else if (pItem == ptr->info)
		ptr->info = pItem->next;
	free(pItem);
	return 0;
}

int D_Delete(Table* ptab)
{
	int k, rc, n, variant, v;
	n = variousInput(&k, &variant, &v);
	if (n == 0) return 0;
	rc = (variant == 1) ? deleteItem(ptab, k, v) : deleteNode(ptab, k);
	printf("%s\n", errmsgs[rc]);
	return 1;
}

void printInfo(Node* infNode)
{
	int key = infNode->key;
	Item* ptr = infNode->info;
	printf("%75d : ", key);
	printf("%s ", ptr->value);
	ptr = ptr->next;
	while (ptr != infNode->info)
	{
		printf("-> %s ", ptr->value);
		ptr = ptr->next;
	}
}

int D_Show(Table* ptab)
{
	Node* ptr = ptab->pFirst;
	for (int i = 0; i < ptab->size; ++i)
	{
		printInfo(ptr);
		puts("");
		ptr = ptr->nextNode;
	}
	if (ptab->size == 0)
		printf("\n%90s\n", "Таблица пуста!");
	return 1;
}