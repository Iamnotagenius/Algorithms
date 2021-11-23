#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const char IN_FILE[] = "set.in";
const char OUT_FILE[] = "set.out";

bool lookup(unsigned char *bitmap, unsigned key) {
    return bitmap[key / 8] & (1 << (key % 8));
}

void insert(unsigned char *bitmap, unsigned key) {
    bitmap[key / 8] |= 1 << (key % 8);
}

void delete(unsigned char *bitmap, unsigned key) {
    bitmap[key / 8] &= ~(1 << (key % 8));
}

int main() {
    FILE *in = fopen(IN_FILE, "r"),
         *out = fopen(OUT_FILE, "w");

    /* 2^32 / 8 = 2^29 = 536870912 */
    unsigned char *bitmap = calloc(536870912, 1);
    char command[7];
    unsigned int key;
    
    while (!feof(in)) {
        fscanf(in, "%s %d\n", command, &key);

        if (strcmp(command, "insert") == 0) {
            insert(bitmap, key);
        }
        else if (strcmp(command, "exists") == 0) {
            fprintf(out, lookup(bitmap, key) ? "true\n" : "false\n");
        }
        else if (strcmp(command, "delete") == 0) {
            delete(bitmap, key);
        }
    }
}