#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef char* string;

char keys[151] = " \t\nabcdefghijklmnopqrstuvwxyzABCDEF"
				"GHIJKLMNOPQRSTUVWXYZàáâãäå¸æçèéêëìíîïğñò"
				"óôõö÷ûúüéışÿÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ÛÚÜÉİŞß"
				"!@#$%^&*()_+-=\|}{[];:\'/?.,<>\"~`";

void SetRusLang()
{
	system("chcp 1251");
	system("cls");
}

string getline(void)
{
	printf("Ââåäèòå ñòğîêó ÷èñåë èëè Êîíåö Ôàéëà: ");
	string ptr = (string)malloc(1);
	char buf[21];
	int n, len = 0;
	*ptr = '\0';
	do
	{
		n = scanf_s("%20[^\n]", buf, 21);
		if (n < 0)
			ptr = NULL;
		else if (n == 0)
			scanf_s("%*c");
		else
		{
			len += strlen(buf);
			ptr = (string)realloc(ptr, len + 2);
			strcat(ptr, buf);
		}
	} while (n > 0);
	if (ptr != NULL) strcat(ptr, "\n");
	return ptr;
}

string skipSpace(string s)
{
	int k = strspn(s, keys);
	return s + k;
}

string SkipNum(string s)
{
	int k = strcspn(s, keys);
	return s + k;
}

void revString(string bigNum)
{
	unsigned int len = strlen(bigNum);
	for (unsigned int i = 0; i < len / 2; i++)
	{
		char tmp = bigNum[i];
		bigNum[i] = bigNum[len - 1 - i];
		bigNum[len - 1 - i] = tmp;
	}
}

string copyNumber(string from, string to)
{
	int k = strcspn(from, keys);
	strncat(to, from, k);
	revString(to);
	int n = strspn(to, "0");
	if (k == n) n--;
	memcpy(to, to + n, k - n);
	to += k-n;
	*to++ = ' ';
	*to = '\0';
	return to;
}

string createResString(string s)
{
	string p = (string)malloc(strlen(s) + 1);
	char* tmp = p;
	*tmp = '\0';
	int k = 0;
	while (*(s = skipSpace(s)))
	{
		tmp = copyNumber(s, tmp);
		s = SkipNum(s);
	}
	if (p != tmp)
		*--tmp = '\0';
	p = (string)realloc(p, strlen(p) + 1);
	return p;
}

int main(void)
{
	SetRusLang();
	string s, res_str;
	while ((s = getline()) != NULL)
	{
		printf("Init String : %s\n", s);
		res_str = createResString(s);
		printf("     Result : %s\n", res_str);
		free(s);
		free(res_str);
	}
	return 0;
}