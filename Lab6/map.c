#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>

#define HASHMOD 193939

struct linked_list {
	struct linked_list *next;
	char key_value[];
};

int hash(char *key) {
	unsigned long hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASHMOD;
}

struct linked_list **lookup_node(struct linked_list **map, char *key) {
	int index = hash(key);
	struct linked_list **search = &map[index];
	while (*search != NULL and strcmp((*search)->key_value, key) != 0) {
		search = &(*search)->next;
	}
	return search;
}

void insert(struct linked_list **map, char *key, char *value) {
	int index = hash(key);
	int len_key = strlen(key), len_value = strlen(value);
	struct linked_list **existing = lookup_node(map, key);
	if (*existing != NULL) {
		*existing = realloc(*existing, sizeof(size_t) + len_key + len_value + 2);
		strcpy((*existing)->key_value + len_key + 1, value);
		return;
	}
	struct linked_list *new = malloc(sizeof(size_t) + len_key + len_value + 2);
	new->next = map[index];
	strcpy(new->key_value, key);
	strcpy(new->key_value + len_key + 1, value);
	map[index] = new;
}


char *lookup(struct linked_list **map, char *key) {
	struct linked_list *search = *lookup_node(map, key);
	return search ? search->key_value + strlen(search->key_value) + 1 : NULL;
}

void delete(struct linked_list **map, char *key) {
	struct linked_list **delete = lookup_node(map, key), *tmp;
	if (*delete == NULL)
		return;
	
	tmp = *delete;
	*delete = tmp->next;
	free(tmp);
}

const char IN_FILE[] = "map.in";
const char OUT_FILE[] = "map.out";

int main() {
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
	struct linked_list **map = calloc(HASHMOD, sizeof(size_t));
	char command[7], key[21], value[21];
	while (!feof(in)) {
		fscanf(in, "%s %s\n", command, key);
		if (strcmp(command, "put") == 0) {
			fscanf(in, "%s\n", value);
			insert(map, key, value);
		}
		else if (strcmp(command, "get") == 0) {
			char *search = lookup(map, key);
			fprintf(out, "%s\n", search ? search : "none");
		}
		else if (strcmp(command, "delete") == 0) {
			delete(map, key);
		}
	}
}
