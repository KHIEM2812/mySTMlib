#include "TQueue.h"

void TQueue(void) {
	Queue queue ;
	QueueInit(&queue);
	queue.display(&queue);

	myPrintf1("push item 2\r\n");
	queue.push(&queue, 2);
	myPrintf1("push item 3\r\n");
	queue.push(&queue, 3);
	myPrintf1("push item 6\r\n");
	queue.push(&queue, 6);

	queue.display(&queue);

	myPrintf3( "peek item %d\r\n", queue.peek(&queue));
	queue.display(&queue);

	myPrintf3("pop item %d\r\n", queue.pop(&queue));
	myPrintf3("pop item %d\r\n", queue.pop(&queue));
	queue.display(&queue);

	myPrintf3("pop item %d\r\n", queue.pop(&queue));
	queue.display(&queue);
	myPrintf3("push item 6\r\n");

	queue.display(&queue);
}
