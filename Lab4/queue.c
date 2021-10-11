#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int QUEUE_MIN_SIZE = 1024;

typedef struct {
	int length;
	int head;
	int tail;
	void *buffer;
	size_t size;
} queue;

queue *create_queue(size_t size) {
	queue *new = malloc(sizeof(queue));
	new->length = QUEUE_MIN_SIZE;
	new->head = -1;
	new->tail = 0;
	new->buffer = malloc(size * new->length);
	new->size = size;
	return new;
}

void delete_queue(queue *s) {
	free(s->buffer);
	free(s);
}

void expand(queue *s) {
	s->length *= 2;
	s->buffer = realloc(s->buffer, s->length);
	if (!s->buffer) {
		fprintf(stderr, "Reallocating memory failed\n");
		exit(1);
	}
}

void push(queue *s, void *element) {
	if (s->length <= ++s->head * s->size)
		expand(s);

	memcpy(s->buffer + s->head * s->size, element, s->size);
}

int pop(queue *s, void *buf) {
	if (s->tail > s->head) 
		return 0;

	memcpy(buf, s->buffer + s->tail * s->size, s->size);
	s->tail++;
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
