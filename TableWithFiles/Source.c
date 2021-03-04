#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* string;

typedef struct Item
{
	int len;
	int index;
	struct Item* next;
	struct Item* prev;
} Item;

typedef struct Node
{
	int key;
	int offset;
	int len;
	Item* info;
	struct Node* next;
	struct Node* prev;
} Node;

typedef struct Table
{
	Node* pFirst;
	FILE* fd;
	char* fname;
} Table;

const string msgs[] = { "0. Quit", "1.LOAD", "2.Save" ,"3. Add", "4.Show", "5.Delete", "6.Find", "7.Clear Console" };

void SetRusLang()
{
	system("chcp 1251 && cls");
}

void ignoreConsoleBuf()
{
	int n;
	n = scanf("%*[^\n]");
	n = scanf("%*c");
}

int _getInt(int* a)
{
	int n = scanf("%d", a);
	if (n != -1) ignoreConsoleBuf();
	return n < 0 ? 0 : 1;
}

int getInt(int* a)
{
	int n;
	do {
		if ((n = scanf("%d", a)) == 0)
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
	do {
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
	newTable->pFirst = newTable->fd = NULL;
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
	do {
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

int D_Load(Table*), D_Save(Table*), D_Add(Table*), D_Show(Table*), D_Delete(Table*), D_Find(Table*);

int ClearConsole(Table* ptab)
{
	system("cls");
	return 1;
}

int (*fptr[])(Table*) = { NULL, D_Load, D_Save, D_Add, D_Show, D_Delete, D_Find, ClearConsole };

Table* delTable(Table* ptab)
{
	Node* ptr = ptab->pFirst;
	while (ptr)
	{
		Node* tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	free(ptab);
	return NULL;
}

////////////////////////////////////
int main(void)
{
	SetRusLang();
	Table* table = createTable();
	int rc;
	if (D_Load(table) == 0)
		return 1;
	while (rc = dialog(msgs, sizeof(msgs) / sizeof(msgs[0])))
		if (!fptr[rc](table))
			break;
	save(table);
	puts("That's all. Bye!");
	table = delTable(table);
	return 0;
}
////////////////////////////////////

const string errmsgs[] = { "Ok", "This key doesn't exist", "Item doesn't exist", "Something go wrong" };

int load(Table* ptab, char* fname)
{
	FILE* fd = NULL;
	string fdatname;
	int len = strlen(fname);
	Node* cur = NULL, * last = NULL;
	fdatname = _strdup(fname);
	if ((fdatname = realloc(fdatname, len + 5)) == NULL) return 0;
	strcat(fdatname, ".tab");
	ptab->fname = _strdup(fdatname);
	if ((fdatname = realloc(fname, len + 5)) == NULL) return 0;
	strcat(fdatname, ".dat");
	ptab->pFirst = NULL;
	fd = fopen(ptab->fname, "rb");
	if (fd != NULL)
	{
		ptab->fd = fopen(fdatname, "r+b");
		if (ptab->fd == NULL)
		{
			free(fdatname);
			return 0;
		}
		while (1)
		{
			Node* node = (Node*)malloc(sizeof(Node));
			if (node == NULL) return 2;
			node->key = node->len = node->offset = 0;
			node->info = node->next = node->prev = NULL;
			if (fread(node, sizeof(int), 3, fd) < 3)
				break;
			for (int i = 0; i < node->len; i++)
			{
				Item* item = (Item*)malloc(sizeof(Item));
				item->index = item->len = 0;
				item->next = item->prev = NULL;
				fread(item, sizeof(int), 2, fd);
				if (node->info == NULL)
					node->info = item;
				else
				{
					Item* pFirst = node->info;
					(node->info = item)->next = pFirst;
					pFirst->prev = node->info;
				}
			}
			if (ptab->pFirst == NULL)
				ptab->pFirst = node;
			else
			{
				Node* pFirst = ptab->pFirst;
				(ptab->pFirst = node)->next = pFirst;
				pFirst->prev = ptab->pFirst;
			}
		}
		fclose(fd);
	}
	else
	{
		printf("Такого файла не существует. Будет создана новая таблица");
		ptab->fd = fopen(fdatname, "w+b");
		if (ptab->fd == NULL)
		{
			free(fdatname);
			return 0;
		}
	}
	return 1;
}

int D_Load(Table* ptab)
{
	if (ptab->fd != NULL) fclose(ptab->fd);
	int rc = 1;
	string fname = NULL;
	printf("Enter file name: --> ");
	fname = getStr();
	if (fname == NULL) return 0;
	rc = load(ptab, fname);
	if (rc == 0)
		puts("The appropriate data file is not exists");
	else if (rc == 2)
		puts("Something go wrong. Maybe file is too big to load");
	return rc;
}

int fmwrite(FILE* f, void* Data, int size, int from)
{
	int bsize = 60000;
	fseek(f, 0L, 2);
	int s = ftell(f);
	fseek(f, s + size, 0);
	fseek(f, from, 0);
	int alloc = bsize > s ? s - from : bsize;
	char* temp = (char*)malloc(alloc);
	if (temp == NULL) return 3;
	int pose = s + size;
	while (pose > from)
	{
		int read = pose - size <= from ? size : alloc;
		fseek(f, pose - size - read, 0);
		fread((void*)temp, read, 1, f);
		fseek(f, pose - read, 0);
		fwrite((void*)temp, read, 1, f);
		pose -= read;
	}
	fseek(f, from, 0);
	return fwrite(Data, size, 1, f);
}

int save(Table* ptab)
{
	FILE* fd = NULL;
	Node* cur = NULL;

	fd = fopen(ptab->fname, "wb");
	if (fd == NULL) return 0;
	cur = ptab->pFirst;
	if (cur == NULL)
	{
		char end = 25;
		fwrite(&end, sizeof(char), 1, fd);
		return 1;
	}
	while (cur->next != NULL) cur = cur->next;
	for (; cur; cur = cur->prev)
	{
		fwrite(cur, sizeof(int), 3, fd);
		Item* pFirst = cur->info;
		Item* curItem = pFirst;
		while (curItem->next != NULL) curItem = curItem->next;
		for (; curItem; curItem = curItem->prev)
			fwrite(curItem, sizeof(int), 2, fd);
	}
	fclose(fd);
	return 1;
}

D_Save(Table* ptab)
{
	int rc = 1;
	rc = save(ptab);
	puts("done");
	return rc;
}

Node* findKey(Table* ptab, int key)
{
	for (Node* ptr = ptab->pFirst; ptr; ptr = ptr->next)
		if (ptr->key == key)
			return ptr;
	return NULL;
}

Item* createItem(string newValue, int index)
{
	Item* newItem = malloc(sizeof(Item));
	if (newItem == NULL) return NULL;
	newItem->len = strlen(newValue);
	newItem->next = newItem->prev = NULL;
	newItem->index = index;
	return newItem;
}

int addItem(Node* keyNode, string newValue)
{
	int rc = 0;
	Item* pFirst = keyNode->info;
	keyNode->info = createItem(newValue, pFirst->index + 1);
	keyNode->info->next = pFirst;
	pFirst->prev = keyNode->info;
	keyNode->len++;
	return rc;
}

Node* createNode(int key, string newValue)
{
	Node* newNode = malloc(sizeof(Node));
	if (newNode == NULL) return NULL;
	newNode->key = key;
	newNode->offset = 0;
	newNode->next = newNode->prev = NULL;
	newNode->len = 1;
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
		Node* pFirst = ptab->pFirst;
		ptab->pFirst = createNode(key, newValue);
		fseek(ptab->fd, 0L, SEEK_END);
		ptab->pFirst->offset = ftell(ptab->fd);
		ptab->pFirst->next = pFirst;
		pFirst->prev = ptab->pFirst;
	}
	return rc;
}

void chainRecount(Node* node, int addSize)
{
	node = node->prev;
	for (; node; node->offset += addSize, node = node->prev);
}

int insert(Table* ptab, int key, string newValue)
{
	int rc = 0;
	Node* keyNode = findKey(ptab, key);
	if (keyNode == NULL)
	{
		rc = addNode(ptab, key, newValue);
		fmwrite(ptab->fd, newValue, (strlen(newValue) + 1) * sizeof(char), ptab->pFirst->offset);
	}
	else
	{
		rc = addItem(keyNode, newValue);
		fmwrite(ptab->fd, newValue, (strlen(newValue) + 1)*sizeof(char), keyNode->offset);
		chainRecount(keyNode, (strlen(newValue) + 1) * sizeof(char));
	}
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
	save(ptab);
	printf("%s: %d\n", errmsgs[rc], k);
	return 1;
}

void printInfo(Node* infNode, FILE* datafile)
{
	int key = infNode->key;
	Item* ptr = infNode->info;
	int offset = infNode->offset;
	printf("%75d : ", key);
	FILE* seek = datafile;
	fseek(seek, offset, SEEK_SET);
	while (ptr)
	{
		string value = (string)calloc((ptr->len)+1,sizeof(char));
		if (value == NULL)
		{
			printf("При загрузке ноды произошла ошибка чтения");
			return;
		}
		fread(value, sizeof(char), ptr->len+1, seek);
		printf("-> (%s :: %d) ", value, ptr->index);
		ptr = ptr->next;
		free(value);
	}
}

int D_Show(Table* ptab)
{
	if (ptab->pFirst == NULL)
		printf("\n%90s\n", "Таблица пуста!");
	for (Node* ptr = ptab->pFirst; ptr; ptr = ptr->next)
	{
		printInfo(ptr, ptab->fd);
		puts("");
	}
	return 1;
}

int fmdelete(FILE* f, int size, int from)
{
	int bsize = 60000;
	fseek(f, 0L, 2);
	int s = ftell(f);
	fseek(f, from, 0);
	int alloc = s - from - size;
	char* temp = (char*)malloc(alloc);
	if (temp == NULL) return 0;
	int to_pos = from;
	int from_pos = from + size;
	FILE* fromfd = f;
	fseek(fromfd, from_pos, 0);
	fread((void*)temp, sizeof(char), alloc, fromfd);
	FILE* tofd = f;
	fseek(tofd, to_pos, 0);
	fwrite((void*)temp, sizeof(char), alloc, tofd);
	to_pos += alloc;
	fseek(tofd, to_pos, 0);
	char end = 25;
	fwrite(&end, sizeof(char), 1, tofd);
	fseek(f, from, 0);
	return 1;
}

int deleteNode(Table* ptab, int key)
{
	Node* ptr = findKey(ptab, key);
	if (ptr == NULL) return 1;
	if (ptr == ptab->pFirst)
	{
		ptab->pFirst = ptr->next;
		if (ptab->pFirst != NULL)
			ptab->pFirst->prev = NULL;
	}
	else
	{
		ptr->prev->next = ptr->next;
		if (ptr->next != NULL)
			ptr->next->prev = ptr->prev;
	}	
	int memlen = 0;
	for (Item* p = ptr->info; p; memlen += p->len + 1,p = p->next);
	fmdelete(ptab->fd, memlen, ptr->offset);
	chainRecount(ptr, -memlen);
	free(ptr);
	return 0;
}

int deleteItem(Table* ptab, int key, int index)
{
	Node* ptr = findKey(ptab, key);
	if (ptr == NULL) return 1;
	int offset = ptr->offset;
	Item* pItem = ptr->info;
	for (; pItem; pItem = pItem->next)
	{
		if (pItem->index == index)
			break;
		offset += (pItem->len + 1) * sizeof(char);
	}
	if (pItem == NULL) return 2;
	if (pItem == ptr->info)
	{
		ptr->info = pItem->next;
		if (ptr->info != NULL)
			ptr->info->prev = NULL;
	}
	else
	{
		pItem->prev->next = pItem->next;
		if (pItem->next != NULL)
			pItem->next->prev = pItem->prev;
	}
	int memlen = pItem->len + 1;
	fmdelete(ptab->fd, memlen, offset);
	chainRecount(ptr, -memlen);
	ptr->len--;
	if (ptr->info == NULL)
		deleteNode(ptab, key);
	free(pItem);
	return 0;
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

int D_Delete(Table* ptab)
{
	int k, rc, n, variant, v;
	n = variousInput(&k, &variant, &v);
	if (n == 0) return 0;
	rc = (variant == 1) ? deleteItem(ptab, k, v) : deleteNode(ptab, k);
	save(ptab);
	printf("%s\n", errmsgs[rc]);
	return 1;
}

int D_Find(Table* ptab)
{
	int k, rc = 0, n, variant, v;
	n = variousInput(&k, &variant, &v);
	if (n == 0) return 0;
	if (variant == 1)
	{
		Node* infNode = findKey(ptab, k);
		if (infNode == NULL)
			rc = 1;
		else
		{
			Item* ptr = infNode->info;
			int offset = infNode->offset;
			printf("%75d : ", k);
			FILE* seek = ptab->fd;
			fseek(seek, offset, 0);
			while (ptr != NULL)
			{
				for (; ptr->index != v; ptr = ptr->next);
				if (ptr == NULL)
					rc = 2;
				else
				{
					string value = (string)calloc((ptr->len) + 1, sizeof(char));
					if (value == NULL)
					{
						printf("При загрузке ноды произошла ошибка чтения");
						return 0;
					}
					fread(value, sizeof(char), ptr->len + 1, seek);
					printf("-> %s ", value);
					free(value);
				}
			}
		}
	}
	else
	{
		Node* ptr = findKey(ptab, k);
		if (ptr == NULL)
			rc = 1;
		else
		{
			printInfo(ptr, ptab->fd);
			puts("");
		}	
	}
	if (rc == 1) printf("\n%90s\n", "Таблица пуста!");
	printf("%s\n", errmsgs[rc]);
	return 1;
}