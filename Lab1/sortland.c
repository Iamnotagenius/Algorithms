#include <stdio.h>
#include <stdlib.h>

const char IN_FILE[] = "sortland.in";
const char OUT_FILE[] = "sortland.out";

struct Civilian {
		int index;
		float salary;
};

void swap(struct Civilian *x, struct Civilian *y) {
	struct Civilian tmp = *y;
	*y = *x;
	*x = tmp;
}

int partition(struct Civilian *array, int left, int right) {
	struct Civilian pivot, *ppivot;
	int smaller;
	ppivot = array + ((left + right) / 2);
	pivot = *ppivot;
	smaller = left;
	swap(ppivot, array + right);
	ppivot = array + right;
	for (int i = left; i < right; i++) {
		if (array[i].salary < pivot.salary) {
			swap(array + i, array + smaller);
			smaller += 1;
		}
	}
	swap(ppivot, array + smaller);

	return smaller;
}

void quick_sort(struct Civilian *array, int left, int right) {
		int pivot;
		if (left >= right)
				return;
		pivot = partition(array, left, right);
		quick_sort(array, left, pivot);
		quick_sort(array, pivot + 1, right);
}

int main() {
	int amount;
	struct Civilian *array;
	FILE *input_file, *output_file;
	input_file = fopen(IN_FILE, "r");
	output_file = fopen(OUT_FILE, "w");
	
	fscanf(input_file, "%d", &amount);
	array = malloc(amount * sizeof(struct Civilian));

	for (int i = 0; i < amount; i++) {
			array[i].index = i + 1;
			fscanf(input_file, "%f", &array[i].salary);
	}

	quick_sort(array, 0, amount - 1);
	fprintf(output_file, "%d %d %d", array[0].index, 
						array[amount / 2].index, 
						array[amount - 1].index);
	
	fclose(input_file);
	fclose(output_file);
	return 0;
}
