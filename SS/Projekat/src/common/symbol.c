#include "common/symbol.h"
#include <stdio.h>
#include <string.h>

void symbol_print(struct symbol symbol) {
	printf("%.7s\t", symbol.name);
	switch (symbol.type) {
		case SYMBOL_TYPE_NOTYPE:
			printf("NOTYP\t");
			break;
		case SYMBOL_TYPE_SECTION:
			printf("SCTN\t");
			break;
	}
	printf("%lu\t%lu\t", symbol.value, symbol.size);
	switch (symbol.section) {
		case SECTION_UNDEF:
			printf("UND\t");
			break;
		case SECTION_ABS:
			printf("ABS\t");
			break;
		default:
			printf("%lu\t", symbol.section);
	}
	printf(symbol.global ? "GLOB" : "LOC");
}

void symbol_table_append(struct symbol_table* table, struct symbol symbol) {
	table->size++;
	table->arr = realloc(table->arr, table->size * sizeof(struct symbol));

	table->arr[table->size - 1] = symbol;
}

void symbol_table_print(struct symbol_table table) {
	printf("Num\tName\tType\tValue\tSize\tSection\tBind\n");
	for (size_t i = 0; i < table.size; i++) {
		printf("%lu\t", i);
		symbol_print(table.arr[i]);
		putchar('\n');
	}
}

struct symbol* symbol_table_find(struct symbol_table table, char* name) {
	for (size_t i = 0; i < table.size; i++)
		if (strcmp(table.arr[i].name, name) == 0)
			return &table.arr[i];

	return NULL;
}