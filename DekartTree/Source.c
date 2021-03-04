#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef struct Node {
	int x, y;
	struct Node* left, * right;
} Node;

typedef struct Pair {
	struct Node* ptrLeft;
	struct Node* ptrRight;
} Pair;

Node* createNode(int val) {
	Node* newNode = malloc(sizeof(Node));
	*newNode = (Node){ val, rand(), NULL, NULL };
}

Node* deleteNode(Node** node) {
	free((*node)->left);
	free((*node)->right);
	free(*node);
}

char exists(Node* root, int key) {
	if (root == NULL) {
		return FALSE;
	}
	if (key == root->x) {
		return TRUE;
	}
	if (key > root->x) {
		return exists(root->right, key);
	}
	return exists(root->left, key);
}

Pair split(Node* root, int val) {
	if (root == NULL) {
		return (Pair) { NULL, NULL };
	}
	if (root->x <= val) {
		Pair res = split(root->right, val);
		root->right = res.ptrLeft;
		return (Pair) { root, res.ptrRight };
	}
	else {
		Pair res = split(root->left, val);
		root->left = res.ptrRight;
		return (Pair) {res.ptrLeft, root};
	}
}

Node* merge(Node* root1, Node* root2) {
	if (root1 == NULL) {
		return root2;
	}
	if (root2 == NULL) {
		return root1;
	}
	if (root1->y <= root2->y) {
		root1->right = merge(root1->right, root2);
		return root1;
	}
	else {
		root2->left = merge(root1, root2->left);
		return root2;
	}
}

Node* insert(Node* root, int val) {
	if (exists(root, val)) {
		return root;
	}
	Pair res = split(root, val);
	Node* newNode = createNode(val);
	return merge(merge(res.ptrLeft, newNode), res.ptrRight);
}

Node* erase(Node* root, int val) {
	if (!exists(root, val)) {
		return root;
	}
	Pair res1 = split(root, val);
	Pair res2 = split(res1.ptrLeft, val - 1);
	free(res2.ptrRight);
	return merge(res2.ptrLeft, res1.ptrRight);
}

void print(Node* root) {
	if (root == NULL) {
		return;
	}
	print(root->left);
	printf("%d ", root->x);
	print(root->right);
}

int depth(Node* root) {
	if (root == NULL) {
		return 0;
	}
	return 1 + max(depth(root->left), depth(root->right));
}

int main() {
	Node* root = NULL;
	for (int i = 1; i <= 10000; ++i) {
		root = insert(root, i);
		//insert может менять корень дерева
	}
	printf("%d", depth(root));
	return 0;
}