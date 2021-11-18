#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iso646.h>

typedef struct node {
    int key;
    struct node *left;
    struct node *right;
    struct node *parent;
} tree_t;

tree_t **find_s(tree_t **node, int key) {
    while (*node != NULL and (*node)->key != key) {
        if (key > (*node)->key)
            node = &(*node)->right;
        else if (key < (*node)->key)
            node = &(*node)->left;
    }
    return node;
}

void insert(tree_t **node, int new) {
    tree_t *parent = NULL;
    while (*node != NULL and (*node)->key != new) {
        parent = *node;

        if (new > (*node)->key) {
            node = &(*node)->right;
        }
        else if (new < (*node)->key) {
            node = &(*node)->left;
        }
    }

    if (*node != NULL) {
        return;
    }

    *node = malloc(sizeof(tree_t));
    (*node)->key = new;
    (*node)->parent = parent;
    (*node)->left = NULL;
    (*node)->right = NULL;
}

tree_t *find(tree_t *node, int key) {
    while (node != NULL and node->key != key) {
        if (key > node->key)
            node = node->right;
        else if (key < node->key)
            node = node->left;
    }
    return node;
}


tree_t *min(tree_t *root) {
    if (root == NULL)
        return NULL;

    while (root->left) 
        root = root->left;
    
    return root;
}

tree_t *max(tree_t *root) {
    if (root == NULL)
        return NULL;
        
    while (root->right) 
        root = root->right;
    
    return root;
}

tree_t *next(tree_t *node, int key) {
    if (node == NULL)
        return NULL;
    
    tree_t *found = find(node, key), *parent = NULL;
    if (found == NULL) {
        while (node != NULL) {
            parent = node;
            if (key > node->key) {
                node = node->right;
            }
            else if (key < node->key) {
                node = node->left;
            }
        }
        if (parent->key > key)
            return parent;
        found = parent;
        parent = parent->parent;
    }
    else if (found->right) {
        return min(found->right);
    }
    else {
        parent = found->parent;
    }
    
    while (parent and parent->right == found) {
        found = parent;
        parent = parent->parent;
    }
    return parent;
}

tree_t **next_s(tree_t **node, int key) {
    if (*node == NULL)
        return node;
    
    tree_t **found = find_s(node, key), **parent = NULL;
    if (*found == NULL) {
        while (node != NULL) {
            parent = node;
            if (key > (*node)->key) {
                node = &(*node)->right;
            }
            else if (key < (*node)->key) {
                node = &(*node)->left;
            }
            if ((*parent)->key > key)
                return parent;
            found = parent;
            parent = &(*parent)->parent;
        }
    }
    else if ((*found)->right) {
        found = &(*found)->right;
        while ((*found)->left != NULL) {
            found = &(*found)->left;
        }
        return found;
    }
    else {
        parent = &(*found)->parent;
    }
    
    while (*parent and (*parent)->right == *found) {
        found = parent;
        parent = &(*parent)->parent;
    }
    return parent;
}
tree_t *prev(tree_t *node, int key) {
    if (node == NULL) {
        return NULL;
    }

    tree_t *found = find(node, key), *parent = NULL;

    if (found == NULL) {
        while (node != NULL) {
            parent = node;
            if (key > node->key) {
                node = node->right;
            }
            else if (key < node->key) {
                node = node->left;
            }
        }
        if (parent->key < key)
            return parent;
        found = parent;
        parent = parent->parent;
    }
    else if (found->left) {
        return max(found->left);
    }
    else {
        parent = found->parent;
    }
    
    while (parent and parent->left == found) {
        found = parent;
        parent = parent->parent;
    }
    return parent;
}

void delete(tree_t **root, int key) {
    tree_t **node = find_s(root, key), *parent;
    
    if (*node == NULL)
        return;

    if ((*node)->right and (*node)->left) {
        tree_t **to_delete = next_s(node, key);
        (*node)->key = (*to_delete)->key;
        node = to_delete;
    }
    parent = (*node)->parent;
    if ((*node)->left) {
        *node = (*node)->left;
    }
    else if ((*node)->right) {
        *node = (*node)->right;
    }
    else {
        free(*node);
        *node = NULL;
        return;
    }
    free((*node)->parent);
    (*node)->parent = parent;
}

const char IN_FILE[] = "bstsimple.in";
const char OUT_FILE[] = "bstsimple.out";

int main() {
    FILE *in = fopen(IN_FILE, "r"),
         *out = fopen(OUT_FILE, "w");
    char command[7];
    int key;
    tree_t *root = NULL;

    while (!feof(in)) {
        fscanf(in, "%s %d\n", command, &key);
        if (strcmp(command, "insert") == 0) {
            insert(&root, key);
        }
        
        else if (strcmp(command, "delete") == 0) {
            delete(&root, key);
        }

        else if (strcmp(command, "exists") == 0) {
            fprintf(out, "%s\n", find(root, key) != NULL ? "true" : "false");
        }

        else if (strcmp(command, "next") == 0) {
            tree_t *node = next(root, key);
            if (node == NULL) {
                fprintf(out, "none\n");
                continue;
            }
            fprintf(out, "%d\n", node->key);
        }

        else if (strcmp(command, "prev") == 0) {
            tree_t *node = prev(root, key);
            if (node == NULL) {
                fprintf(out, "none\n");
                continue;
            }
            fprintf(out, "%d\n", node->key);
        }
    }

    return 0;
}
