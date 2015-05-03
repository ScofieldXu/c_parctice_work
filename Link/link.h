#include "type.h"

SingleLinkNode *new_single_link_node(int const id);
SingleLink *new_single_link();
int append_single_node(SingleLink *link, SingleLinkNode *node);
int insert_single_node_after(SingleLink *link, SingleLinkNode *node, int const after_id);
int insert_single_node_before(SingleLink *link, SingleLinkNode *node, int const before_id);
int delete_single_node(SingleLink *link, int const id);

SingleLinkNode *new_cycle_single_link_node(int const id);
SingleLink *new_cycle_single_link();
int append_cycle_single_node(SingleLink *link, SingleLinkNode *node);
int insert_cycle_single_node_after(SingleLink *link, SingleLinkNode *node, int const after_id);
int insert_cycle_single_node_before(SingleLink *link, SingleLinkNode *node, int const before_id);
int delete_cycle_single_node(SingleLink *link, int const id);

DoubleLinkNode *new_double_link_node(int const id);
DoubleLink *new_double_link();
int append_double_node(DoubleLink *link, DoubleLinkNode *node);
int insert_double_node_after(DoubleLink *link, DoubleLinkNode *node, int const after_id);
int insert_double_node_before(DoubleLink *link, DoubleLinkNode *node, int const before_id);
int delete_double_node(DoubleLink *link, int const id);

DoubleLinkNode *new_cycle_double_link_node(int const id);
DoubleLink *new_cycle_double_link();
int append_cycle_double_node(DoubleLink *link, DoubleLinkNode *node);
int insert_cycle_double_node_after(DoubleLink *link, DoubleLinkNode *node, int const after_id);
int insert_cycle_double_node_before(DoubleLink *link, DoubleLinkNode *node, int const before_id);
int delete_cycle_double_node(DoubleLink *link, int const id);