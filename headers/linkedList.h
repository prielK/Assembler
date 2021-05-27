/* linkedList.c contains all the linked list functions */

#ifndef BASICS_H
	#define BASICS_H
	#include "basics.h"
#endif

#define DELETE_NODE(node) S_FREE(node->data); S_FREE(node);

int lineNum;	/* Line counter*/
bool errorFound;	/* Error tracker */

typedef struct node {
    struct node * next;
    void * data;
} defNode, *nodePtr, **nodeRef;

/* Creates a new node and adds it to the end of the list */
void add_node(nodeRef head, void * data, int size);

/* Returns the number of nodes in the list */
int list_len(nodePtr head);

/* Gets the first node from the list and removes it */
defNode pop_node(nodeRef head);

/* Gets the top node from the list */
defNode get_top(nodePtr head);

/* Removes node from the list */
void remove_node(nodeRef head, nodePtr node_to_delete);

/* Clears a list */
void clear_list(nodePtr *head);
