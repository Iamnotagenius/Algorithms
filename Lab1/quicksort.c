#include <stdio.h>
#include <stdlib.h>

const char IN_FILE[] = "smallsort.in";
const char OUT_FILE[] = "smallsort.out";

void swap(int *x, int *y) {
	int tmp = *y;
	*y = *x;
	*x = tmp;
}

int partition(int *array, int left, int right) {
	int pivot, *ppivot, smaller;
	ppivot = array + ((left + right) / 2);
	pivot = *ppivot;
	smaller = left;
	swap(ppivot, array + right);
	ppivot = array + right;
	for (int i = left; i < right; i++) {
		if (array[i] < pivot) {
			swap(array + i, array + smaller);
			smaller += 1;
		}
	}
	swap(ppivot, array + smaller);
	return smaller;
}

void quick_sort(int *array, int left, int right) {
		int pivot;
		if (left >= right)
				return;
		pivot = partition(array, left, right);
		quick_sort(array, left, pivot);
		quick_sort(array, pivot + 1, right);
}

int main() {
	int amount, *array;
	FILE *input_file, *output_file;
	input_file = fopen(IN_FILE, "r");
	output_file = fopen(OUT_FILE, "w");
	
	fscanf(input_file, "%d", &amount);
	array = malloc(amount * sizeof(int));

	for (int i = 0; i < amount; i++)
			fscanf(input_file, "%d", array + i);

	quick_sort(array, 0, amount - 1);
	
	for (int i = 0; i < amount; i++)
			fprintf(output_file, "%d ", array[i]);
	
	fclose(input_file);
	fclose(output_file);
	return 0;
}
