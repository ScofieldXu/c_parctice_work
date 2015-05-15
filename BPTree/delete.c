/* 1�����Բ����ڵ�key
*  2���ҵ�������ŵ�Ҷ�ӽڵ�
*  3��ɾ��Ҷ�ӽڵ��е�key
*  4�����Ҷ�ӽڵ㲻Υ�棬��ɾ������
*  5�����Ҷ�ӽڵ�Υ�棬����Ҷ�ӽڵ�
*      6�������root�ڵ㣬��ɾ������
*      7�����Ҷ�ӽڵ������ֵܣ�����Ҷ�ӽڵ�����ֵܵ�keys�ܺ�
*          8������ܺʹ��ڵ���2���ģ�order/2����ȡ�����������ֵܽڵ�index��order/2����ȡ����ʼ��keys�Ͷ�Ӧ��pointers������Ҷ�ӽڵ�
*          9����������key����С��keyֵ�滻���ڵ��Ӧ��keyֵ������Ҷ�ӽڵ㶼��Υ�棬ɾ������
*          10�����򣬽����ֵܺ͵�ǰҶ�Ӻϲ���ɾ�����ֵܽڵ��ڸ��ڵ��е�key
*          11��������ڵ㲻Υ�棬��ɾ������
*          12��������ڵ�Υ�棬�����ڵ�
*              13�������root�ڵ㣬���key��Ŀ=0��ɾ��root�ڵ㣬�ϲ�root�ڵ���ӽڵ��Ϊ�µ�root�ڵ㣬���key��Ŀ����0��ɾ������
*              14������ڵ������ֵܣ�����ڵ�����ֵܵ�keys�ܺ�
*                  15������ܺʹ��ڵ���2����order/2����ȡ��-1���������ֵܽڵ�index��order/2����ȡ��-1��ʼ��keys��index��order/2����ȡ����ʼ��poninter������ԭ�ڵ�
*                  16�������key����С��keyֵ�븸�ڵ��Ӧ��keyֵ�����������ڵ㶼��Υ�棬ɾ������
*                  17�������ظ��������ֵܺ͵�ǰ�ڵ�ϲ���ɾ����ڵ��ڸ��ڵ��е�key��֮��Ĳ���
*              18������ڵ�ֻ�����ֵܣ�����ڵ�����ֵܵ�keys�ܺ�
*                  19������ܺʹ��ڵ���2����order/2����ȡ��-1���������ֵܽڵ�index��0��order/2����ȡ��-2��ʼ��keys��index��0��order/2����ȡ��-1��ʼ��poninter������ԭ�ڵ�
*                  20�������key������keyֵ�븸�ڵ��Ӧ��keyֵ�����������ڵ㶼��Υ�棬ɾ������
*                  21�������ظ��������ֵܺ͵�ǰ�ڵ�ϲ���ɾ����ڵ��ڸ��ڵ��е�key��֮��Ĳ���
*      22�����Ҷ�ӽڵ�ֻ�����ֵܣ�����Ҷ�ӽڵ�����ֵܵ�keys�ܺ�
*          23������ܺʹ��ڵ���2���ģ�order/2����ȡ�����������ֵܽڵ�index��0��order/2����ȡ��-2��keys�Ͷ�Ӧ��pointers������Ҷ�ӽڵ�
*          24�������ֵ������µ�key����С��key�滻���ڵ��ж�Ӧ��keyֵ������Ҷ�ӽڵ㶼��Υ�棬ɾ������
*          25�����򣬽����ֵܺ͵�ǰҶ�Ӻϲ���ɾ��Ҷ�ӽڵ��ڸ��ڵ��е�key
*          26��������ڵ㲻Υ�棬��ɾ������
*          27��������ڵ�Υ�棬�����ڵ�
*              28���ظ�12 */
#include <stdlib.h>
#include <assert.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif
#include "bptree.h"

/* �������ֵ��ڸ��ڵ��е���ţ�-1��ʾû�����ֵ� */
int get_left_sibling_index(Node * node)
{
	for (int i = 0; i <= node->parent->num_of_keys; i++)
		if (node->parent->pointers[i] == node)
			return i - 1;
	return -1;
}

/* ɾ��Ҷ�ӽڵ��е�key */
void delete_from_leaf(Node * leaf, int const key)
{
	int i = 0;
	/* ɾ��key��������key��ǰ���� */
	i = 0;
	while (leaf->keys[i] != key)
		i++;
	for (++i; i < leaf->num_of_keys; i++) {
		leaf->keys[i - 1] = leaf->keys[i];
		leaf->pointers[i - 1] = leaf->pointers[i];
	}
	leaf->num_of_keys--;
}

/* �ƶ�Ҷ�ӽڵ��е�key */
void borrow_from_left_leaf(BTree * tree, int const index, int const parent_index, Node * d_leaf)
{
	Node * sibling = (Node *)d_leaf->parent->pointers[parent_index];
	int num_of_borrow = sibling->num_of_keys - index;
	int upkey = sibling->keys[index];
	for (int i = d_leaf->num_of_keys - 1; i >= 0; i--) {
		d_leaf->keys[i + num_of_borrow] = d_leaf->keys[i];
		d_leaf->pointers[i + num_of_borrow] = d_leaf->pointers[i];
	}
	for (int i = 0, j = index; j < sibling->num_of_keys; i++, j++) {
		d_leaf->keys[i] = sibling->keys[j];
		d_leaf->pointers[i] = sibling->pointers[j];
		d_leaf->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* 9����������key����С��keyֵ�滻���ڵ��Ӧ��keyֵ������Ҷ�ӽڵ㶼��Υ�棬ɾ������ */
	d_leaf->parent->keys[parent_index] = upkey;
}

void borrow_from_right_leaf(BTree * tree, int const index, int const parent_index, Node * d_leaf)
{
	Node * sibling = (Node *)d_leaf->parent->pointers[parent_index];
	int num_of_borrow = index + 1;
	int upkey = sibling->keys[index + 1];
	for (int i = d_leaf->num_of_keys, j = 0; j < num_of_borrow; i++, j++) {
		d_leaf->keys[i] = sibling->keys[j];
		d_leaf->pointers[i] = sibling->pointers[j];
		d_leaf->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* 24�������ֵ������µ�key����С��key�滻���ڵ��ж�Ӧ��keyֵ������Ҷ�ӽڵ㶼��Υ�棬ɾ������ */
	d_leaf->parent->keys[parent_index] = upkey;
}

/* �ϲ�Ҷ�ӽڵ� */
void merge_left_leaf(BTree * tree, int const parent_index, Node * leaf)
{
	Node * sibling = (Node *)leaf->parent->pointers[parent_index];
	int num_of_borrow = sibling->num_of_keys;
	for (int i = leaf->num_of_keys - 1; i >= 0; i--) {
		leaf->keys[i + num_of_borrow] = leaf->keys[i];
		leaf->pointers[i + num_of_borrow] = leaf->pointers[i];
	}
	for (int i = 0, j = 0; i < num_of_borrow; i++, j++) {
		leaf->keys[i] = sibling->keys[j];
		leaf->pointers[i] = sibling->pointers[j];
		leaf->num_of_keys++;
		sibling->num_of_keys--;
	}
	Node * parent = leaf->parent;
	for (int i = parent_index; i < parent->num_of_keys; i++) {
		parent->keys[i] = parent->keys[i + 1];
		parent->pointers[i] = parent->pointers[i + 1];
	}
	parent->pointers[parent->num_of_keys] = parent->pointers[parent->num_of_keys + 1];
	parent->num_of_keys--;
	leaf->prev = sibling->prev;
	if (sibling->prev != NULL)
		sibling->prev->next = leaf;
	free_node(sibling);
}

void merge_right_leaf(BTree * tree, int const parent_index, Node * leaf)
{
	/* ���ֵ�ָ�룬ֻ������ߵ�Ҷ�Ӳ�û�����ֵܣ��������ֵ��ڸ��ڵ��ָ��ӦΪ1*/
	assert(parent_index == 1);
	Node * sibling = (Node *)leaf->parent->pointers[parent_index];
	/* �ϲ����ڽ������keys */
	int num_of_borrow = sibling->num_of_keys;
	for (int i = leaf->num_of_keys, j = 0; j < num_of_borrow; i++, j++) {
		leaf->keys[i] = sibling->keys[j];
		leaf->pointers[i] = sibling->pointers[j];
		leaf->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* ���ڵ� */
	Node * parent = leaf->parent;
	for (int i = parent_index; i < parent->num_of_keys; i++) {
		parent->keys[i] = parent->keys[i + 1];
		parent->pointers[i] = parent->pointers[i + 1];
	}
	parent->pointers[parent->num_of_keys] = parent->pointers[parent->num_of_keys + 1];
	parent->num_of_keys--;
	free_node(sibling);
}

/* �ƶ��ڵ��е�key */
void borrow_from_left_node(BTree * tree, int const index, int const parent_index, Node * d_node)
{
	Node * sibling = (Node *)d_node->parent->pointers[parent_index];
	int num_of_borrow = sibling->num_of_keys - index;
	int upkey = sibling->keys[index];
	/* ��num_of_borrow���ٴӸ��ڵ�����һ��key��һ��Ҫ�ճ�num_of_borrow+1��λ�� */
	d_node->pointers[d_node->num_of_keys + num_of_borrow + 1] = d_node->pointers[d_node->num_of_keys];
	for (int i = d_node->num_of_keys - 1; i >= 0; i--) {
		d_node->keys[i + num_of_borrow + 1] = d_node->keys[i];
		d_node->pointers[i + num_of_borrow + 1] = d_node->pointers[i];
	}
	d_node->keys[num_of_borrow] = d_node->parent->keys[parent_index];
	d_node->pointers[num_of_borrow] = sibling->pointers[sibling->num_of_keys];
	d_node->num_of_keys++;
	for (int i = 0, j = index; j < sibling->num_of_keys; i++, j++) {
		d_node->keys[i] = sibling->keys[j];
		d_node->pointers[i] = sibling->pointers[j + 1];
		d_node->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* 16�������key����С��keyֵ�븸�ڵ��Ӧ��keyֵ�����������ڵ㶼��Υ�棬ɾ������ */
	d_node->parent->keys[parent_index] = upkey;
}

void borrow_from_right_node(BTree * tree, int const index, int const parent_index, Node * d_node)
{
	Node * sibling = (Node *)d_node->parent->pointers[parent_index];
	int num_of_borrow = index + 1;
	int upkey = sibling->keys[index];
	/* ��num_of_borrow���ٴӸ��ڵ�����һ��key��һ��Ҫ�ճ�num_of_borrow+1��λ�� */
	d_node->keys[d_node->num_of_keys] = d_node->parent->keys[parent_index];
	for (int i = d_node->num_of_keys + 1, j = 0; j <= index; i++, j++) {
		d_node->keys[i] = sibling->keys[j];
		d_node->pointers[i] = sibling->pointers[j];
		d_node->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* 20�������key������keyֵ�븸�ڵ��Ӧ��keyֵ�����������ڵ㶼��Υ�棬ɾ������ */
	d_node->parent->keys[parent_index] = upkey;
}

/* �ϲ��ڵ� */
void merge_left_node(BTree * tree, int const parent_index, Node * node)
{
	Node * sibling = (Node *)node->parent->pointers[parent_index];
	int downkey = node->parent->keys[parent_index];
	int num_of_borrow = sibling->num_of_keys;
	node->pointers[node->num_of_keys + 1 + num_of_borrow] = node->pointers[node->num_of_keys];
	for (int i = node->num_of_keys - 1; i >= 0; i--) {
		node->keys[i + num_of_borrow + 1] = node->keys[i];
		node->pointers[i + num_of_borrow + 1] = node->pointers[i];
	}
	node->keys[sibling->num_of_keys] = downkey;
	node->pointers[sibling->num_of_keys] = sibling->pointers[sibling->num_of_keys];
	node->num_of_keys++;
	for (int i = 0, j = 0; i < sibling->num_of_keys; i++, j++) {
		node->keys[i] = sibling->keys[j];
		node->pointers[i] = sibling->pointers[j];
		((Node *)node->pointers[i])->parent = node;
		node->num_of_keys++;
		sibling->num_of_keys--;
	}
	Node * parent = node->parent;
	for (int i = parent_index; i < parent->num_of_keys; i++) {
		parent->keys[i] = parent->keys[i + 1];
		parent->pointers[i] = parent->pointers[i + 1];
	}
	parent->pointers[parent->num_of_keys] = parent->pointers[parent->num_of_keys + 1];
	parent->num_of_keys--;
	node->prev = sibling->prev;
	if (sibling->prev != NULL)
		sibling->prev->next = node;
	free_node(sibling);
}

void merge_right_node(BTree * tree, int const parent_index, Node * node)
{
	Node * sibling = (Node *)node->parent->pointers[parent_index];
	int downkey = node->parent->keys[parent_index - 1];
	int num_of_borrow = sibling->num_of_keys;
	node->keys[node->num_of_keys] = downkey;
	node->num_of_keys++;
	for (int i = node->num_of_keys, j = 0; j < num_of_borrow; i++, j++) {
		node->keys[i] = sibling->keys[j];
		node->pointers[i] = sibling->pointers[j];
		((Node *)node->pointers[i])->parent = node;
		node->num_of_keys++;
		sibling->num_of_keys--;
	}
	node->pointers[node->num_of_keys] = sibling->pointers[num_of_borrow];
	((Node *)node->pointers[node->num_of_keys])->parent = node;
	Node * parent = node->parent;
	for (int i = parent_index; i < parent->num_of_keys; i++) {
		parent->keys[i - 1] = parent->keys[i];
		parent->pointers[i] = parent->pointers[i + 1];
	}
	parent->pointers[parent->num_of_keys] = parent->pointers[parent->num_of_keys + 1];
	parent->num_of_keys--;
	node->next = sibling->next;
	sibling->next->prev = node;
	free_node(sibling);
}

/* �����ڵ� */
void adjust_node(BTree * tree, Node * node)
{
	/* 13�������root�ڵ㣬���key��Ŀ=0��ɾ��root�ڵ㣬�ϲ�root�ڵ���ӽڵ��Ϊ�µ�root�ڵ㣬���key��Ŀ����0��ɾ������ */
	if (tree->root == node) {
		if (node->num_of_keys == 0) {
			free(tree);
		}
		return;
	}
	/* 14������ڵ������ֵܣ�����ڵ�����ֵܵ�keys�ܺ� */
	int left_silbing_index_in_parent = get_left_sibling_index(node);
	if (left_silbing_index_in_parent > -1) {
		/* 15������ܺʹ��ڵ���2����order/2����ȡ��-1���������ֵܽڵ�index��order/2����ȡ��-1��ʼ��keys��index��order/2����ȡ����ʼ��poninter������ԭ�ڵ� */
		int sum_of_keys = node->num_of_keys + ((Node *)node->parent->pointers[left_silbing_index_in_parent])->num_of_keys;
		if (sum_of_keys >= 2 * (ceil(ORDER) - 1)) {
			borrow_from_left_node(tree, ceil(ORDER) - 1, left_silbing_index_in_parent, node);
		}
		/* 17�������ظ��������ֵܺ͵�ǰ�ڵ�ϲ���ɾ����ڵ��ڸ��ڵ��е�key��֮��Ĳ��� */
		else {
			merge_left_node(tree, left_silbing_index_in_parent, node);
			if (node->parent->num_of_keys < ceil(ORDER) - 1)
				adjust_node(tree, node->parent);
		}
	}
	/* 18������ڵ�ֻ�����ֵܣ�����ڵ�����ֵܵ�keys�ܺ� */
	else{
		/* 19������ܺʹ��ڵ���2����order/2����ȡ��-1���������ֵܽڵ�index��0��order/2����ȡ��-2��ʼ��keys��index��0��order/2����ȡ��-1��ʼ��poninter������ԭ�ڵ� */
		int sum_of_keys = node->num_of_keys + ((Node *)node->parent->pointers[1])->num_of_keys;
		if (sum_of_keys >= 2 * (ceil(ORDER) - 1)) {
			borrow_from_right_node(tree, ceil(ORDER) - 2, 1, node);
		}
		/* 21�������ظ��������ֵܺ͵�ǰ�ڵ�ϲ���ɾ����ڵ��ڸ��ڵ��е�key��֮��Ĳ��� */
		else {
			merge_right_node(tree, 1, node);
			if (node->parent->num_of_keys < ceil(ORDER) - 1)
				adjust_node(tree, node->parent);
		}
	}
}

/* ����Ҷ�ӽڵ� */
void adjust_leaf(BTree * tree, Node * leaf)
{
	/* 6�������root�ڵ㣬��ɾ������ */
	if (tree->root == leaf) {
		if (leaf->num_of_keys == 0) {
			free_tree(tree);
		}
		return;
	}
	
	/* 7�����Ҷ�ӽڵ������ֵܣ�����Ҷ�ӽڵ�����ֵܵ�keys�ܺ� */
	int left_silbing_index_in_parent = get_left_sibling_index(leaf);
	if (left_silbing_index_in_parent > -1) {
		/* 8������ܺʹ��ڵ���2���ģ�order/2����ȡ�����������ֵܽڵ�index��order/2����ȡ����ʼ��keys�Ͷ�Ӧ��pointers������Ҷ�ӽڵ� */
		int sum_of_keys = leaf->num_of_keys + ((Node *)leaf->parent->pointers[left_silbing_index_in_parent])->num_of_keys;
		if (sum_of_keys >= 2 * ceil(ORDER)) {
			borrow_from_left_leaf(tree, ceil(ORDER), left_silbing_index_in_parent, leaf);
		}
		/* 10�����򣬽����ֵܺ͵�ǰҶ�Ӻϲ���ɾ�����ֵܽڵ��ڸ��ڵ��е�key */
		else {
			merge_left_leaf(tree, left_silbing_index_in_parent, leaf);
			/* 12��������ڵ�Υ�棬�����ڵ� */
			if (leaf->parent->num_of_keys < ceil(ORDER) - 1) {
				adjust_node(tree, leaf->parent);
			}
			/* 11��������ڵ㲻Υ�棬��ɾ������ */
		}
	}
	/* 22�����Ҷ�ӽڵ�ֻ�����ֵܣ�����Ҷ�ӽڵ�����ֵܵ�keys�ܺ� */
	else {
		/* 23������ܺʹ��ڵ���2���ģ�order/2����ȡ�����������ֵܽڵ�index��0��order/2����ȡ��-2��keys�Ͷ�Ӧ��pointers������Ҷ�ӽڵ㣬����Ҷ�ӽڵ㶼��Υ�棬ɾ������ */
		int sum_of_keys = leaf->num_of_keys + ((Node *)leaf->parent->pointers[1])->num_of_keys;
		if (sum_of_keys >= 2 * ceil(ORDER)) {
			borrow_from_right_leaf(tree, ceil(ORDER) - 2, 1, leaf);
		}
		/* 25�����򣬽����ֵܺ͵�ǰҶ�Ӻϲ���ɾ��Ҷ�ӽڵ��ڸ��ڵ��е�key */
		else {
			merge_right_leaf(tree, 1, leaf);
			/* 26��������ڵ�Υ�棬�����ڵ� */
			if (leaf->parent->num_of_keys < ceil(ORDER) - 1) {
				adjust_node(tree, leaf->parent);
			}
			/* 27��������ڵ㲻Υ�棬��ɾ������ */
		}
	}
}

/* ɾ�� */
BTree * delete(BTree * tree, int const key)
{
	Node * leaf;
	Record * record;
	/* 1�����Բ����ڵ�key */
	record = search_record(tree, key);
	if (record == NULL)
		return tree;
	/* 2���ҵ�������ŵ�Ҷ�ӽڵ� */
	leaf = search_leaf(tree, key);
	if (leaf != NULL) {
		/* 3��ɾ��Ҷ�ӽڵ��е�key */
		delete_from_leaf(leaf, key);
		free(record);
	}
	/* 5�����Ҷ�ӽڵ�Υ�棬����Ҷ�ӽڵ� */
	if (((tree->root == leaf) && (leaf->num_of_keys < 1))	//rootҶ�ӽڵ�
		|| ((tree->root != leaf) && (leaf->num_of_keys < ceil(ORDER)))) {	//��ͨҶ�ӽڵ�
		adjust_leaf(tree, leaf);
	}
	/* 4�����Ҷ�ӽڵ㲻Υ�棬��ɾ������ */
	return tree;
}