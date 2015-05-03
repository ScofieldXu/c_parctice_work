#include <stdlib.h>
#include <assert.h>
#include "type.h"

#define CYCLE_SINGLE_LINK_ROOT_ID 0

SingleLinkNode *new_cycle_single_link_node(int const id)
{
	SingleLinkNode *node = malloc(sizeof(SingleLinkNode));
	node->id = id;
	node->next = node;
}

SingleLink *new_cycle_single_link()
{
	SingleLinkNode *root = new_cycle_single_link_node(CYCLE_SINGLE_LINK_ROOT_ID);
	SingleLink *cycle_single_link = malloc(sizeof(SingleLink));
	cycle_single_link->root = root;
	cycle_single_link->last = root;
	return cycle_single_link;
}

int append_cycle_single_node(SingleLink *link, SingleLinkNode *node)
{
	link->last->next = node;
	link->last = node;
	node->next = link->root;
	return EXIT_SUCCESS;
}

int insert_cycle_single_node_after(SingleLink *link, SingleLinkNode *node, int const after_id)
{
	SingleLinkNode *curr_node = link->root;
	while (curr_node->next != NULL && curr_node->id != after_id){
		curr_node = curr_node->next;
		if (curr_node == link->root)
			return EXIT_FAILURE;
	}

	if (curr_node == link->last){
		append_cycle_single_node(link, node);
	}
	else{
		node->next = curr_node->next;
		curr_node->next = node;
	}

	return EXIT_SUCCESS;
}

int insert_cycle_single_node_before(SingleLink *link, SingleLinkNode *node, int const before_id)
{
	SingleLinkNode *curr_node = link->root;
	SingleLinkNode *prev_node = NULL;
	while (curr_node->next != NULL && curr_node->id != before_id){
		prev_node = curr_node;
		curr_node = curr_node->next;
		if (curr_node == link->root)
			return EXIT_FAILURE;
	}

	node->next = curr_node;
	prev_node->next = node;

	return EXIT_SUCCESS;
}

int delete_cycle_single_node(SingleLink *link, int const id)
{
	/*can't delete root*/
	assert(id > 0);

	SingleLinkNode *curr_node = link->root;
	SingleLinkNode *prev_node = NULL;
	while (curr_node->next != NULL && curr_node->id != id){
		prev_node = curr_node;
		curr_node = curr_node->next;
		if (curr_node == link->root)
			return EXIT_FAILURE;
	}

	if (curr_node == link->last)
		link->last = prev_node;

	prev_node->next = curr_node->next;
	curr_node->next = NULL;
	free(curr_node);

	return EXIT_SUCCESS;
}


