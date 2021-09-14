#include <stdio.h>
const char IN_FILE[] = "aplusbb.in";
const char OUT_FILE[] = "aplusbb.out";

int main() {
	int a, b;
	FILE* input_file = fopen(IN_FILE, "r");	
	FILE* output_file = fopen(OUT_FILE, "w");	

	fscanf(input_file, "%d %d", &a, &b);
	
	fprintf(output_file, "%lld", a + b * b);
	fclose(input_file);
	fclose(output_file);
	return 0;
}
