#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int STACK_MIN_SIZE = 1024;
struct node;
typedef struct node {
	void *current;
	struct node *prev;
} stack_node;

typedef struct {
	size_t size;
	stack_node *last;
} stack;

stack *create_stack(size_t size) {
	stack *new = malloc(sizeof(stack));
	new->size = size;
	new->last = NULL;
	return new;
}

void delete_stack(stack *s) {
	while (s->last != NULL) {
		stack_node *tmp = s->last;
		s->last = s->last->prev;	
		free(tmp->current);
		free(tmp);
	}
}

void push(stack *s, void *element) {
	stack_node *new = malloc(sizeof(stack_node));
	new->current = malloc(s->size);
	memcpy(new->current, element, s->size);
	new->prev = s->last;
	s->last = new;
}

int pop(stack *s, void *buf) {
	if (s->last == NULL) 
		return 0;

	memcpy(buf, s->last->current, s->size);
	stack_node *tmp = s->last;
	s->last = s->last->prev;	
	free(tmp->current);
	free(tmp);
	return 1;
}

const char IN_FILE[] = "stack.in";
const char OUT_FILE[] = "stack.out";

int main() {
	stack *ints = create_stack(sizeof(int));
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
