#include <stdlib.h>
#include <stdio.h>
#include "link.h"

void testSingleLink()
{
	SingleLink *single_link = new_single_link();
	SingleLinkNode *node1 = new_single_link_node(1);
	SingleLinkNode *node2 = new_single_link_node(2);
	SingleLinkNode *node3 = new_single_link_node(3);
	SingleLinkNode *node4 = new_single_link_node(4);
	SingleLinkNode *node5 = new_single_link_node(5);

	append_single_node(single_link, node1);
	append_single_node(single_link, node2);
	insert_single_node_after(single_link, node3, 1);
	insert_single_node_after(single_link, node4, 2);
	insert_single_node_before(single_link, node5, 1);
	delete_single_node(single_link, 3);

	SingleLinkNode *curr_node = single_link->root;
	while (curr_node->next != NULL){
		printf("%d ->", curr_node->id);
		SingleLinkNode *free_p = curr_node;
		curr_node = curr_node->next;
		free(free_p);
	}
	printf("%d\n", curr_node->id);
	free(curr_node);

	/*expect 0->5->1->2->4*/
	free(single_link);
}

void testCycleSingleLink()
{
	SingleLink *cycle_single_link = new_cycle_single_link();
	SingleLinkNode *node1 = new_cycle_single_link_node(1);
	SingleLinkNode *node2 = new_cycle_single_link_node(2);
	SingleLinkNode *node3 = new_cycle_single_link_node(3);
	SingleLinkNode *node4 = new_cycle_single_link_node(4);
	SingleLinkNode *node5 = new_cycle_single_link_node(5);

	append_cycle_single_node(cycle_single_link, node1);
	append_cycle_single_node(cycle_single_link, node2);
	insert_cycle_single_node_after(cycle_single_link, node3, 1);
	insert_cycle_single_node_after(cycle_single_link, node4, 2);
	insert_cycle_single_node_before(cycle_single_link, node5, 1);
	delete_cycle_single_node(cycle_single_link, 3);

	SingleLinkNode *curr_node = cycle_single_link->root;
	while (curr_node->next != cycle_single_link->root){
		printf("%d ->", curr_node->id);
		SingleLinkNode *free_p = curr_node;
		curr_node = curr_node->next;
		if (free_p != cycle_single_link->root)
			free(free_p);
	} 
	printf("%d\n", curr_node->id);
	free(curr_node);

	/*expect 0->5->1->2->4*/
	free(cycle_single_link);
}

void testDoubleLink()
{
	DoubleLink *double_link = new_double_link();
	DoubleLinkNode *node1 = new_double_link_node(1);
	DoubleLinkNode *node2 = new_double_link_node(2);
	DoubleLinkNode *node3 = new_double_link_node(3);
	DoubleLinkNode *node4 = new_double_link_node(4);
	DoubleLinkNode *node5 = new_double_link_node(5);

	append_double_node(double_link, node1);
	append_double_node(double_link, node2);
	insert_double_node_after(double_link, node3, 1);
	insert_double_node_after(double_link, node4, 2);
	insert_double_node_before(double_link, node5, 1);
	delete_double_node(double_link, 3);

	DoubleLinkNode *curr_node = double_link->root;
	while (curr_node->next != NULL){
		printf("%d ->", curr_node->id);
		DoubleLinkNode *free_p = curr_node;
		curr_node = curr_node->next;
		free(free_p);
	}
	printf("%d\n", curr_node->id);
	free(curr_node);

	/*expect 0->5->1->2->4*/
	free(double_link);
}

void testCycleDoubleLink()
{
	DoubleLink *cycle_double_link = new_cycle_double_link();
	DoubleLinkNode *node1 = new_cycle_double_link_node(1);
	DoubleLinkNode *node2 = new_cycle_double_link_node(2);
	DoubleLinkNode *node3 = new_cycle_double_link_node(3);
	DoubleLinkNode *node4 = new_cycle_double_link_node(4);
	DoubleLinkNode *node5 = new_cycle_double_link_node(5);

	append_cycle_double_node(cycle_double_link, node1);
	append_cycle_double_node(cycle_double_link, node2);
	insert_cycle_double_node_after(cycle_double_link, node3, 1);
	insert_cycle_double_node_after(cycle_double_link, node4, 2);
	insert_cycle_double_node_before(cycle_double_link, node5, 1);
	delete_cycle_double_node(cycle_double_link, 3);

	DoubleLinkNode *curr_node = cycle_double_link->root;
	while (curr_node->next != cycle_double_link->root){
		printf("%d ->", curr_node->id);
		DoubleLinkNode *free_p = curr_node;
		curr_node = curr_node->next;
		if (free_p != cycle_double_link->root)
			free(free_p);
	}
	printf("%d\n", curr_node->id);
	free(curr_node);

	/*expect 0->5->1->2->4*/
	free(cycle_double_link);
}

int main()
{
	testSingleLink();
	testCycleSingleLink();
	testDoubleLink();
	testCycleDoubleLink();
	system("pause");
	return EXIT_SUCCESS;
}
