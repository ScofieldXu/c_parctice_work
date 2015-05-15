#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif

Record * new_record(char * const value);
Node * new_leaf(void);
Node * new_node(void);
BTree * new_tree(int const key, Record * const pointer);
void free_node(Node * node);
void free_tree(BTree * tree);

Record * search_record(BTree * const tree, int const key);
Node * search_leaf(BTree * const tree, int const key);

void insert_into_new_root(BTree * tree, Node * left, int const key, Node * right);
void insert_into_node(BTree * tree, Node * node, int const key, Node * pointer);
void insert_into_parent(BTree * tree, Node * left, int const key, Node * right);
void insert_into_leaf(Node * leaf, int const key, Record * const pointer);
void splitting_leaf(BTree * tree, Node * leaf);
void splitting_node(BTree * tree, Node * node);
BTree * insert(BTree * tree, int const key, char * const value);

void delete_from_leaf(Node * leaf, int const key);
void borrow_from_left_leaf(BTree * tree, int const index, int const parent_index, Node * d_leaf);
void borrow_from_right_leaf(BTree * tree, int const index, int const parent_index, Node * d_leaf);
void merge_left_leaf(BTree * tree, int const parent_index, Node * leaf);
void merge_right_leaf(BTree * tree, int const parent_index, Node * leaf);
void borrow_from_left_node(BTree * tree, int const index, int const parent_index, Node * d_node);
void borrow_from_right_node(BTree * tree, int const index, int const parent_index, Node * d_node);
void merge_left_node(BTree * tree, int const parent_index, Node * node);
void merge_right_node(BTree * tree, int const parent_index, Node * node);
void adjust_node(BTree * tree, Node * node);
void adjust_leaf(BTree * tree, Node * leaf);
BTree * delete(BTree * tree, int const key);

queue *init_queue(int capacity);
void enqueue(queue *q, Node *nd);
Node *dequeue(queue * q);

int ceil(int const length);
int floor(int const length);
void print_tree(BTree * tree);