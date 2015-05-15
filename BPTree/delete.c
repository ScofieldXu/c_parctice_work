/* 1、忽略不存在的key
*  2、找到可以落脚的叶子节点
*  3、删除叶子节点中的key
*  4、如果叶子节点不违规，则删除结束
*  5、如果叶子节点违规，调剂叶子节点
*      6、如果是root节点，则删除结束
*      7、如果叶子节点有左兄弟，计算叶子节点和左兄弟的keys总和
*          8、如果总和大于等于2倍的（order/2向上取整），则将左兄弟节点index从order/2向上取整开始的keys和对应的pointers调整到叶子节点
*          9、将调整出key中最小的key值替换父节点对应的key值，两个叶子节点都不违规，删除结束
*          10、否则，将左兄弟和当前叶子合并，删除左兄弟节点在父节点中的key
*          11、如果父节点不违规，则删除结束
*          12、如果父节点违规，调剂节点
*              13、如果是root节点，如果key数目=0，删除root节点，合并root节点的子节点成为新的root节点，如果key数目大于0，删除结束
*              14、如果节点有左兄弟，计算节点和左兄弟的keys总和
*                  15、如果总和大于等于2倍（order/2向上取整-1），则将左兄弟节点index从order/2向上取整-1开始的keys和index从order/2向上取整开始的poninter调整到原节点
*                  16、将借出key中最小的key值与父节点对应的key值交换，两个节点都不违规，删除结束
*                  17、否则，重复“将左兄弟和当前节点合并，删除左节点在父节点中的key”之后的步骤
*              18、如果节点只有右兄弟，计算节点和右兄弟的keys总和
*                  19、如果总和大于等于2倍（order/2向上取整-1），则将右兄弟节点index从0到order/2向上取整-2开始的keys和index从0到order/2向上取整-1开始的poninter调整到原节点
*                  20、将借出key中最大的key值与父节点对应的key值交换，两个节点都不违规，删除结束
*                  21、否则，重复“将左兄弟和当前节点合并，删除左节点在父节点中的key”之后的步骤
*      22、如果叶子节点只有右兄弟，计算叶子节点和右兄弟的keys总和
*          23、如果总和大于等于2倍的（order/2向上取整），则将右兄弟节点index从0到order/2向上取整-2的keys和对应的pointers调整到叶子节点
*          24、将右兄弟中留下的key中最小的key替换父节点中对应的key值，两个叶子节点都不违规，删除结束
*          25、否则，将右兄弟和当前叶子合并，删除叶子节点在父节点中的key
*          26、如果父节点不违规，则删除结束
*          27、如果父节点违规，调剂节点
*              28、重复12 */
#include <stdlib.h>
#include <assert.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif
#include "bptree.h"

/* 查找左兄弟在父节点中的序号，-1表示没有左兄弟 */
int get_left_sibling_index(Node * node)
{
	for (int i = 0; i <= node->parent->num_of_keys; i++)
		if (node->parent->pointers[i] == node)
			return i - 1;
	return -1;
}

/* 删除叶子节点中的key */
void delete_from_leaf(Node * leaf, int const key)
{
	int i = 0;
	/* 删除key，后续的key向前补齐 */
	i = 0;
	while (leaf->keys[i] != key)
		i++;
	for (++i; i < leaf->num_of_keys; i++) {
		leaf->keys[i - 1] = leaf->keys[i];
		leaf->pointers[i - 1] = leaf->pointers[i];
	}
	leaf->num_of_keys--;
}

/* 移动叶子节点中的key */
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
	/* 9、将调整出key中最小的key值替换父节点对应的key值，两个叶子节点都不违规，删除结束 */
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
	/* 24、将右兄弟中留下的key中最小的key替换父节点中对应的key值，两个叶子节点都不违规，删除结束 */
	d_leaf->parent->keys[parent_index] = upkey;
}

/* 合并叶子节点 */
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
	/* 右兄弟指针，只有最左边的叶子才没有左兄弟，所以右兄弟在父节点的指针应为1*/
	assert(parent_index == 1);
	Node * sibling = (Node *)leaf->parent->pointers[parent_index];
	/* 合并等于借出所有keys */
	int num_of_borrow = sibling->num_of_keys;
	for (int i = leaf->num_of_keys, j = 0; j < num_of_borrow; i++, j++) {
		leaf->keys[i] = sibling->keys[j];
		leaf->pointers[i] = sibling->pointers[j];
		leaf->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* 父节点 */
	Node * parent = leaf->parent;
	for (int i = parent_index; i < parent->num_of_keys; i++) {
		parent->keys[i] = parent->keys[i + 1];
		parent->pointers[i] = parent->pointers[i + 1];
	}
	parent->pointers[parent->num_of_keys] = parent->pointers[parent->num_of_keys + 1];
	parent->num_of_keys--;
	free_node(sibling);
}

/* 移动节点中的key */
void borrow_from_left_node(BTree * tree, int const index, int const parent_index, Node * d_node)
{
	Node * sibling = (Node *)d_node->parent->pointers[parent_index];
	int num_of_borrow = sibling->num_of_keys - index;
	int upkey = sibling->keys[index];
	/* 借num_of_borrow，再从父节点下移一个key，一共要空出num_of_borrow+1个位置 */
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
	/* 16、将借出key中最小的key值与父节点对应的key值交换，两个节点都不违规，删除结束 */
	d_node->parent->keys[parent_index] = upkey;
}

void borrow_from_right_node(BTree * tree, int const index, int const parent_index, Node * d_node)
{
	Node * sibling = (Node *)d_node->parent->pointers[parent_index];
	int num_of_borrow = index + 1;
	int upkey = sibling->keys[index];
	/* 借num_of_borrow，再从父节点下移一个key，一共要空出num_of_borrow+1个位置 */
	d_node->keys[d_node->num_of_keys] = d_node->parent->keys[parent_index];
	for (int i = d_node->num_of_keys + 1, j = 0; j <= index; i++, j++) {
		d_node->keys[i] = sibling->keys[j];
		d_node->pointers[i] = sibling->pointers[j];
		d_node->num_of_keys++;
		sibling->num_of_keys--;
	}
	/* 20、将借出key中最大的key值与父节点对应的key值交换，两个节点都不违规，删除结束 */
	d_node->parent->keys[parent_index] = upkey;
}

/* 合并节点 */
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

/* 调整节点 */
void adjust_node(BTree * tree, Node * node)
{
	/* 13、如果是root节点，如果key数目=0，删除root节点，合并root节点的子节点成为新的root节点，如果key数目大于0，删除结束 */
	if (tree->root == node) {
		if (node->num_of_keys == 0) {
			free(tree);
		}
		return;
	}
	/* 14、如果节点有左兄弟，计算节点和左兄弟的keys总和 */
	int left_silbing_index_in_parent = get_left_sibling_index(node);
	if (left_silbing_index_in_parent > -1) {
		/* 15、如果总和大于等于2倍（order/2向上取整-1），则将左兄弟节点index从order/2向上取整-1开始的keys和index从order/2向上取整开始的poninter调整到原节点 */
		int sum_of_keys = node->num_of_keys + ((Node *)node->parent->pointers[left_silbing_index_in_parent])->num_of_keys;
		if (sum_of_keys >= 2 * (ceil(ORDER) - 1)) {
			borrow_from_left_node(tree, ceil(ORDER) - 1, left_silbing_index_in_parent, node);
		}
		/* 17、否则，重复“将左兄弟和当前节点合并，删除左节点在父节点中的key”之后的步骤 */
		else {
			merge_left_node(tree, left_silbing_index_in_parent, node);
			if (node->parent->num_of_keys < ceil(ORDER) - 1)
				adjust_node(tree, node->parent);
		}
	}
	/* 18、如果节点只有右兄弟，计算节点和右兄弟的keys总和 */
	else{
		/* 19、如果总和大于等于2倍（order/2向上取整-1），则将右兄弟节点index从0到order/2向上取整-2开始的keys和index从0到order/2向上取整-1开始的poninter调整到原节点 */
		int sum_of_keys = node->num_of_keys + ((Node *)node->parent->pointers[1])->num_of_keys;
		if (sum_of_keys >= 2 * (ceil(ORDER) - 1)) {
			borrow_from_right_node(tree, ceil(ORDER) - 2, 1, node);
		}
		/* 21、否则，重复“将左兄弟和当前节点合并，删除左节点在父节点中的key”之后的步骤 */
		else {
			merge_right_node(tree, 1, node);
			if (node->parent->num_of_keys < ceil(ORDER) - 1)
				adjust_node(tree, node->parent);
		}
	}
}

/* 调整叶子节点 */
void adjust_leaf(BTree * tree, Node * leaf)
{
	/* 6、如果是root节点，则删除结束 */
	if (tree->root == leaf) {
		if (leaf->num_of_keys == 0) {
			free_tree(tree);
		}
		return;
	}
	
	/* 7、如果叶子节点有左兄弟，计算叶子节点和左兄弟的keys总和 */
	int left_silbing_index_in_parent = get_left_sibling_index(leaf);
	if (left_silbing_index_in_parent > -1) {
		/* 8、如果总和大于等于2倍的（order/2向上取整），则将左兄弟节点index从order/2向上取整开始的keys和对应的pointers调整到叶子节点 */
		int sum_of_keys = leaf->num_of_keys + ((Node *)leaf->parent->pointers[left_silbing_index_in_parent])->num_of_keys;
		if (sum_of_keys >= 2 * ceil(ORDER)) {
			borrow_from_left_leaf(tree, ceil(ORDER), left_silbing_index_in_parent, leaf);
		}
		/* 10、否则，将左兄弟和当前叶子合并，删除左兄弟节点在父节点中的key */
		else {
			merge_left_leaf(tree, left_silbing_index_in_parent, leaf);
			/* 12、如果父节点违规，调剂节点 */
			if (leaf->parent->num_of_keys < ceil(ORDER) - 1) {
				adjust_node(tree, leaf->parent);
			}
			/* 11、如果父节点不违规，则删除结束 */
		}
	}
	/* 22、如果叶子节点只有右兄弟，计算叶子节点和右兄弟的keys总和 */
	else {
		/* 23、如果总和大于等于2倍的（order/2向上取整），则将右兄弟节点index从0到order/2向上取整-2的keys和对应的pointers调整到叶子节点，两个叶子节点都不违规，删除结束 */
		int sum_of_keys = leaf->num_of_keys + ((Node *)leaf->parent->pointers[1])->num_of_keys;
		if (sum_of_keys >= 2 * ceil(ORDER)) {
			borrow_from_right_leaf(tree, ceil(ORDER) - 2, 1, leaf);
		}
		/* 25、否则，将右兄弟和当前叶子合并，删除叶子节点在父节点中的key */
		else {
			merge_right_leaf(tree, 1, leaf);
			/* 26、如果父节点违规，调剂节点 */
			if (leaf->parent->num_of_keys < ceil(ORDER) - 1) {
				adjust_node(tree, leaf->parent);
			}
			/* 27、如果父节点不违规，则删除结束 */
		}
	}
}

/* 删除 */
BTree * delete(BTree * tree, int const key)
{
	Node * leaf;
	Record * record;
	/* 1、忽略不存在的key */
	record = search_record(tree, key);
	if (record == NULL)
		return tree;
	/* 2、找到可以落脚的叶子节点 */
	leaf = search_leaf(tree, key);
	if (leaf != NULL) {
		/* 3、删除叶子节点中的key */
		delete_from_leaf(leaf, key);
		free(record);
	}
	/* 5、如果叶子节点违规，调剂叶子节点 */
	if (((tree->root == leaf) && (leaf->num_of_keys < 1))	//root叶子节点
		|| ((tree->root != leaf) && (leaf->num_of_keys < ceil(ORDER)))) {	//普通叶子节点
		adjust_leaf(tree, leaf);
	}
	/* 4、如果叶子节点不违规，则删除结束 */
	return tree;
}