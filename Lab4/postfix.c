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


void print_stack(stack *s) {
	printf("[");
	for (int i = 0; i < s->index; ++i)
		printf("%d, ", ((int*)s->buffer)[i]);
	printf("%d]\n", ((int*)s->buffer)[s->index]);
}

const char IN_FILE[] = "postfix.in";
const char OUT_FILE[] = "postfix.out";

int main() {
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
	int tmp, operand[2];
	char op[2];
	stack *operands = create_stack(sizeof(int));
	while (!feof(in)) {
		if (fscanf(in, " %1[0-9] ", op)) {
			tmp = op[0] - '0';
			push(operands, &tmp);
			continue;
		}

		if (fscanf(in, " %1[-*+] ", op)) {
			pop(operands, operand);
			pop(operands, operand + 1);
			switch (*op) {
				case '+': tmp = operand[1] + operand[0]; break;
				case '-': tmp = operand[1] - operand[0]; break;
				case '*': tmp = operand[1] * operand[0]; break;
			}
			push(operands, &tmp);
			continue;
		}

	}

	pop(operands, &tmp);
	fprintf(out, "%d\n", tmp);
}

