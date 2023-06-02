#include "common/section.h"
#include <stdio.h>
#include <string.h>

struct section section_init(struct symbol section_symbol) {
	struct section section = {.name = section_symbol.name,
							  .size = section_symbol.size,
							  .position = 0,
							  .content = malloc(section_symbol.size),
							  .relocation_table = {NULL, 0}};

	return section;
}

void section_write(struct section* section, void* content, size_t size) {
	memcpy(section->content + section->position, content, size);
	section->position += size;
}

void section_print(struct section section) {
	printf("Section \"%s\" (%lu bytes)\n", section.name, section.size);
	relocation_table_print(section.relocation_table);
}

void sections_append(struct sections* sections, struct section section) {
	sections->size++;
	sections->arr =
		realloc(sections->arr, sections->size * sizeof(struct section));

	sections->arr[sections->size - 1] = section;
}

void sections_print(struct sections sections) {
	for (size_t i = 0; i < sections.size; i++)
		section_print(sections.arr[i]);
}