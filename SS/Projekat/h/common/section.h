#pragma once

#include <stdlib.h>
#include "symbol.h"
#include "relocation.h"

struct section {
	char* name;
	size_t size;

	size_t position;
	void* content;

	struct relocation_table relocation_table;
};

struct section section_init(struct symbol section_symbol);
void section_write(struct section* section, void* content, size_t size);
void section_print(struct section section);

struct sections {
	struct section* arr;
	size_t size;
};

void sections_append(struct sections* sections, struct section section);
void sections_print(struct sections sections);
