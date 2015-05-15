/* �����µĴ洢ֵ��key - value��
*  1�������ظ���key
*  2��������������ʼ���½�һ����
*  3���ҵ�������ŵ�Ҷ�ӽ��
*  4����key����Ҷ�ӽ��
*  5�����Ҷ�ӽ�㲻Υ�棬��������
*  6�����Ҷ�ӽ��Υ�棬����Ҷ�ӽ��
*      7��������Ҷ�ӽ�㣬ԭҶ����Ϊ��Ҷ�ӣ��½�Ҷ��Ϊ��Ҷ�ӣ�����ͬһ�������
*      8����һ�루index��order/2����ȡ����ʼ����keys�Ͷ�Ӧ��pointers�Ƶ���Ҷ�ӽ��
*      9������Ҷ�ӽ����С��keyֵ���븸����ʵ���λ���У�ָ����Ҷ�ӽ�㣬����Ҷ�ӽ���С����������͸�����keyֵ
*      10�����3�����Ϊ�գ����½�һ��root��㣬����Ҷ�ӷֱ���Ϊroot���������ӽ�㣬�������
*      11����key���븸���
*      12���������㲻Υ�棬��������
*      13����������Υ�棬���Ѹ���㣨ע�⣬���ķ��Ѻ�Ҷ�ӵķ��Ѳ�ͬ����ΪҶ�ӵ��ӽ����Ŀ��key��Ŀһ�£������ӽ����Ŀ��key����Ŀ+1��
*          14�������µĽ�㣬ԭ�����Ϊ���㣬�½������Ϊ�ҽ�㣬����ͬһ�������
*          15����һ�루index��order/2����ȡ����ʼ����keys�Ͷ�Ӧ��pointers�Ƶ��½�㣬�½����Ϊ�����½���poniters��ָ���ӽ��ĸ���㣬�������С������������͸����keyֵ
*          16���ظ�������������keyֵ���븸����ʵ���λ���С�֮��Ĳ��� */
#include <stdlib.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif
#include "bptree.h"

/* ������root */
void insert_into_new_root(BTree * tree, Node * left, int const key, Node * right)
{
	Node * root = new_node();
	root->keys[0] = key;
	root->pointers[0] = left;
	root->pointers[1] = right;
	root->num_of_keys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;
	tree->root = root;
	tree->first_node = left;
}

/* ���м�����keyֵ */
void insert_into_node(BTree * tree, Node * node, int const key, Node * pointer)
{
	int i, j;
	for (i = 0; i < node->num_of_keys; i++) {
		if (key <= node->keys[i]) break;
	}
	if (key == node->keys[i])
		return;
	for (j = node->num_of_keys; j > i; j--)
	{
		node->keys[j] = node->keys[j - 1];
		node->pointers[j + 1] = node->pointers[j];
	}
	node->keys[i] = key;
	node->pointers[i + 1] = pointer;
	node->num_of_keys++;
}

/* ��Ҷ�ӽ���в���keyֵ */
void insert_into_leaf(Node * leaf, int const key, Record * const pointer)
{
	int i, insertion_point;
	insertion_point = 0;
	while (insertion_point < leaf->num_of_keys && leaf->keys[insertion_point] < key)
		insertion_point++;

	for (i = leaf->num_of_keys; i > insertion_point; i--) {
		leaf->keys[i] = leaf->keys[i - 1];
		leaf->pointers[i] = leaf->pointers[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pointers[insertion_point] = pointer;
	leaf->num_of_keys++;
}

/* �򸸽�����keyֵ */
void  insert_into_parent(BTree * tree, Node * left, int const key, Node * right)
{
	Node * parent = left->parent;
	/* 10����������Ϊ�գ����½�һ��root��㣬����Ҷ�ӷֱ���Ϊroot���������ӽ�㣬������� */
	if (parent == NULL) {
		insert_into_new_root(tree, left, key, right);
		return;
	}
	/* 11����key���븸��� */
	insert_into_node(tree, parent, key, right);
	/* 13����������Υ�棬���Ѹ���㣨ע�⣬���ķ��Ѻ�Ҷ�ӵķ��Ѳ�ͬ����ΪҶ�ӵ��ӽ����Ŀ��key��Ŀһ�£������ӽ����Ŀ��key����Ŀ+1�� */
	if (parent->num_of_keys > ORDER - 1)
		splitting_node(tree, parent);
	/* 12���������㲻Υ�棬�������� */
}

/* ���Ѹ���� */
void splitting_node(BTree * tree, Node * node)
{
	/* 14�������µĽ�㣬ԭ�����Ϊ���㣬�½������Ϊ�ҽ�㣬����ͬһ������� */
	Node * _new_node = new_node();
	/* 15����һ�루index��order/2����ȡ����ʼ����keys�Ͷ�Ӧ��pointers�Ƶ��½�㣬�½����Ϊ�����½���poniters��ָ���ӽ��ĸ���� */
	int middle = node->num_of_keys / 2, upkey = node->keys[middle - 1], keycount = node->num_of_keys;
	for (int i = middle, j = 0; i < keycount; i++, j++) {
		_new_node->keys[j] = node->keys[i];
		_new_node->pointers[j] = node->pointers[i];
		((Node *)node->pointers[i])->parent = _new_node;
		_new_node->num_of_keys++;
		node->num_of_keys--;
	}
	/* pointer���key��һ�� */
	_new_node->pointers[_new_node->num_of_keys] = node->pointers[keycount];
	((Node *)node->pointers[keycount])->parent = _new_node;
	_new_node->parent = node->parent;
	_new_node->prev = node;
	_new_node->next = node->next;
	node->next = _new_node;
	/* �������С������������͸����keyֵ */
	node->num_of_keys--;
	/* 16���ظ�������������keyֵ���븸����ʵ���λ���С�֮��Ĳ��� */
	insert_into_parent(tree, node, upkey, _new_node);
}

/* ����Ҷ�ӽ�� */
void splitting_leaf(BTree * tree, Node * leaf)
{
	/* 7��������Ҷ�ӽ�㣬ԭҶ����Ϊ��Ҷ�ӣ��½�Ҷ��Ϊ��Ҷ�ӣ�����ͬһ������� */
	Node * _new_leaf = new_leaf();
	/* 8����һ�루index��order/2����ȡ����ʼ����keys�Ͷ�Ӧ��pointers�Ƶ���Ҷ�ӽ�� */
	int middle = leaf->num_of_keys / 2, keycount = leaf->num_of_keys;
	/* ����Ҷ�ӽ���б�����keyֵ */
	for (int i = middle, j = 0; i < keycount; i++, j++) {
		_new_leaf->keys[j] = leaf->keys[i];
		_new_leaf->pointers[j] = leaf->pointers[i];
		_new_leaf->num_of_keys++;
		leaf->num_of_keys--;
	}
	_new_leaf->parent = leaf->parent;
	_new_leaf->prev = leaf;
	_new_leaf->next = leaf->next;
	leaf->next = _new_leaf;
	/* 9������Ҷ�ӽ����С��keyֵ���븸����ʵ���λ���У�ָ����Ҷ�ӽ�� */
	insert_into_parent(tree, leaf, _new_leaf->keys[0], _new_leaf);
}

/* ���� */
BTree * insert(BTree * tree, int const key, char * const value)
{
	/* 1�������ظ���key */
	if (search_record(tree, key) != NULL)
		return tree;
	Record * pointer = new_record(value);
	/* 2��������������ʼ���½�һ���� */
	if (tree == NULL) {
		tree = new_tree(key, pointer);
		return tree;
	}
	/* 3���ҵ�������ŵ�Ҷ�ӽ�� */
	Node * leaf = search_leaf(tree, key);
	/* 4����key����Ҷ�ӽ�� */
	insert_into_leaf(leaf, key, pointer);
	/* 6�����Ҷ�ӽ��Υ�棬����Ҷ�ӽ�� */
	if (leaf->num_of_keys > ORDER - 1) {
		splitting_leaf(tree, leaf);
	}
	/* 5�����Ҷ�ӽ�㲻Υ�棬�������� */
	return tree;
}