#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct Node {
	int key;
	struct Node* left, * right, * parent;
} Node;

typedef Node* bSTree;

Node* createTree() {
	Node* newTree = NULL;
	return newTree;
}

Node* findParent(Node* tree, int key) {
	Node* cur = tree, * parent = NULL;
	while (cur != NULL) {
		parent = cur;
		if (key < cur->key) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}
	return parent;
}

void addNode(Node** tree, int key) {
	Node* newNode = malloc(sizeof(Node));
	*newNode = (Node){ key, NULL, NULL, NULL };
	if (*tree == NULL) {
		*tree = newNode;
	}
	else {
		Node* ptr = findParent(*tree, key);
		if (key < ptr->key) {
			newNode->parent = ptr;
			ptr->left = newNode;
		}
		else {
			newNode->parent = ptr;
			ptr->right = newNode;
		}
	}
}

Node* find(Node* tree, int key) {
	Node* cur = tree;
	while (cur != NULL) {
		if (key == cur->key) {
			return cur;
		}
		else  if (key < cur->key){
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}
	return NULL;
}

Node* findMin(Node* subTree) {
	Node* cur = subTree;
	while (cur->left != NULL) {
		cur = cur->left;
	}
	return cur;
}

Node* findRealDelElem(Node* tree, Node* delElem) {
	if (delElem->right != NULL) {
		return findMin(delElem->right);
	}
	else {
		Node* parent = delElem->parent;
		while (parent != NULL && parent->right == delElem) {
			delElem = parent;
			parent = delElem->parent;
		}
		return parent;
	}

}
 
void deleteNode(Node** tree, int key) {
	Node* ptrElem = find(*tree, key);
	Node* ptrDel;
	if (ptrElem->left == NULL || ptrElem->right == NULL) {
		ptrDel = ptrElem;
	}
	else ptrDel = findRealDelElem(*tree, ptrElem);
	Node* subTree = NULL;
	if (ptrDel->left != NULL) {
		subTree = ptrDel->left;
	}
	else {
		subTree = ptrDel->right;
	}
	if (subTree != NULL) {
		subTree->parent = ptrDel->parent;
	}
	if (ptrDel->parent == NULL) {
		tree = subTree;
	}
	else {
		if (ptrDel->parent->left == ptrDel) {
			ptrDel->parent->left = subTree;
		}
		else {
			ptrDel->parent->right = subTree;
		}
	}
	if (ptrElem != ptrDel) {
		ptrElem->key = ptrDel->key;
	}
}

int main(void) {
	Node* bsTree = createTree();
	const int keys[14] = { 
		20, 10, 26, 3, 16, 22, 28,
		14, 18, 24, 11, 23, 25, 12
	};
	for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
		addNode(&bsTree, keys[i]);
	}
	deleteNode(&bsTree, 22);
	deleteNode(&bsTree, 10);
	deleteNode(&bsTree, 20);
	printf("HUY");
	return 0;
}