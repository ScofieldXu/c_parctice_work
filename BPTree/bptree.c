#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifndef _TYPE_H
#define _TYPE_H
#include "type.h"
#endif
#include "bptree.h"

int main()
{
	srand((unsigned int)time(NULL));
	BTree * tree = NULL;
	for (int i = 0; i < 14; i++)
		tree = insert(tree, i + 1, rand() + "0");
	print_tree(tree);

	tree = delete(tree, 7);
	print_tree(tree);
	
	getchar();
}