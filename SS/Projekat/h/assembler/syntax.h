#pragma once

#include <stdlib.h>

struct line {
	char* label;

	enum {
		LINE_EMPTY,
		LINE_DIR,
		LINE_INS
	} type;

	union {
		int dir;
		int inst;
	};
};

struct lines {
	struct line* arr;
	size_t size;
};

void lines_append(struct lines* lines, struct line line);