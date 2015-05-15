#include <stdlib.h>
#include <assert.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif

/* ��ʼ���½�һ����¼ */
Record * new_record(char * const value)
{
	Record * record = (Record *)malloc(sizeof(Record));
	assert(record != NULL);
	record->value = value;
	return record;
}

/* ��ʼ���½�һ���ڵ� */
Node * new_node(void)
{
	Node * node = (Node *)malloc(sizeof(Node));
	assert(node != NULL);
	node->pointers = (void **)malloc((ORDER + 1) * sizeof(void *));	//��ռһλ�����Ȳ������ѣ�ʵ��ָ�볤��ΪORDER
	assert(node->pointers != NULL);
	node->keys = (int *)malloc(ORDER * sizeof(int));				//��ռһλ�����Ȳ������ѣ�ʵ��key����ΪORDER - 1
	assert(node->keys != NULL);
	node->parent = NULL;
	node->num_of_keys = 0;
	node->is_leaf = false;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

/* ��ʼ���½�һ��Ҷ�ӽڵ� */
Node * new_leaf(void)
{
	Node * leaf = new_node();
	leaf->is_leaf = true;
	return leaf;
}

/* ��ʼ���½�һ��B+�� */
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

/* �ͷ�һ���ڵ� */
void free_node(Node * node)
{
	free(node->keys);
	free(node->pointers);
	free(node);
}

/* �ͷ�һ���� */
void free_tree(BTree * tree)
{
	free(tree->root->keys);
	free(tree->root->pointers);
	free(tree->root);
	free(tree);
}