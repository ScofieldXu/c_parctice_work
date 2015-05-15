#include <stdlib.h>
#include <assert.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif

/* 初始化新建一条记录 */
Record * new_record(char * const value)
{
	Record * record = (Record *)malloc(sizeof(Record));
	assert(record != NULL);
	record->value = value;
	return record;
}

/* 初始化新建一个节点 */
Node * new_node(void)
{
	Node * node = (Node *)malloc(sizeof(Node));
	assert(node != NULL);
	node->pointers = (void **)malloc((ORDER + 1) * sizeof(void *));	//多占一位用于先插入后分裂，实际指针长度为ORDER
	assert(node->pointers != NULL);
	node->keys = (int *)malloc(ORDER * sizeof(int));				//多占一位用于先插入后分裂，实际key长度为ORDER - 1
	assert(node->keys != NULL);
	node->parent = NULL;
	node->num_of_keys = 0;
	node->is_leaf = false;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

/* 初始化新建一个叶子节点 */
Node * new_leaf(void)
{
	Node * leaf = new_node();
	leaf->is_leaf = true;
	return leaf;
}

/* 初始化新建一颗B+树 */
BTree * new_tree(int const key, Record * const pointer)
{
	BTree * tree = (BTree *)malloc(sizeof(BTree));
	assert(tree != NULL);
	tree->root = new_leaf();
	tree->root->keys[0] = key;
	tree->root->pointers[0] = pointer;
	tree->root->num_of_keys++;
	tree->first_node = tree->root;
	return tree;
}

/* 释放一个节点 */
void free_node(Node * node)
{
	free(node->keys);
	free(node->pointers);
	free(node);
}

/* 释放一棵树 */
void free_tree(BTree * tree)
{
	free(tree->root->keys);
	free(tree->root->pointers);
	free(tree->root);
	free(tree);
}