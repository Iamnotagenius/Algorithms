#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STACK_MIN_SIZE = 1024;

typedef struct {
	int length;
	int index;
	void *buffer;
	size_t size;
} stack;

stack *create_stack(size_t size) {
	stack *new = malloc(sizeof(stack));
	new->length = STACK_MIN_SIZE;
	new->index = -1;
	new->buffer = malloc(size * new->length);
	new->size = size;
	return new;
}

void delete_stack(stack *s) {
	free(s->buffer);
	free(s);
}

void expand(stack *s) {
	s->length *= 2;
	s->buffer = realloc(s->buffer, s->length);
	if (!s->buffer) {
		fprintf(stderr, "Reallocating memory failed\n");
		exit(1);
	}
}

void push(stack *s, void *element) {
	if (s->length <= ++s->index * s->size)
		expand(s);

	memcpy(s->buffer + s->index * s->size, element, s->size);
}

int pop(stack *s, void *buf) {
	if (s->index < 0) 
		return 0;

	memcpy(buf, s->buffer + s->index * s->size, s->size);
	s->index--;
	return 1;
}

const char IN_FILE[] = "stack.in";
const char OUT_FILE[] = "stack.out";

int main() {
	stack *ints = create_stack(sizeof(int));
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
	/* this works though
	stack *ints = malloc(sizeof(stack));
	ints->buffer = malloc(100000 * sizeof(int));
	ints->index = -1;
	ints->length = 100000;
	ints->size = sizeof(int); */
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
