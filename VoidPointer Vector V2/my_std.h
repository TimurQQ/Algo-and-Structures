#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//This function change the text-Code to Win-1251
void SetRusLang(void);
//This function can help you to scan some data of console
long getline(char s[]);
int getInt(int* num);
//Translate one thing to another thing
long hex_to_10(char s[]);
char* dec_to_2(int num, char s[]);
long double cels_to_fahr(long double celsius);
int str_to_int(char s[]);
char* int_to_str(int x, char s[]);
long double str_to_f(char s[]);
//Math functions
long double binpow(long double base, long degree);
long double power(long double base, long step);
long double sqrt_l(long double);
long double exp_l(long deg);
long double sin_l(long double angle);
//Functions with characters
char high_letter(char c);
char low_letter(char c);
int is_low_letter(char c);
int is_high_letter(char c);
int is_digit(char c);
int is_letter(char c);
int is_space(char c);
//Functions for strings
void squeeze(char s1[], char s2[]);
char* reverse(char s[]);
int in(char s[], char ch); //return TRUE or FALSE
int fsym_in(char s1[], char s2[]);
void expand(char s1[], char s2[]);
void replace(char s1[], char s2[]);
void concat(char s1[], char s2[], char res[]);
int compare(char s1[], char s2[]);
char* escape(char to[], char from[]);
int sum(char s1[], char s2[], char s[]);
int str_size(char s[]);
int str_right_shift(char s[]);
int strindex_l(char source[], char searchfor[]);
int strindex_r(char source[], char searchfor[]);
void str_copy(char to[], char from[]);
//Operations with Bits
int setbits(int x, int pos, int col, int y);
int invert(int x, int pos, int col);
int right_shift(int x, int col);
int bitcount(unsigned x);
//Random Modules
void randseed(unsigned seed);
int randomInt(void);
//Algo
int binsearch(int x, int v[], int l, int r);
void shellsort(int v[], int size);
void bubble_sort(int v[], int size);
void quickSort(int* arr, int first, int last);

#define MAXLEN 10000
#define LITER_DIST 7
#define TRUE 1
#define FALSE 0
#define _PI_ 3.141592
#define _E_ binpow(1.00000001,100000000)

////////////////////////////////////////////////////////////////////////////////////////
void SetRusLang(void)
{
	system("chcp 1251");
	system("cls");
}
///////////////////////////////////////////////////////////////////////////////////////////
long getline(char s[])
{
	int ch;
	long i;
	for (i = 0; i < MAXLEN && (ch = getchar()) != EOF && ch != '\n'; ++i)
	{
		s[i] = ch;
	}
	if (ch == '\n')
	{
		s[i++] = '\n';
		s[i] = '\0';
	}
	return i;
}

int getInt(int* num)
{
	*num = 0;
	int inNum = FALSE;
	int ch, sign = 1;
	while ((ch = getchar()) != EOF && (is_digit(ch) || is_space(ch) || ch == '-'))
	{
		if (!inNum && ch == '-')
			sign *= -1;
		else if (!inNum && is_space(ch))
			continue;
		else if (!inNum && is_digit(ch))
		{
			inNum = TRUE;
			*num = (ch - '0');
		}
		else if (inNum && is_digit(ch))
			*num = *num * 10 + (ch - '0');
		else if (inNum && !is_digit(ch))
			break;
	}
	if (!inNum)
	{
		printf("Error");
		scanf_s("%*[^\n]");
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////
long hex_to_10(char s[])
{
	int i = 0;
	if (s[0] == '0' && low_letter(s[1]) == 'x') i = 2;
	long res = 0;
	for (; s[i] != '\0'; ++i)
	{
		if (s[i] <= '9' && s[i] >= '0')
			res = res * 16 + (s[i] - '0');
		else if (high_letter(s[i]) <= 'Z' && high_letter(s[i]) >= 'A')
			res = res * 16 + (s[i] - LITER_DIST - '0');
	}
	return res;
}

char* dec_to_2(int num, char s[])
{
	int i = 0;
	do
	{
		s[i++] = num % 2 + '0';

	} while ((num /= 2) > 0);
	s[i] = '\0';
	reverse(s);
	return s;
}

long double cels_to_fahr(long double cels)
{
	return 9.0 / 5.0 * cels + 32.0;
}

int str_to_int(char s[])
{
	int i, n, sign;
	for (i = 0; is_space(s[i]); i++)
		;
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (n = 0; s[i] <= '9' && s[i] >= '0'; i++)
		n = 10 * n + (s[i] - '0');
	return sign * n;
}

long double str_to_f(char s[])
{
	long double val, pow;
	int i, sign;
	long exp;
	int exp_sign = 1;
	for (i = 0; is_space(s[i]); ++i);/*игнорируем левые символы-разделители*/
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		++i;
	for (val = 0.0; is_digit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if (s[i] == '.')
		i++;
	for (pow = 1.0; is_digit(s[i]); i++)
	{
		val = 10.0 * val + (s[i] - '0');
		pow *= 10.0;
	}
	if (s[i] == 'e' || s[i] == 'E')
	{
		i++;
	}
	exp_sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '-' || s[i] == '+') ++i;
	for (exp = 0; is_digit(s[i]); i++)
	{
		exp = exp * 10 + (s[i] - '0');
	}
	return sign * val / pow * binpow(10, exp_sign * exp);
}
////////////////////////////////////////////////////////////////////////////////////////////////
long double binpow(long double base, long deg)
{
	long double num;
	if (deg == 0) return 1;
	num = binpow(base, deg / 2);
	return (deg % 2 == 0) ? num * num : num * num * base;
}

long double power(long double base, long step)
{
	long double res = 1;
	for (long i = 0; i < step; ++i)
	{
		res *= base;
	}
	return res;
}

long double sqrt_l(long double base)
{
	long double low = 0.0, high = (base >= 1) ? base : 1, mid;
	while (1)
	{
		mid = (low + high) / 2.0;
		if (mid * mid - base > 0.00000001)
			high = mid;
		else if (base - mid * mid > 0.00000001)
			low = mid;
		else
			break;
	}
	return mid;
}

long double exp_l(long deg)
{
	return binpow(_E_, deg);
}

long double sin_l(long double angle)
{
	angle = angle - ((int)angle / 360) * 360;
	if (angle < 0) angle += 360;
	if (angle > 90 && angle <= 180)
	{
		return sin_l(180 - angle);
	}
	else if (angle > 180 && angle <= 270)
	{
		return -sin_l(angle - 180);
	}
	else if (angle > 270 && angle < 360)
	{
		return -sin_l(360 - angle);
	}
	angle = angle * _PI_ / 180;
	long double cos_x = 1.0 - binpow(angle, 2) / 2.0;
	long double sin_x = sqrt_l(1.0 - binpow(cos_x, 2));
	return sin_x;
}
///////////////////////////////////////////////////////////////////////////////////////////////
char high_letter(char c)
{
	return (c <= 'z' && c >= 'a') ? c - ('a' - 'A') : c;
}

int is_low_letter(char c)
{
	return (is_letter(c) && low_letter(c) == c);
}

int is_high_letter(char c)
{
	return (is_letter(c) && high_letter(c) == c);
}

char low_letter(char c)
{
	return (c <= 'Z' && c >= 'A') ? c + ('a' - 'A') : c;
}

int is_digit(char c)
{
	return (c <= '9' && c >= '0') ? TRUE : FALSE;
}

int is_letter(char c)
{
	return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') ? TRUE : FALSE;
}
int is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n') ? TRUE : FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void squeeze(char s1[], char s2[])
{
	int j = 0;
	for (int i = 0; s1[i] != '\0'; ++i)
	{
		char sym = s1[i];
		if (!in(s2, sym))
			s1[j++] = s1[i];
	}
	s1[j] = '\0';
}

char* reverse(char s[])
{
	int size = str_size(s);
	char tmp;
	for (int i = 0; i < size / 2; ++i)
	{
		tmp = s[i];
		s[i] = s[size - 1 - i];
		s[size - 1 - i] = tmp;
	}
	return s;
}

int in(char s[], char ch)
{
	for (int i = 0; s[i] != '\0'; ++i)
	{
		if (s[i] == ch)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int fsym_in(char s1[], char s2[])
{
	for (int i = 0; s1[i] != '\0'; ++i)
	{
		char sym = s1[i];
		if (in(s2, sym))
		{
			return i;
		}
	}
	return -1;
}

void expand(char s[], char str[])
{
	int i, j;
	for (i = 0, j = 0; s[i] != '\0'; ++i)
	{
		if (s[i] == '-' && i != 0)
		{
			if (is_digit(s[i - 1]) && is_digit(s[i + 1]) ||
				is_low_letter(s[i - 1]) && is_low_letter(s[i + 1]) ||
				is_high_letter(s[i - 1]) && is_high_letter(s[i + 1]))
			{
				int cnt = s[i - 1] + 1;
				if (s[i - 1] == s[i + 1])
					j--;
				else if (s[i - 1] > s[i + 1])
					str[j++] = s[i];
				else if (s[i + 1] - s[i - 1] == 1)
					continue;
				else
					while (cnt < s[i + 1])
						str[j++] = (cnt++);
			}
			else
				str[j++] = s[i];
		}
		else
			str[j++] = s[i];
	}
	str[j] = '\0';
}

/// first <- -> second
void replace(char s1[], char s2[])
{
	char trasfer[MAXLEN];
	for (unsigned int i = 0; s1[i] != '\0'; i++)
		trasfer[i] = s1[i];
	unsigned int index = 0;
	for (; s2[index] != '\0'; index++)
		s1[index] = s2[index];
	s1[index] = '\0';
	index = 0;
	for (; trasfer[index] != '\0'; index++)
		s2[index] = trasfer[index];
	s2[index] = '\0';
}

void concat(char s1[], char s2[], char res[])
{
	unsigned int index = 0;
	for (int i = 0; s1[i] != '\0'; i++, index++)
		res[index] = s1[i];
	for (int i = 0; s2[i] != '\0'; i++, index++)
		res[index] = s2[i];
	res[index] = '\0';
}
int compare(char s1[], char s2[])
{
	unsigned int i = 0;
	for (; s1[i] != '\0' || s2[i] != '\0'; i++)
		if (s1[i] != s2[i])
			if (s1[i] > s2[i])
				return 1;
			else
				return -1;
	if (s1[i] == '\0' && s2[i] == '\0')
		return 0;
	if (s1[i] != '\0')
		return 1;
	else
		return -1;
}

//from t[] to s[]
char* escape(char s[], char t[])
{
	int j = 0;
	for (int i = 0; t[i] != '\0'; ++i)
	{
		if (t[i] == ' ')
		{
			s[j++] = '\'', s[j++] = ' ', s[j++] = '\'';
		}
		else if (t[i] == '\n')
		{
			s[j++] = '\\', s[j++] = 'n';
		}
		else if (t[i] == '\t')
		{
			s[j++] = '\\', s[j++] = 't';
		}
		else
			s[j++] = t[i];
	}
	s[j] = '\0';
	return s;
}

int sum(char s1[], char s2[], char s[])
{
	int i, j, k = 0;
	int tmp = 0;
	int size1 = str_size(s1);
	int size2 = str_size(s2);
	for (j = size2 - 1, i = size1 - 1; size1 >= size2 && j >= 0 || size1 < size2 && i >= 0; --i, --j)
	{
		int sum = (s1[i] - '0') + (s2[j] - '0') + tmp;
		s[k++] = sum % 10 + '0';
		tmp = sum / 10;
	}
	while (size1 >= size2 && i >= 0 || size1 < size2 && j >= 0)
	{
		int sum = (size1 >= size2) ? s1[i] - '0' + tmp : s2[j] - '0' + tmp;
		s[k++] = sum % 10 + '0';
		tmp = sum / 10;
		i = i - (size1 >= size2);
		j = j - (size1 < size2);
	}
	if (tmp != 0)
	{
		s[k++] = tmp + '0';
		s[k] = '\0';
	}
	else
		s[k] = '\0';
	reverse(s);
	return k;
}

int str_size(char s[])
{
	int i = 0;
	for (; s[i] != '\0'; ++i);
	return i;
}

int str_right_shift(char s[])
{
	int n;
	for (n = str_size(s) - 1; n >= 0; --n)
	{
		if (!is_space(s[n]))
			break;
	}
	s[n + 1] = '\0';
	return n;
}

int strindex_l(char s[], char t[])
{
	for (int i = 0; s[i] != '\0'; ++i)
	{
		int k = 0;
		for (int j = i; t[k] != '\0' && s[j] == t[k]; ++j, ++k);
		if (k > 0 && t[k] == '\0')
			return i;
	}
	return -1;
}

int strindex_r(char s[], char t[])
{
	int t_size = str_size(t);
	int s_size = str_size(s);
	for (int i = s_size - 1; i >= 0; i--)
	{
		int k = t_size - 1;
		for (int j = i; k >= 0 && j >= 0 && s[j] == t[k]; --j, --k);
		if (k < 0) return i - t_size + 1;
	}
	return -1;
}

void str_copy(char to[], char from[])
{
	for (int i = 0; (to[i] = from[i]) != '\0'; ++i);
}
/////////////////////////////////////////////////////////////////////////////////////////
int setbits(int x, int pos, int col, int y)
{
	y = ((1 << col) - 1) & y;
	int right = ((1 << (pos - col + 1)) - 1) & x;
	int left = (~0 << (pos + 1)) & x;
	return left + (y << (pos - col + 1)) + right;
}

int invert(int x, int pos, int col)
{
	int right = ((1 << (pos - col + 1)) - 1) & x;
	int left = (~0 << (pos + 1)) & x;
	int inv = ~((x ^ right) ^ left);
	int mask = ((1 << col) - 1) << (pos - col + 1);
	return left + (inv & mask) + right;
}

int right_shift(int x, int col)
{
	col %= sizeof(int) * 8;
	int item = x & ((1 << col) - 1);
	x >>= col;
	item <<= (sizeof(int) * 8 - col);
	return item + x;
}

int bitcount(unsigned int x)
{
	int b = 0;
	while (x != 0)
	{
		x &= (x - 1);
		++b;
	}
	return b;
}
/////////////////////////////////////////////////////////////////////////////////////////
unsigned long _spec_next = 1;
void randseed(unsigned int seed)
{
	_spec_next = seed;
}

int randomInt(void)
{
	_spec_next = _spec_next * 1103515245 + 12345;
	return (unsigned)(_spec_next / 65536) % 32768;
}
/////////////////////////////////////////////////////////////////////////////////////////
/* binsearch:found х in v[0] <= v[1] <= Е <= v[n-1] */
int binsearch(int x, int v[], int l, int r)
{
	int low = l, high = r - 1, mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (x < v[mid])
			high = mid - 1;
		else if (x > v[mid])
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

void shellsort(int v[], int n)
{
	int gap, i, j, tmp;
	for (gap = n / 2; gap > 0; gap /= 2)
	{
		for (i = gap; i < n; i++)
		{
			for (j = i - gap; j >= 0 && v[j] > v[j + gap]; j -= gap)
			{
				tmp = v[j], v[j] = v[j + gap], v[j + gap] = tmp;
			}
		}
	}
}

void bubble_sort(int v[], int size)
{
	int tmp;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size - 1; ++j)
		{
			if (v[j] < v[j + 1])
			{
				tmp = v[j + 1], v[j + 1] = v[j], v[j] = tmp;
			}
		}
	}
}

void quickSort(int* arr, int first, int last)
{
	if (first < last)
	{
		int left = first, right = last, middle = arr[(left + right) / 2];
		do
		{
			while (arr[left] < middle) left++;
			while (arr[right] > middle) right--;
			if (left <= right)
			{
				int tmp = arr[left];
				arr[left++] = arr[right];
				arr[right--] = tmp;
			}
		} while (left <= right);
		quickSort(arr, first, right);
		quickSort(arr, left, last);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////