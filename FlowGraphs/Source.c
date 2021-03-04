#include <stdlib.h>
#include "graph.h"

string startMenu[] = {
	"0.Создать пустой граф",
	"1.Создать случайный граф",
	"2.Загрузить граф из файла",
	"3.Выйти"
};

string graphMenu[] = {
	"0.Добавить вершины в граф",
	"1.Удалить вершину графа",
	"2.Добавить ребра в граф",
	"3.Удалить ребра из графа",
	"4.Определить максимальную производительность",
	"5.Вывести граф в консоль",
	"6.Выйти в начальное меню",
	"7.Выйти"
};

void(*startMenuFunctions[])(Graph*) = {
	D_Create_Graph,
	D_Random_Graph,
	D_Load_Graph,
	NULL
};

void(*graphMethods[])(Graph*) = {
	D_Add_Node,
	D_Del_Node,
	D_Add_Connection,
	D_Del_Connection,
	D_Max_Product,
	D_Print_Graph,
	NULL, NULL
};

void setRusLang(void) { system("chcp 1251 && cls"); }

int startDialog(void) {
	int n, funcNumber;
	string errmsg = "";
	do {
		puts(errmsg);
		errmsg = "Неверное значение.Повторите ввод!\n";
		for (int i = 0; i < sizeof(startMenu) / sizeof(startMenu[0]); i++) {
			printf("%s\n", startMenu[i]);
		}
		printf("Выберите пункт меню: --> ");
		n = _getInt(&funcNumber);
		if (n == 0) funcNumber = 0;
	} while (funcNumber < 0 || funcNumber > 3);
	return funcNumber;
}

int graphDialog(void) {
	int n, funcNumber;
	string errmsg = "";
	do {
		puts(errmsg);
		errmsg = "Неверное значение.Повторите ввод!\n";
		for (int i = 0; i < sizeof(graphMenu) / sizeof(graphMenu[0]); i++) {
			printf("%s\n", graphMenu[i]);
		}
		printf("Выберите пункт меню: --> ");
		n = _getInt(&funcNumber);
		if (n == 0) funcNumber = 0;
	} while (funcNumber < 0 || funcNumber > 7);
	return funcNumber;
}

int main(void) {
	int funcNumber;
	srand(42);
	setRusLang();
	do {
		Graph* graph = makeGraph();
		funcNumber = startDialog();
		if (funcNumber == 3) {
			free(graph);
			return 0;
		}
		startMenuFunctions[funcNumber](graph);
		while ((funcNumber = graphDialog()) < 6) {
			graphMethods[funcNumber](graph);
		}
		fclose(graph->fd);
		free(graph);
		if (funcNumber == 7) return 0;
	} while (1);
	return 0;
}