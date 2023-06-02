#pragma once

#include <stdlib.h>

struct relocation {
	size_t offset;

	size_t symbol;
	size_t addend;
};

void relocation_print(struct relocation relocation);

struct relocation_table {
	struct relocation* arr;
	size_t size;
};

void relocation_table_append(struct relocation_table *table, struct relocation relocation);
void relocation_table_print(struct relocation_table table);