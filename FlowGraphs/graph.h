#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

typedef char* string;
struct Node;

typedef struct Connection {
	int maxFlow;
	int flow;
	struct Node* toNode;
	struct Connection* tConnect;
	struct Connection* next;
} Connection;

typedef struct Node {
	int used;
	string nodeName;
	Connection* Adj;
	struct Node* next;
} Node;

typedef struct Graph {
	Node* nodes;
	FILE* fd;
	string fname;
} Graph;

#define TRUE 1
#define FALSE 0
#define INF INT_MAX

void ignoreConsoleBuf(void);
int _getInt(int* a);
int getInt(int* a);
int getIntNum(FILE* fd, int* n);
string getStr(FILE* fd, const string pattern);

void D_Create_Graph(Graph*);
void D_Random_Graph(Graph*);
void D_Load_Graph(Graph*);

void D_Add_Node(Graph*);
void D_Del_Node(Graph*);
void D_Add_Connection(Graph*);
void D_Del_Connection(Graph*);
void D_Max_Product(Graph*);
void D_Find_Path(Graph*);
void D_Print_Graph(Graph*);

int getConnection(string* A, string* B, int* maxFlow);
Graph* makeGraph(void);
void addNode(Graph* graph, string nodeName);
char findNode(Graph* graph, string nodeName, Node** prev, Node** cur);
void findPair(Graph* graph, string A, string B, Node** nodeA, Node** nodeB);
int addConnection(Graph* graph, string A, string B, int maxFlow);
void printConnections(Node* node);
void deleteConnection(Graph* graph, Node* toNode, Connection* connection);
void deleteNode(Graph* graph, string nodeName);
void loadGraph(Graph* graph);
int dfs(Node* node, int Cmin, int cc);
void createRandGraph(Graph* graph, int n, int m);
Connection* findConnection(Node* node, string to, int maxFlow);
