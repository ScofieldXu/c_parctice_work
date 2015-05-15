#include <stdlib.h>
#include <stdio.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif
#include "bptree.h"

/* 计算子节点深度 */
int path_to_root(Node * root, Node * child) 
{
	int length = 0;
	Node * c = child;
	while (c != root) {
		c = c->parent;
		length++;
	}
	return length;
}

/* 打印出B+树 */
void print_tree(BTree * tree) 
{
	Node * n = new_node();
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (tree == NULL || tree->root == NULL) {
		printf("Empty tree.\n");
		return;
	}

	queue * q = init_queue(1000);
	enqueue(q, tree->root);
	while (q->size > 0) {
		n = dequeue(q);
		if (n == NULL)
			break;
		if (n->parent != NULL && n == n->parent->pointers[0]) {
			new_rank = path_to_root(tree->root, n);
			if (new_rank != rank) {
				rank = new_rank;
				printf("\n");
			}
		}
		for (i = 0; i < n->num_of_keys; i++) {
			printf("%d ", n->keys[i]);
		}
		if (!n->is_leaf)
			for (i = 0; i <= n->num_of_keys; i++)
				enqueue(q, n->pointers[i]);
		printf("| ");
	}
	printf("\n");
}

/* 二分之一length向上取整 */
int ceil(int const length) 
{
	if (length % 2 == 0)
		return length / 2;
	else
		return length / 2 + 1;
}

/* 二分之一length向下取整 */
int floor(int const length) 
{
	return length / 2;
}
