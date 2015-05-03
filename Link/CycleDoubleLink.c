#include <stdlib.h>
#include <assert.h>
#include "type.h"

#define CYCLE_SINGLE_LINK_ROOT_ID 0

DoubleLinkNode *new_cycle_double_link_node(int const id)
{
	DoubleLinkNode *node = malloc(sizeof(DoubleLinkNode));
	node->id = id;
	node->prev = node;
	node->next = node;
}

DoubleLink *new_cycle_double_link()
{
	DoubleLinkNode *root = new_cycle_double_link_node(CYCLE_SINGLE_LINK_ROOT_ID);
	DoubleLink *cycle_double_link = malloc(sizeof(DoubleLink));
	cycle_double_link->root = root;
	cycle_double_link->last = root;
	return cycle_double_link;
}

int append_cycle_double_node(DoubleLink *link, DoubleLinkNode *node)
{
	link->last->next = node;
	node->prev = link->last;
	link->last = node;
	node->next = link->root;
	link->root->prev = node;
	return EXIT_SUCCESS;
}

int insert_cycle_double_node_after(DoubleLink *link, DoubleLinkNode *node, int const after_id)
{
	DoubleLinkNode *curr_node = link->root;
	while (curr_node->next != NULL && curr_node->id != after_id){
		curr_node = curr_node->next;
		if (curr_node == link->root)
			return EXIT_FAILURE;
	}

	if (curr_node == link->last){
		append_cycle_double_node(link, node);
	}
	else{
		node->next = curr_node->next;
		curr_node->next->prev = node;
		curr_node->next = node;
		node->prev = curr_node;
	}

	return EXIT_SUCCESS;
}

int insert_cycle_double_node_before(DoubleLink *link, DoubleLinkNode *node, int const before_id)
{
	DoubleLinkNode *curr_node = link->root;
	while (curr_node->next != NULL && curr_node->id != before_id){
		curr_node = curr_node->next;
		if (curr_node == link->root)
			return EXIT_FAILURE;
	}

	node->next = curr_node;
	curr_node->prev->next = node;
	node->prev = curr_node->prev;
	curr_node->prev = node;

	return EXIT_SUCCESS;
}

int delete_cycle_double_node(DoubleLink *link, int const id)
{
	/*can't delete root*/
	assert(id > 0);

	DoubleLinkNode *curr_node = link->root;
	while (curr_node->next != NULL && curr_node->id != id){
		curr_node = curr_node->next;
		if (curr_node == link->root)
			return EXIT_FAILURE;
	}

	if (curr_node == link->last)
		link->last = curr_node->prev;

	curr_node->prev->next = curr_node->next;
	curr_node->next->prev = curr_node->prev;
	curr_node->next = NULL;
	curr_node->prev = NULL;
	free(curr_node);

	return EXIT_SUCCESS;
}


