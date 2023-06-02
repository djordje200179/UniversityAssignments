#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define SECTION_ABS 0
#define SECTION_UNDEF -1

struct symbol {
	enum { SYMBOL_TYPE_NOTYPE, SYMBOL_TYPE_SECTION } type;
	char* name;
	size_t value;
	size_t size;
	size_t section;
	bool global;
};

void symbol_print(struct symbol symbol);

struct symbol_table {
	struct symbol* arr;
	size_t size;
};

void symbol_table_append(struct symbol_table *table, struct symbol symbol);
void symbol_table_print(struct symbol_table table);
struct symbol* symbol_table_find(struct symbol_table table, char* name);