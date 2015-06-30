#include "queue.h"

/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void push (Queue* queue, int item) {
	// Create a new node
	Node* n = (Node*) malloc (sizeof(Node));
	n->item = item;
	n->next = NULL;

	if (queue->head == NULL) { // no head
		queue->head = n;
	} else{
		queue->tail->next = n;
	}
	queue->tail = n;
	queue->size++;
}

/**
 * Return and remove the first item.
 */
int pop (Queue* queue) {
    // get the first item
	Node* head = queue->head;
	int item = head->item;
	// move head pointer to next node, decrease size
	queue->head = head->next;
	queue->size--;
	// free the memory of original head
	free(head);
	return item;
}

/**
 * Return but not remove the first item.
 */
int peek (Queue* queue) {
	Node* head = queue->head;
	return head->item;
}

/**
 * Show all items in queue.
 */
void display (Queue* queue) {
	myPrintf1("\nDisplay: ");
	// no item
	if (queue->size == 0)
		myPrintf1("No item in queue.\r\n");
	else { // has item(s)
		Node* head = queue->head;
		int i, size = queue->size;
		myPrintf3("%d item(s):\r\n", queue->size);
		for (i = 0; i < size; i++) {
			if (i > 0)
				myPrintf1(", ");
			myPrintf3("%d", head->item);
			head = head->next;
		}
	}
	myPrintf1("\r\n\r\n");
}
/**
 * Create and initiate a Queue
 */
void QueueInit (Queue* queue) {
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;
	queue->push = &push;
	queue->pop = &pop;
	queue->peek = &peek;
	queue->display = &display;
}
