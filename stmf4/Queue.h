#ifndef _queue_h
#define _queue_h

#include <stdio.h>
#include <stdlib.h>
#include "Uart.h"

typedef struct Node {
	int item;
	struct Node* next;
} Node;

typedef struct {
	int positionSetpoint;
	int velocitySetpoint;
	int forceSetpoint;
} NodeFCmd;

typedef struct Queue {
	Node* head;
	Node* tail;

	void (*push)(struct Queue*, int); // add item to tail
	// get item from head and remove it from queue
	int (*pop)(struct Queue*);
	// get item from head but keep it in queue
	int (*peek)(struct Queue*);
	// display all element in queue
	void (*display)(struct Queue*);
	// size of this queue
	int size;
} Queue;
/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
void push(Queue* queue, int item);
/**
 * Return and remove the first item.
 */
int pop(Queue* queue);
/**
 * Return but not remove the first item.
 */
int peek(Queue* queue);
/**
 * Show all items in queue.
 */
void display(Queue* queue);
/**
 * Create and initiate a Queue
 */
void QueueInit(Queue* queue);

#endif
