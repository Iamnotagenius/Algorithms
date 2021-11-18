#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <iso646.h>
#include <stdbool.h>


struct node;
typedef struct node {
	void *current;
	struct node *next;
} queue_node;

typedef struct q {
	size_t size;
	queue_node *head;
	queue_node *tail;
} * queue;


queue create_queue(size_t size) {
	queue new = malloc(sizeof(struct q));
	new->size = size;
	new->head = NULL;
	new->tail = NULL;
	return new;
}

void delete_queue(queue s) {
	while (s->tail != NULL) {
		queue_node *tmp = s->tail;
		s->tail = s->tail->next;	
		free(tmp->current);
		free(tmp);
	}
}

void push(queue s, void *element) {
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

int pop(queue s, void *buf) {
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


#define HASH_MOD 7723

struct state;


typedef struct ins_list {
    struct ins_list *next;
    void (*command)(struct state *, char[]);
    char *args;
} * command_list;

typedef struct {
    command_list commands;
    char *name;
} label;

typedef struct label_list {
    struct label_list *next;
    label label;
} * label_list;

typedef struct state {
    queue queue;
    label_list *labels;
    command_list current;
    char *skip_to_label;
    FILE *output;
    uint16_t registers[26];
} * machine_state;

int hash_label(char *str) {
    int result = 0;
    while (*++str) {
        result += *str;
        result <<= 6;
    }
    return result % HASH_MOD;
}

void put_label(machine_state state, char *name) {
    int index = hash_label(name);

    if (state->labels[index] == NULL) {
        label_list new_list;
        new_list = malloc(sizeof(struct label_list));
        new_list->label.commands = state->current;
        new_list->label.name = strdup(name);
        new_list->next = NULL;
        state->labels[index] = new_list;
        if (state->skip_to_label != NULL and strcmp(name, state->skip_to_label) == 0) {
            free(state->skip_to_label);
            state->skip_to_label = NULL;
        }
        return;
    }

    label_list collided = state->labels[index];
    while (collided->next != NULL) {
        collided = collided->next;
    }

    collided->next = malloc(sizeof(struct label_list));
    collided->next->label.commands = state->current;
    collided->next->label.name = strdup(name);
    collided->next->next = NULL;

    if (strcmp(name, state->skip_to_label) == 0) {
        free(state->skip_to_label);
        state->skip_to_label = NULL;
    }
}

void set_label(machine_state state, char *name) {
    int index = hash_label(name);
    label_list collided = state->labels[index];
    
    while (collided != NULL and strcmp(collided->label.name, name) != 0) {
        collided = collided->next;
    }
    if (collided == NULL) {
        state->skip_to_label = strdup(name);
        return;
    }
    state->current = collided->label.commands;
}

machine_state init() {
    machine_state state = malloc(sizeof(struct state));
    state->queue = create_queue(sizeof(uint16_t));
    state->labels = calloc(HASH_MOD, sizeof(label_list));
    state->current = NULL;
    state->skip_to_label = NULL;
    state->output = stdout;
    memset(state->registers, 0, sizeof(uint16_t) * 26);
    return state;
}


void put(machine_state state, char arg[]) {
    uint16_t converted = atoi(arg);
    push(state->queue, &converted);
}

void plus(machine_state state, char arg[]) {
    uint16_t x, y;
    pop(state->queue, &x);
    pop(state->queue, &y);
    x += y;
    push(state->queue, &x);
}

void minus(machine_state state, char arg[]) {
    uint16_t x, y;
    pop(state->queue, &x);
    pop(state->queue, &y);
    x -= y;
    push(state->queue, &x);
}

void multiply(machine_state state, char arg[]) {
    uint16_t x, y;
    pop(state->queue, &x);
    pop(state->queue, &y);
    x *= y;
    push(state->queue, &x);
}

void divide(machine_state state, char arg[]) {
    uint16_t x, y;
    pop(state->queue, &x);
    pop(state->queue, &y);
    if (y == 0) {
        uint16_t zero = 0;
        push(state->queue, &zero);
        return;
    }
    x /= y;
    push(state->queue, &x);
}

void mod(machine_state state, char arg[]) {
    uint16_t x, y;
    pop(state->queue, &x);
    pop(state->queue, &y);
    if (y == 0) {
        uint16_t zero = 0;
        push(state->queue, &zero);
        return;
    }
    x %= y;
    push(state->queue, &x);
}

void put_to_reg(machine_state state, char arg[]) {
    uint16_t x;
    pop(state->queue, &x);
    state->registers[arg[0] - 'a'] = x;
}

void get_from_reg(machine_state state, char arg[]) {
    push(state->queue, state->registers + arg[0] - 'a');
}

void print(machine_state state, char arg[]) {
    if (strlen(arg) > 0) {
        fprintf(state->output, "%d\n", state->registers[arg[0] - 'a']);
        return;
    }
    uint16_t x;
    pop(state->queue, &x);
    fprintf(state->output, "%d\n", x);
}

void print_char(machine_state state, char arg[]) {
    if (strlen(arg) > 0) {
        fprintf(state->output, "%c", state->registers[arg[0] - 'a'] % 256);
        return;
    }
    uint16_t x;
    pop(state->queue, &x);
    x %= 256;
    fprintf(state->output, "%c", x);
}

/* current command MUST BE declare label NOT the previous one */
void declare_label(machine_state state, char arg[]) {
    put_label(state, arg);
}

void jump_to_label(machine_state state, char arg[]) {
    set_label(state, arg);
}

void jump_if_zero(machine_state state, char arg[]) {
    if (state->registers[arg[0]  - 'a'] == 0) {
        set_label(state, arg + 1);
    }
}

void jump_if_equal(machine_state state, char arg[]) {
    if (state->registers[arg[0] - 'a'] == state->registers[arg[1] - 'a']) {
        set_label(state, arg + 2);
    }
}

void jump_if_greater(machine_state state, char arg[]) {
    if (state->registers[arg[0] - 'a'] > state->registers[arg[1] - 'a']) {
        set_label(state, arg + 2);
    }
}

void execute_command(machine_state state) {
    if (state->skip_to_label == NULL or state->current->command == declare_label) {
        state->current->command(state, state->current->args);
    }
    state->current = state->current->next;
}

const char IN_FILE[] = "quack.in";
const char OUT_FILE[] = "quack.out";
const int DEFAULT_BUF_SIZE = 4096;


int main(int argc, char **argv) {
    FILE *in = fopen(IN_FILE, "r"),
         *out= fopen(OUT_FILE, "w");
    machine_state state = init();
    state->output = out;
    int buffer_len = argc > 1 ? atoi(argv[1]) : DEFAULT_BUF_SIZE,
        arg_len;
    char *buffer = malloc(buffer_len);
    void (*action)(machine_state, char[]);
    bool has_label = false;
    command_list prev = NULL, *next = &state->current;
    while (!feof(in)) {
        if (state->current == NULL) {
            fgets(buffer, buffer_len, in);
            arg_len = strlen(buffer);
            state->current = malloc(sizeof(struct ins_list) + arg_len + 1);
            state->current->next = NULL;
            if (has_label) {
                *next = state->current;
            }
            switch (buffer[0]) {
                case '+': state->current->command = plus; break;
                case '-': state->current->command = minus; break;
                case '*': state->current->command = multiply; break;
                case '/': state->current->command = divide; break;
                case '%': state->current->command = mod; break;
                case '>': state->current->command = put_to_reg; break;
                case '<': state->current->command = get_from_reg; break;
                case 'P': state->current->command = print; break;
                case 'C': state->current->command = print_char; break;
                case ':': state->current->command = declare_label; has_label = true; break;
                case 'J': state->current->command = jump_to_label; break;
                case 'Z': state->current->command = jump_if_zero; break;
                case 'E': state->current->command = jump_if_equal; break;
                case 'G': state->current->command = jump_if_greater; break;
                case 'Q': return 0;
                default: state->current->command = put;
            }
            state->current->args = (char *)state->current + sizeof(struct ins_list);
            strtok(buffer, "\n");
            if (state->current->command == put) {
                strcpy(state->current->args, buffer);
            }
            else {
                strcpy(state->current->args, buffer + 1);
            }
        }

        if (has_label) {
            next = &state->current->next;
            execute_command(state);
        }
        else {
            prev = state->current;
            execute_command(state);
            free(prev);
        }
    }
}
