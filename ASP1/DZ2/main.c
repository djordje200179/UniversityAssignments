#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "state.h"
#include "misc.h"
#include "game.h"

int main() {
	int dimension = 3;
	int choice = 1;
	StateNode* initial_state = NULL;
	StateNode* tree = NULL;
	int* all_nums = NULL;

	while(true) {
		printf("Odaberite operaciju:\n");
		printf("   1) Unos informacija\n");
		printf("   2) Generisanje stabla\n");
		printf("   3) Ispis kompletnog stabla\n");
		printf("   4) Ispis rjesenja\n");
		printf("   0) Izlazak iz programa\n");
		scanf("%d", &choice);

		switch(choice) {
		case 1:
			if(initial_state)
				clear_tree(initial_state, dimension);
			if(all_nums)
				free(all_nums);

			printf("Unesite dimenzije kvadrata: ");
			scanf("%d", &dimension);

			initial_state = init_state_node(dimension);
			all_nums = allocate_int_array(dimension * dimension);

			printf("Popunite kvadrat podacima (u prazna mjesta upisite 0): \n");
			for(int i = 0; i < dimension; i++)
				for(int j = 0; j < dimension; j++)
					scanf("%d", &initial_state->values[i][j]);
			printf("\n");

			printf("Unesite preostale brojeve: ");
			for(int i = 0; i < dimension; i++)
				for(int j = 0; j < dimension; j++)
					if(initial_state->values[i][j] != EMPTY_FIELD)
						all_nums[i * dimension + j] = initial_state->values[i][j];
					else
						scanf("%d", &all_nums[i * dimension + j]);
			printf("\n");

			break;
		case 2:
			tree = create_tree(initial_state, dimension, all_nums);

			break;
		case 3:
			if(!tree)
				printf("Pocetno stanje nije validno ili stablo nije generisano\n");
			else
				print_tree(tree, dimension, calc_char_sum(all_nums, dimension));

			break;
		case 4:
			if(!tree)
				printf("Pocetno stanje nije validno ili stablo nije generisano\n");
			else
				print_solutions(tree, dimension, calc_char_sum(all_nums, dimension));

			break;
		case 0:
			if(initial_state)
				clear_tree(initial_state, dimension);
			if(all_nums)
				free(all_nums);

			return 0;
		default:
			printf("Nepoznata operacija! \n");

			break;
		}
	}
}