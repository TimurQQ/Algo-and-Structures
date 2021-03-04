#define _CRT_SECURE_NO_WARNINGS
#include "graph.h"

void ignoreConsoleBuf(void)
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

int getIntNum(FILE* fd, int* res) {
	int n =fscanf(fd, "%d", res);
	char tmp;
	int k =fscanf(fd, "%c", &tmp);
	return n > 0;
}

string getStr(FILE* fd, const string pattern) {
	char buf[81];
	buf[80] = '\0';
	string s = (string)malloc(1);
	if (s == NULL) {
		return NULL;
	}
	*s = '\0';
	while ( (fd != NULL) ? fscanf(fd, pattern, buf): scanf(pattern, buf)) {
		string tmp = s;
		s = realloc(s, strlen(s) + strlen(buf) + 1);
		if (s == NULL) {
			s = tmp;
		}
		strcat(s, buf);
	}
	char tmp;
	int n = (fd != NULL) ? fscanf(fd, "%c", &tmp) : scanf("%*c");
	return s;
}

void D_Create_Graph(Graph* graph) {
	if (graph->fname == NULL) {
		string fname = getStr(NULL, "%80[^\n]");
		string tmp = fname;
		fname = realloc(fname, strlen(fname) + 5);
		if (fname == NULL) {
			fname = tmp;
		}
		strcat(fname, ".txt");
		graph->fname = fname;
	}
		graph->fd = fopen(graph->fname, "w+");
	puts("done");
}

void createRandGraph(Graph* graph, int n, int m) {
	time_t t0 = time(0);
	n -= 2;
	char buf[11];
	_itoa(rand() % 1000000, buf, 10);
	strcat(buf, ".txt");
	graph->fname = _strdup(buf);
	graph->fd = fopen(graph->fname, "w+");
	buf[10] = '\0';
	for (int i = 0; i < n; ++i) {
		_itoa(i, buf, 10);
		addNode(graph, buf);
		fprintf(graph->fd, "%d;%s;", 0, buf);
	}
	int numS_to = (rand() % (int)(sqrt(n))) + 2;
	int numT_from = (rand() % (int)(sqrt(n))) + 2;
	m -= numS_to + numT_from;
	for (int i = 0; i < m; ++i) {
		_itoa(rand() % n, buf, 10);
		string v1 = _strdup(buf);
		_itoa(rand() % n, buf, 10);
		int maxFlow = rand() % 100;
		addConnection(graph, v1, buf, maxFlow);
		fprintf(graph->fd, "%d;%s;%s;%d;", 2, v1, buf, maxFlow);
		free(v1);
	}
	addNode(graph, "s");
	addNode(graph, "t");
	fprintf(graph->fd, "%d;%s;%d;%s;", 0, "s", 0, "t");
	
	for (int i = 0; i < numS_to; ++i) {
		_itoa(rand() % n, buf, 10);
		int maxFlow = rand() % 100 + 1;
		addConnection(graph, "s", buf, maxFlow);
		fprintf(graph->fd, "%d;%s;%s;%d;", 2, "s", buf, maxFlow);
	}
	
	for (int i = 0; i < numT_from; ++i) {
		_itoa(rand() % n, buf, 10);
		int maxFlow = rand() % 100 + 1;
		addConnection(graph, buf, "t", maxFlow);
		fprintf(graph->fd, "%d;%s;%s;%d;", 2, buf, "t", maxFlow);
	}
	time_t t1 = time(0);
	double time_in_seconds = difftime(t1, t0);
	printf("Time: %Lf\n", time_in_seconds);
}

void D_Random_Graph(Graph* graph) {
	int n;
	getInt(&n);
	ignoreConsoleBuf();
	int m;
	getInt(&m);
	ignoreConsoleBuf();
	createRandGraph(graph, n, m);
	puts("done");
}

void D_Load_Graph(Graph* graph) {
	string fname = getStr(NULL, "%80[^\n]");
	string tmp = fname;
	fname = realloc(fname, strlen(fname) + 5);
	if (fname == NULL) {
		fname = tmp;
	}
	strcat(fname, ".txt");
	graph->fname = fname;
	graph->fd = fopen(fname, "r+");
	if (graph->fd == NULL) {
		printf("\nÒàêîãî ôàéëà íå ñóùåñòâóåò.Áóäåò ñîçäàí íîâûé ãðàô\n");
		D_Create_Graph(graph);
		return;
	}
	graph->fname = fname;
	loadGraph(graph);
	puts("done");
}

void D_Add_Node(Graph* graph) {
	string nodeName;
	while (strcmp(nodeName = getStr(NULL, "%80[^ \n]"), "") != 0) {
		addNode(graph, nodeName);
		fprintf(graph->fd, "%d;%s;", 0, nodeName);
	}
	puts("done");
}

void D_Del_Node(Graph* graph) {
	string nodeName = getStr(NULL, "%80[^\n]");
	deleteNode(graph, nodeName);
	fprintf(graph->fd, "%d;%s;", 1, nodeName);
	puts("done");
}

void D_Add_Connection(Graph* graph) {
	string A, B;
	int maxFlow;
	while (getConnection(&A, &B, &maxFlow) > 0) {
		addConnection(graph, A, B, maxFlow);
		fprintf(graph->fd, "%d;%s;%s;%d;", 2, A, B, maxFlow);
	}
	puts("done");
}

Connection* findConnection(Node* node, string to, int maxFlow) {
	for (Connection* ptr = node->Adj; ptr && ptr->toNode; ptr = ptr->next) {
		if (strcmp(ptr->toNode->nodeName, to) == 0 && ptr->maxFlow == maxFlow) {
			return ptr;
		}
		else {
			return NULL;
		}
	}
}

void D_Del_Connection(Graph* graph) {
	string A, B;
	int maxFlow;
	while (getConnection(&A, &B, &maxFlow) > 0) {
		Node* prev, * cur;
		findNode(graph, A, &prev, &cur);
		deleteConnection(cur, B, maxFlow);
		fprintf(graph->fd, "%d;%s;%s;%d;", 2, A, B, maxFlow);
	}
	puts("done");
}

void D_Max_Product(Graph* graph) {
	Node* prev, * cur;
	findNode(graph, "s", &prev, &cur);
	int cc = 1, flow = 0, add;
	while ((add = dfs(cur, INF, cc)) > 0) {
		printf("(+ %d flow)\n", add);
		flow += add, ++cc;
	}
	printf("\nMax Flow: %d\n", flow);
	puts("done (using Ford–Fulkerson algorithm)");
}

void D_Print_Graph(Graph* graph) {
	for (Node* ptr = graph->nodes; ptr; ptr = ptr->next) {
		printf("%s ->|", ptr->nodeName);
		printConnections(ptr);
		puts("");
	}
	puts("done");
}

int getConnection(string* A, string* B, int* maxFlow) {
	*A = getStr(NULL, "%80[^ \n]");
	if (strcmp(*A, "") == 0) {
		return FALSE;
	}
	*B = getStr(NULL, "%80[^ \n]");
	int n = getInt(maxFlow);
	ignoreConsoleBuf();
	return TRUE;
}

Graph* makeGraph(void) {
	Graph* newGraph = (Graph*)malloc(sizeof(Graph));
	if (newGraph == NULL) {
		return NULL;
	}
	*newGraph = (Graph){ NULL, NULL, NULL };
	return newGraph;
}

void addNode(Graph* graph, string nodeName) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL) {
		return;
	}
	*newNode = (Node){ FALSE,  _strdup(nodeName), NULL, graph->nodes };
	graph->nodes = newNode;
}

char findNode(Graph* graph, string nodeName, Node** prev, Node** cur) {
	*prev = *cur = graph->nodes;
	for (; *cur; *cur = (*cur)->next) {
		if (strcmp((*cur)->nodeName ,nodeName) == 0) {
			return TRUE;
		}
		else {
			*prev = *cur;
		}
	}
	return FALSE;
}

void findPair(Graph* graph, string A, string B, Node** nodeA, Node** nodeB) {
	for (Node* ptr = graph->nodes; ptr; ptr = ptr->next) {
		if (strcmp(ptr->nodeName, A) == 0) {
			*nodeA = ptr;
		}
		if (strcmp(ptr->nodeName, B) == 0) {
			*nodeB = ptr;
		}
	}
}

int addConnection(Graph* graph, string A, string B, int maxFlow) {
	Node* nodeA, * nodeB;
	findPair(graph, A, B, &nodeA, &nodeB);
	Connection* newConnectionA = (Connection*)malloc(sizeof(Connection));
	if (newConnectionA == NULL) {
		return FALSE;
	}
	*newConnectionA = (Connection){ maxFlow, 0, nodeB, NULL, nodeA->Adj };
	nodeA->Adj = newConnectionA;
	Connection* newConnectionB = (Connection*)malloc(sizeof(Connection));
	if (newConnectionB == NULL) {
		return FALSE;
	}
	*newConnectionB = (Connection){ 0, 0, nodeA, NULL , nodeB->Adj };
	nodeB->Adj = newConnectionB;
	newConnectionA->tConnect = newConnectionB;
	newConnectionB->tConnect = newConnectionA;
	return TRUE;
}

void printConnections(Node* node) {
	for (Connection* ptr = node->Adj; ptr && ptr->toNode; ptr = ptr->next) {
		printf("(%s %d)|", ptr->toNode->nodeName, ptr->maxFlow);
	}
}

void deleteConnection(Graph* graph, Node* toNode, Connection* connection) {
	if (connection == NULL || toNode == NULL) return;
	if (connection == toNode->Adj) {
		if (connection->next == NULL) {
			free(toNode->Adj);
			toNode->Adj = NULL;
		}
		else if (connection->next->toNode == NULL) {
			free(connection->next);
			free(toNode->Adj);
			toNode->Adj = NULL;
		} 
		else {
			toNode->Adj = connection->next;
			free(connection);
		}
	}
	else {
		if (connection->next == NULL) {
			*connection = (Connection){ -1, -1, NULL, NULL, NULL };
		}
		else {
			Connection* toDelete = connection->next;
			*connection = *toDelete;
			free(toDelete);
		}
	}
}

void deleteNode(Graph* graph, string nodeName) {
	Node* prevNode, * curNode;
	findNode(graph, nodeName, &prevNode, &curNode);
	for (Connection* ptr = curNode->Adj; ptr && ptr->toNode; ptr = curNode->Adj) {
		deleteConnection(graph, ptr->toNode, ptr->tConnect);
		deleteConnection(graph, curNode, ptr);
	}
	if (curNode == graph->nodes) {
		graph->nodes = curNode->next;
	}
	else {
		prevNode->next = curNode->next;
	}
	free(curNode);
}

void loadGraph(Graph* graph) {
	int funcNumber;
	while (getIntNum(graph->fd, &funcNumber)) {
		switch (funcNumber) {
		case 0: {
			string nodeName = getStr(graph->fd, "%80[^;]");
			addNode(graph, nodeName);
		}
			  break;
		case 1: {
			string nodeName = getStr(graph->fd, "%80[^;]");
			deleteNode(graph, nodeName);
		}
			  break;
		case 2: {
			string A = getStr(graph->fd, "%80[^;]");
			string B = getStr(graph->fd, "%80[^;]");
			int maxFlow;
			getIntNum(graph->fd, &maxFlow);
			addConnection(graph, A, B, maxFlow);
		}
			  break;
		}
	}
}

int dfs(Node* node, int Cmin, int cc) {
	if (strcmp(node->nodeName, "t") == 0) {
		printf("[t] ");
		return Cmin;
	}
	node->used = cc;
	for (Connection* ptr = node->Adj; ptr && ptr->toNode; ptr = ptr->next) {
		if ((ptr->toNode->used != cc) && (ptr->flow < ptr->maxFlow)) {
			int delta = dfs(ptr->toNode, min(Cmin,ptr->maxFlow - ptr->flow), cc);
			if (delta > 0) {
				ptr->flow += delta;
				ptr->tConnect->flow = - ptr->flow;
				printf("<--(%d/%d;%d/%d)-- [%s] ",
					ptr->maxFlow, ptr->flow - delta, ptr->maxFlow, ptr->flow, node->nodeName);
				return delta;
			}
		}
	}
	return 0;
}
