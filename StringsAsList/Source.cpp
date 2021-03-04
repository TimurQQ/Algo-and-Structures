#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define NOWHITEBLANK 0
#define WHITEBLANK 1

typedef struct Node
{
	char c;
	struct Node *next;
} Node;

Node* deleteList(Node* ptr)
{
	while(ptr!=NULL)
	{
		struct Node* nextNode = ptr->next;
		free(ptr);
		ptr = nextNode;
	}
	return NULL;
}

int getList(Node** pptr)
{
	char buf[81], * str;
	Node head = { '\0', NULL };
	Node* last = &head;
	int n, rc = 1;
	do
	{
		n = scanf_s("%80[^\n]", buf, 81);
		if (n < 0)
		{
			deleteList(head.next);
			head.next = NULL;
			rc = 0;
			continue;
		}
		if (n > 0)
		{
			for (str = buf; *str != '\0'; ++str)
			{
				last->next = (Node*)malloc(sizeof(Node));
				last = last->next;
				last->c = *str;
			}
			last->next = NULL;
		}
		else
			scanf_s("%*c");
	} while (n > 0);
	*pptr = head.next;
	return rc;
}

void putList(const char* pattern, Node* ptr)
{
	if (ptr == NULL || ptr->c == '\0')
	{
		printf("\n%s\n%s\n", pattern,"Пустая строка!");
		return;
	}
	printf("\n%s\n\"", pattern);
	while (ptr->c != '\0')
	{
		printf("%c", ptr->c);
		struct Node* nextNode = ptr->next;
		ptr = nextNode;
	}
	printf("\"\n");
}

Node* delSpace(Node* ptr)
{
	while (ptr != NULL && (ptr->c == ' ' || ptr->c == '\t'))
		ptr = ptr->next;
	return ptr;
}

Node* reorg(Node* p)
{
	char firstSymb = ' ';
	Node head = { '\0',p }, * last = &head;
	while (last && (last->next = delSpace(last->next)))
	{
		firstSymb = (firstSymb == ' ' || firstSymb == '\t') ? last->next->c : firstSymb;
		Node* ptr = last->next;
		Node* prev = last;
		char flag = NOWHITEBLANK;
		while (ptr != NULL && ptr->c !=' ')
			if (ptr->c != firstSymb)
			{
				prev = last;
				last = last->next = ptr;
				ptr = last->next;
				flag = WHITEBLANK;
			}
			else
			{
				prev = ptr;
				ptr = ptr->next;
			}
				
		if (flag == WHITEBLANK)
		{
			prev = last;
			last = last->next = ptr;
		}
		else last->next = ptr;
	}
	last->c = '\0';
	return head.next;
}

void SetRusLang()
{
	system("chcp 1251");
	system("cls");
}

int main()
{
	SetRusLang();
	Node* st;
	while (puts("\n********Enter string********\n"), getList(&st))
	{
		st = reorg(st);
		putList("\n*****Reorganized String*****\n", st);
		st = deleteList(st);
	}
	return 0;
}