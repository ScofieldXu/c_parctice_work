#include <stdlib.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif

/* 二分法查找并返回key值能落脚的叶子节点 */
Node * search_leaf(BTree * const tree, int const key)
{
	if (tree == NULL)
		return NULL;

	Node * node = tree->root;
	if (node == NULL)
		return node;

	int low = 0, high = 0;
	while (!node->is_leaf) {
		low = 0, high = node->num_of_keys - 1;

		if (key < node->keys[low]) {
			node = (Node *)node->pointers[low];
			continue;
		}
		else if (key >= node->keys[high]) {
			node = (Node *)node->pointers[high + 1];
			continue;
		}

		while (1) {
			int middle = (low + high) / 2;
			if (key == node->keys[middle]) {
				node = (Node *)node->pointers[middle];
				break;
			}
			else if (high - low == 1)
			{
				node = (Node *)node->pointers[high];
				break;
			}
			else if (key < node->keys[middle]) {
				high = middle;
			}
			else if (key > node->keys[middle]) {
				low = middle;
			}
		}
	}
	return node;
}

/* 查找并返回key值对应的实际记录 */
Record * search_record(BTree * const tree, int const key)
{
	int i = 0;
	Node * node = search_leaf(tree, key);
	if (node == NULL) return NULL;

	int low = 0, high = node->num_of_keys - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (node->keys[middle] == key)
			return (Record *)node->pointers[middle];
		//在左半边
		else if (node->keys[middle] > key)
			high = middle - 1;
		//在右半边
		else
			low = middle + 1;
	}
	//没找到
	return NULL;
}