#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>
#include <stdbool.h>

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

bool is_opening_bracket(char c) {
	return c == '(' or c == '[';
}

bool is_closing_bracket(char c) {
	return c == ')' or c == ']';
}

bool is_corresponding_brackets(char o, char c) {
	return o == '(' and c == ')' or o == '[' and c == ']';
}

bool check_brackets(char *str) {
	stack *s = create_stack(sizeof(char));
	char *cursor = str;
	char tmp;
	while (*++cursor) {
		if (is_opening_bracket(*cursor)) {
			push(s, cursor);
		}
		else if (is_closing_bracket(*cursor)) {
			if (pop(s, &tmp))
				return is_corresponding_brackets(tmp, *cursor);
			else
				return false;
		}
	}
	delete_stack(s);
}		

const char IN_FILE[] = "stack.in";
const char OUT_FILE[] = "stack.out";

int main() {
	stack *ints = create_stack(sizeof(int));
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
	int n, x;
	char op, *buf = malloc(10000);
	fscanf(in, "%d", &n);
	for (int line = 0; line < n; ++line) {
		fscanf(in, "%s", buf);
		fprintf(out, check_brackets(buf) ? "YES\n" : "NO\n");
	}
}
