#include "type.h"

SingleLinkNode *new_single_link_node(int const id);
SingleLink *new_single_link();
int append_single_node(SingleLink *link, SingleLinkNode *node);
int insert_single_node_after(SingleLink *link, SingleLinkNode *node, int const after_id);
int insert_single_node_before(SingleLink *link, SingleLinkNode *node, int const before_id);
int delete_single_node(SingleLink *link, int const id);