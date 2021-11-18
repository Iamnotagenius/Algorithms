#include <stdint.h>
#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct tree {
    int key;
    int left;
    int right;
    int parent;
};

bool traversal(struct tree *mas, int index, int min, int max) {
    if (index == 0)
        return true;

    if (mas[index].key <= min or mas[index].key >= max) {
        return false;
    }
    
    return traversal(mas, mas[index].left, min, mas[index].key) and
           traversal(mas, mas[index].right, mas[index].key, max);
}

bool is_bst(struct tree *root) {
    return traversal(root, 1, INT32_MIN, INT32_MAX);
}

const char IN_FILE[]  = "check.in";
const char OUT_FILE[] = "check.out";

int main(int argc, char **argv) {
	FILE *in = fopen(IN_FILE, "r"),
		 *out = fopen(OUT_FILE, "w");
    int n; 
    fscanf(in, "%d", &n);

    if (n == 0) {
        fprintf(out, "YES\n");
        return 0;
    }

	struct tree *mas = malloc((n + 1) * sizeof(struct tree));
    mas[0].left = 0;
    mas[0].right = 0;
    for (int i = 1; i <= n; ++i) {
        fscanf(in, "%d %d %d", &mas[i].key, &mas[i].left, &mas[i].right);    
    }
    fprintf(out, "%s\n", is_bst(mas) ? "YES" : "NO");
    return 0;
}