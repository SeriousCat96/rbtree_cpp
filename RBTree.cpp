#include <iostream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

using namespace std;

enum node_color { BLACK, RED };

typedef int T;

struct node {
	struct node* parent;
	struct node* left;
	struct node* right;
	enum node_color color;
	T data;
};

#define NIL &leaf           /* лист дерева */
struct node leaf = { NIL, NIL, 0, BLACK, 0 };

// Корень дерева
node* root = NIL;

/// <summary>
/// Левый поворот дерева
/// </summary>
/// <param name="node">Узел дерева</param>
void rotateLeft(node* x) 
{
	node* y = x->right;

	/* establish x->right link */
	x->right = y->left;

	if (y->left != NIL)
	{
		y->left->parent = x;
	}

	/* establish y->parent link */
	if (y != NIL)
	{
		y->parent = x->parent;
	}
	if (x->parent) 
	{
		if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
	}
	else 
	{
		root = y;
	}

	/* link x and y */
	y->left = x;

	if (x != NIL)
	{
		x->parent = y;
	}
}

/// <summary>
/// Правый поворот дерева
/// </summary>
/// <param name="node">Узел дерева</param>
void rotateRight(node* x) 
{
	node* y = x->left;

	/* establish x->left link */
	x->left = y->right;

	if (y->right != NIL)
	{
		y->right->parent = x;
	}

	/* establish y->parent link */
	if (y != NIL)
	{
		y->parent = x->parent;
	}
	if (x->parent) 
	{
		if (x == x->parent->right)
		{
			x->parent->right = y;
		}
		else
		{
			x->parent->left = y;
		}
	}
	else 
	{
		root = y;
	}

	/* link x and y */
	y->right = x;
	if (x != NIL)
	{
		x->parent = y;
	}
}

/// <summary>
/// Балансирование дерева после вставки узла
/// </summary>
/// <param name="x">Узел дерева</param>
void insertBalance(node* x) 
{
	 /* Проверяем свойства дерева */
	while (x != root && x->parent->color == RED) 
	{
		if (x->parent == x->parent->parent->left) 
		{
			node* y = x->parent->parent->right;

			if (y->color == RED) 
			{

				/* "дядя" красный */
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			}
			else 
			{
				/* "дядя" чёрный */
				if (x == x->parent->right) 
				{
					/* x становится левый потомком */
					x = x->parent;
					rotateLeft(x);
				}

				/* "перекрашиваем" и "поворачиваем" */
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotateRight(x->parent->parent);
			}
		}
		else 
		{
			node* y = x->parent->parent->left;

			if (y->color == RED) 
			{

				/* "дядя" красный */
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			}
			else {

				/* "дядя" чёрный */
				if (x == x->parent->left) 
				{
					x = x->parent;
					rotateRight(x);
				}

				/* "перекрашиваем" и "поворачиваем" */
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rotateLeft(x->parent->parent);
			}
		}
	}

	root->color = BLACK;
}

node* insertNode(T data) 
{
	node* current, * parent, * x;
	current = root;
	parent = 0;

	while (current != NIL) 
	{
		if (data == current->data)
		{
			return current;
		}

		parent = current;
		current = data < current->data 
			? current->left 
			: current->right;
	}

	/* setup new node */
	x = new node();
	x->data = data;
	x->parent = parent;
	x->left = NIL;
	x->right = NIL;
	x->color = RED;

	/* insert node in tree */
	if (parent) 
	{
		if (data < parent->data)
		{
			parent->left = x;
		}
		else
		{
			parent->right = x;
		}
	}
	else 
	{
		root = x;
	}

	insertBalance(x);
	return(x);
}

/// <summary>
/// балансирование дерева после удаления узла
/// </summary>
/// <param name="x">Узел дерева</param>
void deleteBalance(node* x) 
{
	while (x != root && x->color == BLACK) 
	{
		if (x == x->parent->left) 
		{
			node* w = x->parent->right;

			if (w->color == RED) 
			{
				w->color = BLACK;
				x->parent->color = RED;
				rotateLeft(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) 
			{
				w->color = RED;
				x = x->parent;
			}
			else 
			{
				if (w->right->color == BLACK) 
				{
					w->left->color = BLACK;
					w->color = RED;
					rotateRight(w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;

				rotateLeft(x->parent);

				x = root;
			}
		}
		else 
		{
			node* w = x->parent->left;

			if (w->color == RED) 
			{
				w->color = BLACK;
				x->parent->color = RED;

				rotateRight(x->parent);

				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) 
			{
				w->color = RED;
				x = x->parent;
			}
			else 
			{
				if (w->left->color == BLACK) 
				{
					w->right->color = BLACK;
					w->color = RED;

					rotateLeft(w);

					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;

				rotateRight(x->parent);

				x = root;
			}
		}
	}

	x->color = BLACK;
}

/// <summary>
/// Удаляет узел из дерева
/// </summary>
/// <param name="z">Узел дерева</param>
void deleteNode(node* z) 
{
	node* x, * y;

	if (!z || z == NIL)
	{
		return;
	}

	if (z->left == NIL || z->right == NIL) 
	{
		y = z;
	}
	else 
	{
		y = z->right;

		while (y->left != NIL)
		{
			y = y->left;
		}
	}

	if (y->left != NIL)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}

	x->parent = y->parent;

	if (y->parent)
	{
		if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}
	else
	{
		root = x;
	}

	if (y != z)
	{
		z->data = y->data;
	}

	if (y->color == BLACK)
	{
		deleteBalance(x);
	}

	free(y);
}

node* findNode(T data) 
{
	node* current = root;

	while (current != NIL)
	{
		if (data == current->data)
		{
			return current;
		}
		else
		{
			current = data < current->data 
				? current->left 
				: current->right;
		}
	}

	return 0;
}

node* maximum(node* node) 
{
	while (node->right != NIL) 
	{
		node = node->right;
	}

	return node;
}

node* minimum(node* node)
{
	while (node->left != NIL)
	{
		node = node->left;
	}

	return node;
}

void printHelper(node* root, string indent, bool last) {
	// print the tree structure on the screen
	if (root != NIL) {
		cout << indent;
		if (last) {
			cout << "R----";
			indent += "     ";
		}
		else {
			cout << "L----";
			indent += "|    ";
		}

		string sColor = root->color ? "RED" : "BLACK";
		cout << root->data << "(" << sColor << ")" << endl;
		printHelper(root->left, indent, false);
		printHelper(root->right, indent, true);
	}
}

void prettyPrint(node* root)
{
	if (root)
	{
		printHelper(root, "", true);
	}
}

int main() 
{
	int a, maxnum = 200, ct;
	node* t;

	srand(time(NULL));

	for (ct = maxnum; ct; ct--) 
	{
		a = rand() % 999 + 1;

		if ((t = findNode(a)) != NULL) 
		{
			deleteNode(t);
		}
		else 
		{
			insertNode(a);
		}
	}

	prettyPrint(root);

	node* max = maximum(root);
	node* min = minimum(root);

	cout << "Max element is: " << max->data << endl;
	cout << "Min element is: " << min->data << endl;
}
