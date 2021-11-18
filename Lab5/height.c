#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iso646.h>

#define MAX(x, y) x > y ? x : y

const char IN_FILE[] = "height.in";
const char OUT_FILE[] = "height.out";

struct tree {
    int key;
    struct tree *left;
    struct tree *right;
};

bool is_leaf(struct tree *root) {
    return root->left == NULL and root->right == NULL;
}

int height(struct tree *root) {
    if (root == NULL)
        return 0;
    if (is_leaf(root))
        return 1;
    int left_height = height(root->left);
    int right_height = height(root->right);
    
    return left_height > right_height ? left_height + 1 : right_height + 1;
}

int main(int argc, char **argv) {
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
    int n; 
    fscanf(in, "%d", &n);

    if (n == 0) {
        fprintf(out, "0\n");
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
    fprintf(out, "%d\n", height(root));
    return 0;
}