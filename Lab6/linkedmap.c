#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>

#define HASHMOD 193939

struct linked_list {
	struct linked_list *next;
	struct linked_list *succ;
	struct linked_list *prev;
	char key_value[];
};

int hash(char *key) {
	unsigned long hash = 5381;
    int c;

    while ((c = *key++))
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

struct linked_list *insert(struct linked_list **map, char *key, char *value, struct linked_list *last_inserted) {
	int index = hash(key);
	int len_key = strlen(key), len_value = strlen(value);
	struct linked_list **existing = lookup_node(map, key);
	if (*existing != NULL) {
		*existing = realloc(*existing, sizeof(size_t) * 3 + len_key + len_value + 2);
		strcpy((*existing)->key_value + len_key + 1, value);
		
		return last_inserted;
	}
	struct linked_list *new = malloc(sizeof(size_t) * 3 + len_key + len_value + 2);
	new->next = map[index];
	new->succ = NULL;
	strcpy(new->key_value, key);
	strcpy(new->key_value + len_key + 1, value);
	map[index] = new;

	if (last_inserted != NULL)
		last_inserted->succ = new;

	new->prev = last_inserted;
	return new;
}


char *lookup(struct linked_list **map, char *key) {
	struct linked_list *search = *lookup_node(map, key);
	return search ? search->key_value + strlen(search->key_value) + 1 : NULL;
}

char *next(struct linked_list **map, char *key) {
	struct linked_list *search = *lookup_node(map, key);
	if (search == NULL or search->succ == NULL) 
		return NULL;
	
	return search->succ->key_value + strlen(search->succ->key_value) + 1;
}

char *prev(struct linked_list **map, char *key) {
	struct linked_list *search = *lookup_node(map, key);
	if (search == NULL or search->prev == NULL) 
		return NULL;
	
	return search->prev->key_value + strlen(search->prev->key_value) + 1;
}

void delete(struct linked_list **map, char *key) {
	struct linked_list **delete = lookup_node(map, key), *tmp;
	if (*delete == NULL)
		return;
	
	tmp = *delete;
	
	if (tmp->prev != NULL) {
		tmp->prev->succ = tmp->succ;
	}
	if (tmp->succ != NULL) {
		tmp->succ->prev = tmp->prev;
	}

	*delete = tmp->next;
	free(tmp);
}

const char IN_FILE[] = "linkedmap.in";
const char OUT_FILE[] = "linkedmap.out";

int main() {
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
	struct linked_list **map = calloc(HASHMOD, sizeof(size_t)), *last = NULL;
	char command[7], key[21], value[21];
	while (!feof(in)) {
		fscanf(in, "%s %s\n", command, key);
		if (strcmp(command, "put") == 0) {
			fscanf(in, "%s\n", value);
			last = insert(map, key, value, last);
		}
		else if (strcmp(command, "get") == 0) {
			char *search = lookup(map, key);
			fprintf(out, "%s\n", search ? search : "none");
		}
		else if (strcmp(command, "delete") == 0) {
			delete(map, key);
		}
		else if (strcmp(command, "prev") == 0) {
			char *search = prev(map, key);
			fprintf(out, "%s\n", search ? search : "none");
		}
		else if (strcmp(command, "next") == 0) {
			char *search = next(map, key);
			fprintf(out, "%s\n", search ? search : "none");
		}
	}
}
