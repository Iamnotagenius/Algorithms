#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX(x, y) x > y ? x : y

const char IN_FILE[] = "addition.in";
const char OUT_FILE[] = "addition.out";

struct tree {
    int key;
    int balance;
    struct tree *left;
    struct tree *right;
    struct tree *parent;
};

bool is_leaf(struct tree *root) {
    return root->left == NULL and root->right == NULL;
}


int recount_balance(struct tree *root) {
    if (root == NULL)
        return 0;
    int left_height = recount_balance(root->left) + 1;
    int right_height = recount_balance(root->right) + 1;
    root->balance = right_height - left_height;
    return MAX(left_height, right_height);
}

void LL_rotation(struct tree **node) {
    struct tree *temp = (*node)->left->right;
    (*node)->left->right = *node;
    *node = (*node)->left;
    (*node)->right->left = temp;
}

void LR_rotation(struct tree **node) {
    
    struct tree *lr_node = (*node)->left->right,
                *left_temp = lr_node->left,
                *right_temp = lr_node->right;
    
    lr_node->left = (*node)->left;
    lr_node->right = *node;
    *node = lr_node;
    lr_node->left->right = left_temp;
    lr_node->right->left = right_temp;
}

void RL_rotation(struct tree **node) {
    struct tree *rl_node = (*node)->right->left,
                *right_temp = rl_node->right,
                *left_temp = rl_node->left;
    
    rl_node->right = (*node)->right;
    rl_node->left = *node;
    *node = rl_node;
    rl_node->right->left = right_temp;
    rl_node->left->right = left_temp;
}

void RR_rotation(struct tree **node) {
    struct tree *temp = (*node)->right->left;
    (*node)->right->left = *node;
    *node = (*node)->right;
    (*node)->left->right = temp;
}

void perform_rotation(struct tree **node) {
    if ((*node)->balance < 0) {
        if ((*node)->left->balance > 0) {
            LR_rotation(node);
        }
        else {
            LL_rotation(node);
        }
    }
    else if ((*node)->balance > 0) {
        if ((*node)->right->balance < 0) {
            RL_rotation(node);
        }
        else {
            RR_rotation(node);
        }
    }
}

int rebalance(struct tree **root) {
    if (*root == NULL)
        return 0;
    int left_height = rebalance(&(*root)->left) + 1;
    int right_height = rebalance(&(*root)->right) + 1;
    (*root)->balance = right_height - left_height;
    if ((*root)->balance < -1 or (*root)->balance > 1) {
        perform_rotation(root);
        left_height = recount_balance((*root)->left);
        right_height = recount_balance((*root)->right);
        (*root)->balance = right_height - left_height;
    }
    return MAX(left_height, right_height);
}

void insert(struct tree **node, int new) {
    struct tree *parent = NULL;
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

    *node = malloc(sizeof(struct tree));
    (*node)->key = new;
    (*node)->parent = parent;
    (*node)->left = NULL;
    (*node)->right = NULL;
}

int count(struct tree *root) {
    if (root == NULL) {
        return 0;
    }
    int left_height = count(root->left);
    int right_height = count(root->right);
    return left_height + right_height + 1;
}

void print_tree(FILE *file, struct tree *node) {
    static int line = 1;
    fprintf(file, "%d %d %d\n", node->key, 
                                (line + 1) * (node->left != NULL), 
                                (count(node->left) + line + 1) * (node->right != NULL));
    line++;
    if (node->left) {
        print_tree(file, node->left);
    }
    if (node->right) {
        print_tree(file, node->right);
    }
}

int main(int argc, char **argv) {
    int n;
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
    fscanf(in, "%d", &n);

    if (n == 0) {
        int new; 
        fscanf(in, "%d", &new);
        fprintf(out, "1\n%d 0 0\n", new);
        return 0;
    }

    struct tree ***pointers = malloc((n + 1) * sizeof(size_t));

	struct tree *root, *dummy;
    pointers[0] = &dummy;
    pointers[1] = &root;
    for (int i = 1; i <= n; ++i) {
        int left, right;

        struct tree *current = malloc(sizeof(struct tree));
        *pointers[i] = current;
        fscanf(in, "%d %d %d", &current->key, &left, &right);    
        pointers[left] = &current->left;
        *pointers[0] = NULL;
        pointers[right] = &current->right;
        *pointers[0] = NULL;
    }
    free(pointers);
    int new;
    fscanf(in, "%d", &new);
    insert(&root, new);
    rebalance(&root);
    fprintf(out, "%d\n", n + 1);
    print_tree(out, root);
    return 0;
}