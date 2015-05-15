#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bptree.h"

queue *init_queue(int capacity)
{
	queue *q;
	q = malloc(sizeof(queue));
	q->items = malloc(sizeof(Node*) * capacity);
	q->capacity = capacity;
	q->front = q->rear = 0;
	q->size = 0;
	return q;
}

void enqueue(queue *q, Node *nd)
{
	if (q->size == q->capacity) {
		fprintf(stderr, "the queue is full!\n");
		return;
	}
	q->items[q->rear] = nd;
	q->rear = (q->rear + 1) % q->capacity;
	q->size++;
}

Node *dequeue(queue * q)
{
	Node *nd = new_node();
	if (q->size == 0) {
		fprintf(stderr, "the queue is empty!\n");
		return NULL;
	}
	nd = q->items[q->front];
	q->front = (q->front + 1) % q->capacity;
	q->size--;
	return nd;
}
