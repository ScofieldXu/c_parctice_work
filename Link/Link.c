#include <stdlib.h>
#include <stdio.h>
#include "singlelink.h"

int main()
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
	system("pause");
	return EXIT_SUCCESS;
}