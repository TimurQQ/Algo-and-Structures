#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define R 0
#define B 1


typedef struct Node {
	char color;
	int key;
	struct Node* left, * right, * parent;
}Node;

Node* findParent(Node* tree, int key) {
	Node* parent = NULL, * cur = tree;
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

void leftRotate(Node** tree, Node* x) {
	Node* y = x->right;
	Node* p = x->parent;
	x->right = y->left;
	if (y->left != NULL) {
		y->left->parent = x;
	}
	y->parent = p;
	if (p == NULL) {
		*tree = y;
	}
	else {
		if (p->left == x) {
			p->left = y;
		}
		else {
			p->right = y;
		}
	}
	y->left = x;
	x->parent = y;
}

void rightRotate(Node** tree, Node* x) {
	Node* y = x->left;
	Node* p = x->parent;
	x->left = y->right;
	if (y->right != NULL) {
		y->right->parent = x;
	}
	y->parent = p;
	if (p == NULL) {
		*tree = y;
	}
	else {
		if (p->right == x) {
			p->right = y;
		}
		else {
			p->left = y;
		}
	}
	y->right = x;
	x->parent = y;
}

void RB_Insert_Fixup(Node** tree, Node* x) {
	Node* p1 = x->parent;
	while (p1 != NULL && p1->color == R) {
		p1 = x->parent;
		Node* pp = p1->parent;
		Node* p2 = NULL;
		if (p1 == pp->left) {
			p2 = pp->right;
			if (p2 != NULL && p2->color == R) {
				//Случай 1
				p1->color = B;
				p2->color = B;
				pp->color = R;
				x = pp;
				continue;
			}
			else  if (x == p1->right){
				//Случай 2 -> Случай 3
				x = p1;
				leftRotate(tree, x);
				p1 = x->parent;
			}
			//Случай 3
			p1->color = B;
			pp->color = R;
			rightRotate(tree ,pp);
		}
		else {
			p2 = pp->left;
			if (p2 != NULL && p2->color == R) {
				//Случай 1
				p1->color = B;
				p2->color = B;
				pp->color = R;
				x = pp;
				continue;
			}
			else  if (x == p1->left) {
				//Случай 2 -> Случай 3
				x = p1;
				rightRotate(tree, x);
				p1 = x->parent;
			}
			//Случай 3
			p1->color = B;
			pp->color = R;
			leftRotate(tree, pp);
		}
	}
}

void addNode(Node** tree, Node* x) {
	if (*tree == NULL) {
		*tree = x;
		x->color = B;
	}
	else {
		Node* parent = findParent(*tree, x->key);
		if (x->key < parent->key) {
			parent->left = x;
		}
		else {
			parent->right = x;
		}
		x->parent = parent;
		x->color = R;
		RB_Insert_Fixup(tree, x);
	}
	(*tree)->color = B;
}

Node* findMin(Node* x) {
	Node* cur = x;
	while (cur->left != NULL) {
		cur = cur->left;
	}
	return cur;
}

Node* findRealDel(Node* x) {
	if (x->right != NULL) {
		return findMin(x->right);
	}
	else {
		while (x->parent != NULL && x->parent->right == x) {
			x = x->parent;
		}
		return x->parent;
	}
}

void RB_Delete_Fixup(Node** tree, Node* x) {
	while (x != *tree && x->color == B) {
		//Пока дважды-чёрный
		Node* p = x->parent;
		if (x == p->left) {
			Node* w = p->right;
			if (w != NULL && w->color == R) {
				w->color = p->color;
				p->color = R;
				leftRotate(tree, p);
				w = p->right;
			}
			if (w != NULL) {
				if (w->right == NULL || w->right->color == B &&
					w->left == NULL || w->left->color == B) {
					//2 случай
					w->color = R;
					x = p;
				}
				else {
					if (w->right == NULL || w->right->color == B) {
						//Случай 3
						w->color = R;
						w->left->color = B;
						rightRotate(tree, w);
						w = p->right;
					}
					//Случай 4
					w->color = p->color;
					p->color = B;
					if (w->right != NULL) {
						w->right->color = B;
					}
					leftRotate(tree, p);
					x = *tree;
				}
			}
		}
		else {
			Node* p = x->parent;
			if (x == p->right) {
				Node* w = p->left;
				if (w != NULL && w->color == R) {
					w->color = p->color;
					p->color = R;
					rightRotate(tree, p);
					w = p->left;
				}
				if (w != NULL) {
					if (w->left == NULL || w->left->color == B &&
						w->right == NULL || w->right->color == B) {
						//2 случай
						w->color = R;
						x = p;
					}
					else {
						if (w->left == NULL || w->left->color == B) {
							//Случай 3
							w->color = R;
							w->right->color = B;
							rightRotate(tree, w);
							w = p->left;
						}
						//Случай 4
						w->color = p->color;
						p->color = B;
						if (w->left != NULL) {
							w->left->color = B;
						}
						rightRotate(tree, p);
						x = *tree;
					}
				}
			}
		}
	}
	x->color = B;
}

void RB_Delete(Node** tree, Node* x) {
	Node* y = NULL, * subTree = NULL;
	if (x->left == NULL || x->right == NULL) {
		y = x;
	}
	else {
		y = findRealDel(x);
	}
	if (y->left != NULL) {
		subTree = y->left;
	}
	else {
		subTree = y->right;
	}
	if (subTree != NULL) {
		subTree->parent = y->parent;
	}
	if (y->parent == NULL) {
		*tree = subTree;
	}
	else {
		if (y->parent->left == y) {
			y->parent->left = subTree;
		}
		else {
			y->parent->right = subTree;
		}
	}
	if (y != x) {
		x->key = y->key;
	}
	if (y->color == B) {
		RB_Delete_Fixup(tree, subTree);
	}
}

int main(void) {
	Node* tree = NULL;
	const int keys[5] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
		Node* newNode = malloc(sizeof(Node));
		*newNode = (Node){ R, keys[i], NULL, NULL, NULL };
		addNode(&tree, newNode);
	}
	printf("HUI");
	return 0;
}