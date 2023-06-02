#include "common/relocation.h"
#include <stdio.h>

void relocation_print(struct relocation relocation) {
	printf("\tAt %lu write symbol %lu + %lu", relocation.offset,
		   relocation.symbol, relocation.addend);
}

void relocation_table_append(struct relocation_table* table,
							 struct relocation relocation) {
	table->size++;
	table->arr = realloc(table->arr, table->size * sizeof(struct relocation));

	table->arr[table->size - 1] = relocation;
}

void relocation_table_print(struct relocation_table table) {
	for (size_t i = 0; i < table.size; i++) {
		relocation_print(table.arr[i]);
		putchar('\n');
	}
}