/* 插入新的存储值（key - value）
*  1、忽略重复的key
*  2、如果是新树则初始化新建一颗树
*  3、找到可以落脚的叶子结点
*  4、将key插入叶子结点
*  5、如果叶子结点不违规，则插入结束
*  6、如果叶子结点违规，分裂叶子结点
*      7、创建新叶子结点，原叶子作为左叶子，新建叶子为右叶子，属于同一个父结点
*      8、将一半（index从order/2向下取整开始）的keys和对应的pointers移到新叶子结点
*      9、将右叶子结点最小的key值插入父结点适当的位置中，指向左叶子结点，在右叶子结点中“保留”上送父结点的key值
*      10、如果3父结点为空，则新建一个root结点，左右叶子分别作为root结点的左右子结点，插入结束
*      11、将key插入父结点
*      12、如果父结点不违规，则插入结束
*      13、如果父结点违规，分裂父结点（注意，结点的分裂和叶子的分裂不同，因为叶子的子结点数目和key数目一致，结点的子结点数目是key的数目+1）
*          14、创建新的结点，原结点作为左结点，新建结点作为右结点，属于同一个父结点
*          15、将一半（index从order/2向下取整开始）的keys和对应的pointers移到新结点，新结点作为移入新结点的poniters所指的子结点的父结点，在左结点中“不保留”上送父结点key值
*          16、重复“将左结点最大的key值插入父结点适当的位置中”之后的步骤 */
#include <stdlib.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif
#include "bptree.h"

/* 创建新root */
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

/* 向中间点插入key值 */
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

/* 向叶子结点中插入key值 */
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

/* 向父结点插入key值 */
void  insert_into_parent(BTree * tree, Node * left, int const key, Node * right)
{
	Node * parent = left->parent;
	/* 10、如果父结点为空，则新建一个root结点，左右叶子分别作为root结点的左右子结点，插入结束 */
	if (parent == NULL) {
		insert_into_new_root(tree, left, key, right);
		return;
	}
	/* 11、将key插入父结点 */
	insert_into_node(tree, parent, key, right);
	/* 13、如果父结点违规，分裂父结点（注意，结点的分裂和叶子的分裂不同，因为叶子的子结点数目和key数目一致，结点的子结点数目是key的数目+1） */
	if (parent->num_of_keys > ORDER - 1)
		splitting_node(tree, parent);
	/* 12、如果父结点不违规，则插入结束 */
}

/* 分裂父结点 */
void splitting_node(BTree * tree, Node * node)
{
	/* 14、创建新的结点，原结点作为左结点，新建结点作为右结点，属于同一个父结点 */
	Node * _new_node = new_node();
	/* 15、将一半（index从order/2向下取整开始）的keys和对应的pointers移到新结点，新结点作为移入新结点的poniters所指的子结点的父结点 */
	int middle = node->num_of_keys / 2, upkey = node->keys[middle - 1], keycount = node->num_of_keys;
	for (int i = middle, j = 0; i < keycount; i++, j++) {
		_new_node->keys[j] = node->keys[i];
		_new_node->pointers[j] = node->pointers[i];
		((Node *)node->pointers[i])->parent = _new_node;
		_new_node->num_of_keys++;
		node->num_of_keys--;
	}
	/* pointer会比key多一个 */
	_new_node->pointers[_new_node->num_of_keys] = node->pointers[keycount];
	((Node *)node->pointers[keycount])->parent = _new_node;
	_new_node->parent = node->parent;
	_new_node->prev = node;
	_new_node->next = node->next;
	node->next = _new_node;
	/* 在左结点中“不保留”上送父结点key值 */
	node->num_of_keys--;
	/* 16、重复“将左结点最大的key值插入父结点适当的位置中”之后的步骤 */
	insert_into_parent(tree, node, upkey, _new_node);
}

/* 分裂叶子结点 */
void splitting_leaf(BTree * tree, Node * leaf)
{
	/* 7、创建新叶子结点，原叶子作为左叶子，新建叶子为右叶子，属于同一个父结点 */
	Node * _new_leaf = new_leaf();
	/* 8、将一半（index从order/2向下取整开始）的keys和对应的pointers移到新叶子结点 */
	int middle = leaf->num_of_keys / 2, keycount = leaf->num_of_keys;
	/* 在右叶子结点中保留该key值 */
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
	/* 9、将右叶子结点最小的key值插入父结点适当的位置中，指向左叶子结点 */
	insert_into_parent(tree, leaf, _new_leaf->keys[0], _new_leaf);
}

/* 插入 */
BTree * insert(BTree * tree, int const key, char * const value)
{
	/* 1、忽略重复的key */
	if (search_record(tree, key) != NULL)
		return tree;
	Record * pointer = new_record(value);
	/* 2、如果是新树则初始化新建一颗树 */
	if (tree == NULL) {
		tree = new_tree(key, pointer);
		return tree;
	}
	/* 3、找到可以落脚的叶子结点 */
	Node * leaf = search_leaf(tree, key);
	/* 4、将key插入叶子结点 */
	insert_into_leaf(leaf, key, pointer);
	/* 6、如果叶子结点违规，分裂叶子结点 */
	if (leaf->num_of_keys > ORDER - 1) {
		splitting_leaf(tree, leaf);
	}
	/* 5、如果叶子结点不违规，则插入结束 */
	return tree;
}