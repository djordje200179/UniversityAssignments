#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>

#define ARRAY_SIZE 1000

void print_array(int arr[], int n) {
	for(int i = 0; i < n; i++) {
		printf("%d", arr[i]);

		if(i != n - 1)
			printf(" ");
	}
	printf("\n");
}

int main() {
	int n;
	scanf("%d", &n);

	if(n <= 0 || n >= ARRAY_SIZE)
		return 0;

	int nums[ARRAY_SIZE];

	for(int i = 0; i < n; i++)
		scanf("%d", &nums[i]);

	bool negative_exists = false;
	for(int i = 0; i < n; i++)
		if(nums[i] <= 0) {
			negative_exists = true;
			break;
		}

	if(negative_exists) {
		printf("NISU POZITIVNI\n");
		return 0;
	} else
		printf("JESU POZITIVNI\n");

	print_array(nums, n);

	int smaller_nums[ARRAY_SIZE] = { 0 };
	for(int i = 0; i < n; i++)
		for(int j = i + 1; j < n; j++)
			if(nums[j] < nums[i]) {
				smaller_nums[i] = nums[j];
				break;
			}

	print_array(smaller_nums, n);

	return 0;
}