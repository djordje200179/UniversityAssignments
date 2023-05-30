#include "../../h/assembler/syntax.h"
#include <stdlib.h>

void lines_append(struct lines* lines, struct line line) {
	lines->size++;
	lines->arr = realloc(lines->arr, lines->size * sizeof(struct line));
}