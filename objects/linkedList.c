/* linkedList.c contains all the linked list functions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

/* Creates a new node and adds it to the end of the list */
void add_node(nodeRef head, void *data, int size) 
{

	nodePtr newNode; /* node to add */
	nodePtr iter; /* iterator */
	void *buffer; /* data buffer */

    /* Creates the data buffer and the node */
    buffer = calloc(1, size);
	newNode = (nodePtr)calloc(1, sizeof(defNode));
	
	/* Checks if buffer and node were created successfully*/
	VERIFY_ALLOCATION(buffer);
	VERIFY_ALLOCATION(newNode);
	
    memcpy(buffer, data, size); /* data copied into buffer */

   	/* Sets the values of the new node */
    newNode->data = buffer;
   	newNode->next = NULL;

	/* If the list is empty */
	if ((*head) == NULL) {
		(*head) = newNode;
			return;
	}

	/* Adds the node to the end of the list */
    iter = (*head);
   	while (iter->next != NULL)
      	 	iter = iter->next;
    iter->next = newNode;
} /* end of add_node */

/* Returns the number of nodes in the list */
int list_len(nodePtr head)
{
	int count = 0; /* node amount */
    	nodePtr iter = head; /* iterator */

	/* If the list is empty */
	if (iter == NULL)
		return 0;

	do {
		count++;
	} while ((iter = iter->next) != NULL);

   	return count;
} /* end of list_len */

/* Gets the first node from the list and removes it */
defNode pop_node(nodeRef head)
{
	defNode poppedNode;
	nodePtr prevHead;

    /* Gets the first node */
	poppedNode.next = (*head)->next;
	poppedNode.data = (*head)->data;

    /* Removes it from the list */
	prevHead = (*head);
        (*head) = (*head)->next;
	/* Clears buffer */
	S_FREE(prevHead); 
	
    return poppedNode;
} /* end of pop_node */

/* Gets the first node from the list */
defNode get_top(nodePtr head) 
{
	defNode firstNode;
	
	firstNode.data = head->data;
	firstNode.next = head->next;
	
   	return firstNode;
} /* end of get_top */

/* Removes a node from the list */
void remove_node(nodeRef head, nodePtr node_to_del)
{
	nodePtr iter;
	
	/* If the list is empty */
	if (*head == NULL)
		return;

	/* If the node to remove is the head */
	if (*head == node_to_del)
	{
		*head = (*head)->next;
		DELETE_NODE(node_to_del);
		return;
	}

	iter = *head;
	/* Searches the list for the node to delete */
	while (iter->next != node_to_del)
		iter = iter->next;

	/* Deletes the node from list */
	iter->next = node_to_del->next;
	DELETE_NODE(node_to_del);
} /* end of remove_node */

/* Clears a list */
void clear_list(nodePtr *head)
{
	nodePtr iter; /* iterator */
	nodePtr nodeToDelete;

	if (*head == NULL)
		return;

	/* Clears all buffers */
	iter = *head;
	while (iter != NULL)
	{
		nodeToDelete = iter;
		iter = iter->next;

		DELETE_NODE(nodeToDelete);
	}
	/* Clears the pointer */
	*head = NULL;
} /* end of clear_list */

