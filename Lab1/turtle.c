#include <stdio.h>
#include <stdlib.h>

const char IN_FILE[] = "turtle.in";
const char OUT_FILE[] = "turtle.out";

int max(int x, int y) {
		if (x > y)
				return x;
		return y;
}

int main() {
	int w, h;
	int *field = malloc(w*h*sizeof(int));
	FILE* input_file = fopen(IN_FILE, "r");
	FILE* output_file = fopen(OUT_FILE, "w");

	fscanf(input_file, "%d %d", &h, &w);

	for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
					fscanf(input_file, "%d", field + i * w + j);
			}
	}

	for (int i = (h - 2) * w; i >= 0; i -= w) 
			field[i] += field[i + w];
	for (int j = 1; j < w; j++) 
			field[(h - 1) * w + j] += field[(h - 1) * w + j - 1];
	for (int i = (h - 2) * w; i >= 0; i -= w) 
		   for (int j = 1; j < w; j++) 
			   field[i + j] += max(field[i + w + j], field[i + j - 1]);

	fprintf(output_file, "%d", field[w - 1]);
	free(field);
	
	return 0;
}
