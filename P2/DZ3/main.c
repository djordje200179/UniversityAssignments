#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define CHECK_MEM(pointer) if(pointer == NULL) { printf("MEM_GRESKA\n"); return 0; }

char* readLine() {
	char* string = (char*)malloc(sizeof(char) * 1);
	if(string == NULL)
		return NULL;

	char temp_char;
	int length = 0;

	while((temp_char = getchar()) != '\n') {
		length++;

		char* temp_string = (char*)realloc(string, sizeof(char) * (length + 1));
		if(temp_string == NULL) {
			free(string);

			return NULL;
		} else
			string = temp_string;

		string[length - 1] = temp_char;
	}

	string[length] = '\0';

	return string;
}

char** readLines(int* n) {
	char** lines = NULL;
	*n = 0;

	while(true) {
		char* temp_line = readLine();

		if(temp_line == NULL) {
			for(int i = 0; i < *n; i++)
				free(lines[i]);
			free(lines);

			return NULL;
		}

		if(temp_line[0] == '\0') {
			free(temp_line);

			break;
		}

		(*n)++;

		char** temp_lines = (char**)realloc(lines, sizeof(char*) * (*n));
		if(temp_lines == NULL) {
			for(int i = 0; i < *n; i++)
				free(lines[i]);
			free(lines);

			return NULL;
		} else
			lines = temp_lines;

		lines[*n - 1] = temp_line;
	}

	return lines;
}

void removeComments(char* beginComment, char* endComment, char** code, int n) {
	int c_begin_len = strlen(beginComment);
	int c_end_len = strlen(endComment);

	bool in_comment = false;

	for(int i = 0; i < n; i++) {
		int temp_j = 0;

		for(int j = 0; code[i][j] != '\0'; j++) {
			if(in_comment) {
				if(strncmp(&code[i][j], endComment, c_end_len) == 0) {
					in_comment = false;
					j += c_end_len - 1;
				}
			} else {
				if(strncmp(&code[i][j], beginComment, c_begin_len) == 0)
					in_comment = true;
				else
					code[i][temp_j++] = code[i][j];
			}
		}

		char* temp_line = (char*)realloc(code[i], sizeof(char) * (temp_j + 1));
		if(temp_line == NULL) {
			for(int i = 0; i < n; i++)
				free(code[i]);
			free(code);

			code = NULL;
		} else
			code[i] = temp_line;

		code[i][temp_j] = '\0';
	}
}

bool strempty(char* string) {
	return !string || !strlen(string);
}

int main() {
	char* c_begin = readLine();
	CHECK_MEM(c_begin);

	char* c_end = readLine();
	CHECK_MEM(c_end);

	int n;
	char** lines = readLines(&n);
	if(n > 0)
		CHECK_MEM(lines);

	if(strempty(c_begin) || strempty(c_end) || n == 0)
		printf("GRESKA\n");
	else {
		removeComments(c_begin, c_end, lines, n);
		CHECK_MEM(lines);

		for(int i = 0; i < n; i++)
			if(!strempty(lines[i]))
				printf("%s\n", lines[i]);
	}

	free(c_begin);
	free(c_end);
	for(int i = 0; i < n; i++)
		free(lines[i]);
	free(lines);

	return 0;
}
