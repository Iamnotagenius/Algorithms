#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int QUEUE_MIN_SIZE = 1024;

struct node;
typedef struct node {
	void *current;
	struct node *next;
} queue_node;

typedef struct {
	size_t size;
	queue_node *head;
	queue_node *tail;
} queue;

queue *create_queue(size_t size) {
	queue *new = malloc(sizeof(queue));
	new->size = size;
	new->head = NULL;
	new->tail = NULL;
	return new;
}

void delete_queue(queue *s) {
	while (s->tail != NULL) {
		queue_node *tmp = s->tail;
		s->tail = s->tail->next;	
		free(tmp->current);
		free(tmp);
	}
}

void push(queue *s, void *element) {
	queue_node *new = malloc(sizeof(queue_node));
	new->current = malloc(s->size);
	memcpy(new->current, element, s->size);
	new->next = NULL;

	if (s->head == NULL) {
		s->tail = new;
		s->head = new;
	}
	else {
		s->head->next = new;
		s->head = new;
	}
}

int pop(queue *s, void *buf) {
	if (s->tail == NULL) 
		return 0;

	memcpy(buf, s->tail->current, s->size);
	queue_node *tmp = s->tail;
	s->tail = s->tail->next;

	if (s->tail == NULL)
		s->head = NULL;

	free(tmp->current);
	free(tmp);
	return 1;
}

const char IN_FILE[] = "queue.in";
const char OUT_FILE[] = "queue.out";

int main() {
	queue *ints = create_queue(sizeof(int));
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
	int n, x;
	char op;
	fscanf(in, "%d", &n);
	for (int i = 0; i < n; ++i) {
		fscanf(in, "\n%c", &op);
		switch(op) {
			case '+':
				fscanf(in, "%d", &x);
				push(ints, &x);
				break;
			case '-':
				pop(ints, &x);
				fprintf(out, "%d\n", x);
				break;
		}
	}
}
