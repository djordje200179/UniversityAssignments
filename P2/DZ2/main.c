#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
	int row, column;
	char direction;
} Solution;

int main() {
	int n, m;
	scanf("%d %d", &n, &m);

	if(n <= 0 || m <= 0)
		return 0;

	char* matrix = (char*)malloc(n * m * sizeof(char));

	if(!matrix) {
		printf("MEM_GRESKA");
		return 0;
	}

	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			scanf(" %c", &matrix[i * m + j]);

	char word[50];
	scanf("%s", word);

	int word_len = strlen(word);
	if(word_len > n && word_len > m)
		return 0;

	for(int i = 0; i < word_len; i++)
		word[i] = tolower(word[i]);

	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++) {
			printf("%c", matrix[i * m + j]);
			printf((j < m - 1) ? " " : "\n");
		}

	char new_word[1000];
	Solution solutions[1000];
	int solution_counter = 0;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			if(matrix[i * m + j] == '*')
				continue;

			int startJ = j;
			while(j < m && matrix[i * m + j] != '*') {
				new_word[j - startJ] = tolower(matrix[i * m + j]);
				j++;
			}
			new_word[j - startJ] = '\0';

			if(strcmp(new_word, word) == 0) {
				solutions[solution_counter].row = i;
				solutions[solution_counter].column = startJ;
				solutions[solution_counter].direction = 'v';
				solution_counter++;
			}
		}
	}

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			if(matrix[i * m + j] == '*')
				continue;

			int startI = i;
			while(i < n && matrix[i * m + j] != '*') {
				new_word[i - startI] = tolower(matrix[i * m + j]);
				i++;
			}
			new_word[i - startI] = '\0';

			if(strcmp(new_word, word) == 0) {
				solutions[solution_counter].row = startI;
				solutions[solution_counter].column = j;
				solutions[solution_counter].direction = 'u';
				solution_counter++;
			}

			i = startI;
		}
	}

	for(int i = 0; i < solution_counter; i++)
		printf("(%d, %d, %c)\n", solutions[i].row, solutions[i].column, solutions[i].direction);

	free(matrix);

	return 0;
}