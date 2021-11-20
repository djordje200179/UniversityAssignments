#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#if defined _WIN32 || defined _WIN64 
#define CLEAR_SCREEN "cls"
#elif defined __linux__ || defined __unix__
#define CLEAR_SCREEN "clear"
#else
#define CLEAR_SCREEN ""
#endif

// Struktura koja predstavlja cvor liste 
typedef struct listNode {
	unsigned int exponent;
	double coefficient;

	struct listNode* next_node;
} ListNode;

// Struktura koja predstavlja zaglavlje liste
typedef struct listHeader {
	ListNode* head_node;
	ListNode* tail_node;

	unsigned int number_of_elements;
} ListHeader;

/*
 * Funkcija koja kreira praznu listu
 *
 * @returns Novokreirana lista
 */
ListHeader* create_list() {
	ListHeader* list = (ListHeader*)malloc(sizeof(ListHeader));
	
	if(!list)
		return NULL;

	list->head_node = NULL;
	list->tail_node = NULL;
	list->number_of_elements = 0;

	return list;
}

/*
 * Funkcija koja prazni listu
 *
 * @param list Lista na kojoj se izvrsava operacija
 */
void empty_list(ListHeader* list) {
	ListNode* current_node = list->head_node;
	while(current_node) {
		ListNode* next_node = current_node->next_node;

		free(current_node);
		current_node = next_node;
	}

	list->head_node = NULL;
	list->tail_node = NULL;
	list->number_of_elements = 0;
}

/*
 * Funkcija koja brise clan
 *
 * @param list Lista na kojoj se izvrsava operacija
 * @param exponent Eksponent clana koji se brise
 *
 * @exception 1 Nije moguce pronaci clan
 *
 * @returns Kod greske ili 0 ukoliko je operacija uspjesno izvrsena
 */
unsigned char remove_term(ListHeader* list, unsigned int exponent) {
	ListNode* node;
	for(node = list->head_node; node && node->exponent != exponent; node = node->next_node);

	if(!node)
		return 1;
	
	if(node == list->head_node) {
		list->head_node = node->next_node;

		free(node);
	} else {
		ListNode* prev_node;
		for(prev_node = list->head_node; prev_node->next_node != node; prev_node = prev_node->next_node);

		if(node == list->tail_node)
			list->tail_node = prev_node;

		prev_node->next_node = node->next_node;
		free(node);
	}

	list->number_of_elements--;

	return 0;
}

/*
 * Funkcija koja dodaje clan u listu
 *
 * @param list Lista na kojoj se izvrsava operacija
 * @param exponent Eksponent clana koji se dodaje
 * @param coefficient Koeficijent clana koji se dodaje
 *
 * @exception 1 Nije moguce alocirati prostor za novi cvor
 *
 * @returns Kod greske ili 0 ukoliko je operacija uspjesno izvrsena
 */
unsigned char add_term(ListHeader* list, unsigned int exponent, double coefficient) {
	if(!coefficient)
		return 0;

	ListNode* node = (ListNode*)malloc(sizeof(ListNode));

	if(!node)
		return 1;

	node->exponent = exponent;
	node->coefficient = coefficient;
	node->next_node = NULL;

	if(!list->head_node) {
		list->head_node = list->tail_node = node;

		list->number_of_elements++;
	} else {
		if(node->exponent > list->head_node->exponent) {
			ListNode* next_node = list->head_node;

			node->next_node = next_node;
			list->head_node = node;

			list->number_of_elements++;
		} else if(list->head_node->exponent == node->exponent) {
			list->head_node->coefficient += node->coefficient;

			if(!node->coefficient)
				remove_term(list, node->exponent);
		} else {
			ListNode* prev_node;
			for(prev_node = list->head_node;
				prev_node->next_node && prev_node->next_node->exponent > node->exponent;
				prev_node = prev_node->next_node);
			ListNode* next_node = prev_node->next_node;

			if(!next_node) {
				prev_node->next_node = node;
				node->next_node = next_node;
				list->tail_node = node;

				list->number_of_elements++;
			} else if(next_node->exponent == node->exponent) {
				next_node->coefficient += node->coefficient;

				if(!next_node->coefficient)
					remove_term(list, next_node->exponent);
			} else {
				prev_node->next_node = node;
				node->next_node = next_node;

				list->number_of_elements++;
			}
		}
	}

	return 0;
}

/*
 * Funkcija koja racuna polinom
 *
 * @param polynomial Polinom na kom se izvrsava operacija
 * @param x Nezavisna promjenjiva
 *
 * @returns Vrijednost polinoma za unijetu nezavisnu promjenjivu
 */
double calculate_polynomial(ListHeader* polynomial, double x) {
	double result = 0;

	for(ListNode* current_node = polynomial->head_node; current_node; current_node = current_node->next_node)
		result += current_node->coefficient * pow(x, current_node->exponent);

	return result;
}

/*
 * Funkcija koja ispisuje polinom
 *
 * @param polynomial Polinom na kom se izvrsava operacija
 */
void print_polynomial(ListHeader* polynomial) {
	if(!polynomial->head_node) {
		printf("--PRAZAN POLINOM--\n");
		return;
	}

	bool first_printed = false;

	for(ListNode* current_node = polynomial->head_node; current_node; current_node = current_node->next_node) {
		if(current_node->coefficient >= 0) {
			if(first_printed)
				printf("+");

			switch(current_node->exponent) {
				case 0:
					printf(" %.2lf ", current_node->coefficient);
					break;
				case 1:
					printf(" %.2lf * x ", current_node->coefficient);
					break;
				default:
					printf(" %.2lf * x^%u ", current_node->coefficient, current_node->exponent);
					break;
			}
		} else {
			printf("-");

			switch(current_node->exponent) {
				case 0:
					printf(" %.2lf ", -current_node->coefficient);
					break;
				case 1:
					printf(" %.2lf * x ", -current_node->coefficient);
					break;
				default:
					printf(" %.2lf * x^%u ", -current_node->coefficient, current_node->exponent);
					break;
			}
		}

		first_printed = true;
	}

	printf("\n");
}

/*
 * Funkcija koja sabira dva polinoma
 *
 * @param polynomial1 Prvi polinom
 * @param polynomial2 Drugi polinom
 *
 * @returns Polinom koji predstavlja zbir clanova unijeta 2 polinoma
 */
ListHeader* sum_polynomials(ListHeader* polynomial1, ListHeader* polynomial2) {
	ListHeader* result_polynomial = create_list();

	for(ListNode* current_node = polynomial1->head_node; current_node; current_node = current_node->next_node)
		add_term(result_polynomial, current_node->exponent, current_node->coefficient);

	for(ListNode* current_node = polynomial2->head_node; current_node; current_node = current_node->next_node)
		add_term(result_polynomial, current_node->exponent, current_node->coefficient);

	return result_polynomial;
}

/*
 * Funkcija koja mnozi dva polinoma
 *
 * @param polynomial1 Prvi polinom
 * @param polynomial2 Drugi polinom
 *
 * @returns Polinom koji predstavlja proizvod clanova unijeta 2 polinoma
 */
ListHeader* multiply_polynomials(ListHeader* polynomial1, ListHeader* polynomial2) {
	ListHeader* result_polynomial = create_list();

	for(ListNode* current_node_1 = polynomial1->head_node; current_node_1; current_node_1 = current_node_1->next_node)
		for(ListNode* current_node_2 = polynomial2->head_node; current_node_2; current_node_2 = current_node_2->next_node)
			add_term(result_polynomial,
					 current_node_1->exponent + current_node_2->exponent,
					 current_node_1->coefficient * current_node_2->coefficient);

	return result_polynomial;
}

int main() {
	ListHeader* polynomials[5];
	for(unsigned char i = 0; i < 5; i++)
		polynomials[i] = create_list();

	printf("DZ1P1: Rad sa polinomima \n\n");

	while(true) {
		printf("Odaberite operaciju: \n");
		printf("1) Unos polinoma \n");
		printf("2) Dodavanje clana polinoma \n");
		printf("3) Brisanje clana polinoma \n");
		printf("4) Ispis polinoma \n");
		printf("5) Sabiranje polinoma \n");
		printf("6) Mnozenje polinoma \n");
		printf("7) Racunanje polinoma \n");
		printf("8) Brisanje polinoma \n");
		printf("0) Izlaz iz programa \n");

		char choice;
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &choice);

		system(CLEAR_SCREEN);

		switch(choice) {
			case '1':
			{
				int index;
				printf("Odaberite polinom na kom ce se izvrsavati operacija (index): ");
				scanf("%d", &index);
				index--;

				if(index < 0 || index > 4)
					printf("Greska pri odabiru polioma! \n");
				else {
					empty_list(polynomials[index]);

					unsigned int n;
					printf("Unesesite broj clanova polinoma (n): ");
					scanf("%u", &n);

					printf("Unesite clanove polinoma (a*x^b): \n");
					for(unsigned int i = 0; i < n; i++) {
						unsigned int exponent;
						double coefficient;
						scanf("%lf*x^%u", &coefficient, &exponent);

						add_term(polynomials[index], exponent, coefficient);
					}
				}

				break;
			}
			case '2':
			{
				int index;
				printf("Odaberite polinom na kom ce se izvrsavati operacija (index): ");
				scanf("%d", &index);
				index--;

				if(index < 0 || index > 4)
					printf("Greska pri odabiru polioma! \n");
				else {
					unsigned int exponent;
					double coefficient;
					printf("Unesite clan polinoma (a*x^b): ");
					scanf("%lf*x^%u", &coefficient, &exponent);

					add_term(polynomials[index], exponent, coefficient);
				}

				break;
			}
			case '3':
			{
				int index;
				printf("Odaberite polinom na kom ce se izvrsavati operacija (index): ");
				scanf("%d", &index);
				index--;

				if(index < 0 || index > 4)
					printf("Greska pri odabiru polioma! \n");
				else {
					unsigned int exponent;
					printf("Unesite eksponent (b): ");
					scanf("%u", &exponent);

					remove_term(polynomials[index], exponent);
				}

				break;
			}
			case '4':
			{
				int index;
				printf("Odaberite polinom na kom ce se izvrsavati operacija (index): ");
				scanf("%u", &index);
				index--;

				if(index < 0 || index > 4)
					printf("Greska pri odabiru polioma! \n");
				else
					print_polynomial(polynomials[index]);

				break;
			}
			case '5':
			{
				int index1, index2, index3;
				printf("Odaberite polinome na kojima ce se izvrsavati operacija (index1 index2): ");
				scanf("%u %u", &index1, &index2);
				printf("Odaberite polinom za smjestanje (index3): ");
				scanf("%u", &index3);
				index1--; index2--; index3--;

				if(index1 < 0 || index1 > 4 || index2 < 0 || index2 > 4 || index3 < 0 || index3 > 4)
					printf("Greska pri odabiru polioma! \n");
				else {
					empty_list(polynomials[index3]);
					polynomials[index3] = sum_polynomials(polynomials[index1], polynomials[index2]);
				}

				break;
			}
			case '6':
			{
				int index1, index2, index3;
				printf("Odaberite polinome na kojima ce se izvrsavati operacija (index1 index2): ");
				scanf("%d %d", &index1, &index2);
				printf("Odaberite polinom za smjestanje (index3): ");
				scanf("%d", &index3);
				index1--; index2--; index3--;

				if(index1 < 0 || index1 > 4 || index2 < 0 || index2 > 4 || index3 < 0 || index3 > 4)
					printf("Greska pri odabiru polioma! \n");
				else {
					empty_list(polynomials[index3]);
					polynomials[index3] = multiply_polynomials(polynomials[index1], polynomials[index2]);
				}

				break;
			}
			case '7':
			{
				int index;
				printf("Odaberite polinom na kom ce se izvrsavati operacija (index): ");
				scanf("%d", &index);
				index--;

				if(index < 0 || index > 4)
					printf("Greska pri odabiru polioma! \n");
				else {
					double x;
					printf("Unesite nezavisnu promjenjivu (x): ");
					scanf("%lf", &x);

					printf("%lf\n", calculate_polynomial(polynomials[index], x));
				}

				break;
			}
			case '8':
			{
				int index;
				printf("Odaberite polinom na kom ce se izvrsavati operacija (index): ");
				scanf("%d", &index);
				index--;

				if(index < 0 || index > 4)
					printf("Greska pri odabiru polioma! \n");
				else
					empty_list(polynomials[index]);

				break;
			}
			case '0':
			{
				for(unsigned char i = 0; i < 5; i++) {
					empty_list(polynomials[i]);
					free(polynomials[i]);
				}

				return 0;
			}
			default:
				printf("Greska pri odabiru opcije! \n");
		}

		printf("\n");
	}

	return 0;
}
