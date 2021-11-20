#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#if defined _WIN32 || defined _WIN64 
#define CLEAR_SCREEN "cls"
#elif defined __linux__ || defined __unix__
#define CLEAR_SCREEN "clear"
#else
#define CLEAR_SCREEN ""
#endif

// Tip koji predstavlja smjer
typedef enum department { ER, SI } Department;

// Struktura koja predstavlja cvor liste 
typedef struct listNode {
	char name[50], surname[50], student_id[10];
	Department department;
	unsigned int current_year;

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
 * Funkcija koja izbacuje cvor iz liste
 *
 * @param list Lista na kojoj se izvrsava operacija
 * @param index Indeks cvor koji se izbacuje
 *
 * @returns Trazeni cvor
 */
ListNode* pop_node(ListHeader* list, unsigned int index) {
	ListNode* node;

	if(!index) {
		node = list->head_node;

		list->head_node = node->next_node;
		if(list->head_node == NULL)
			list->tail_node = NULL;
	} else {
		ListNode* prev_node = list->head_node;
		for(unsigned int i = 1; i < index; i++)
			prev_node = prev_node->next_node;

		node = prev_node->next_node;
		prev_node->next_node = node->next_node;
	}

	list->number_of_elements--;
	node->next_node = NULL;

	return node;
}

/*
 * Funkcija koja dodaje cvor na kraj liste
 *
 * @param list Lista na kojoj se izvrsava operacija
 * @param node Cvor koji se dodaje
 *
 * @exception 1 Nije moguce alocirati prostor za novi cvor
 *
 * @returns Kod greske ili 0 ukoliko je operacija uspjesno izvrsena
 */
unsigned char push_node(ListHeader* list, ListNode* node) {
	if(!node)
		return 1;

	if(!list->head_node)
		list->head_node = list->tail_node = node;
	else {
		list->tail_node->next_node = node;
		list->tail_node = node;
	}

	list->number_of_elements++;

	return 0;
}

/*
 * Funkcija koja kreira cvor sa studentom
 *
 * @param name Ime studenta
 * @param surname Prezime studenta
 * @param student_id Broj indeksa
 * @param department Smjer studenta
 * @param current_year Trenutna godina studija
 *
 * @returns Novokreirani cvor
 */
ListNode* create_node(char name[], char surname[], char student_id[], Department department, unsigned int current_year) {
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));

	if(!node)
		return NULL;

	strcpy(node->name, name);
	strcpy(node->surname, surname);
	strcpy(node->student_id, student_id);
	node->department = department;
	node->current_year = current_year;
	node->next_node = NULL;

	return node;
}

/*
 * Funkcija koja vraca nasumican broj izmedju 0 i max
 *
 * @param max Maksimalni broj
 *
 * @returns Novokreirani nasumicni broj
 */
unsigned int random_int(unsigned int max_num) {
	return rand() % max_num;
}

/*
 * Funkcija koja nasumicno vraca vrijednost tacno ili netacno
 *
 * @param possibility Vjerovatnoca da bude vracena vrijednost netacno
 *
 * @returns Nasumicna vrijednost tacno ili netacno
 */
bool random_bool(double possibility) {
	double random_double = (double)rand() / RAND_MAX;

	return random_double > possibility;
}

int main() {
	srand(time(NULL));
	ListHeader* students = create_list();
	ListHeader* queue = create_list();

	printf("DZ1P2: Simulacija upisa studenata \n\n");

	while(true) {
		printf("Odaberite operaciju: \n");
		printf("1) Unos studenata \n");
		printf("2) Pravljenje liste cekanja \n");
		printf("3) Upis studenata \n");
		printf("0) Izlaz iz programa \n");

		char choice;
		fseek(stdin, 0, SEEK_END);
		scanf("%c", &choice);

		system(CLEAR_SCREEN);

		switch(choice) {
			case '1':
			{
				empty_list(students);

				int n;

				printf("Unesesite broj studenata (n): ");
				scanf("%d", &n);

				printf("Unesite podatke o studentima (ime | prezime | broj indeksa | smjer | trenutna godina studija): \n");
				for(int i = 0; i < n; i++) {
					char name[30], surname[30], student_id[10], department[3];
					unsigned int current_year;
					scanf("%s %s %s %s %d", name, surname, student_id, department, &current_year);

					ListNode* student = create_node(name, surname, student_id, department == "SI" ? SI : ER, current_year);
					push_node(students, student);
				}

				printf("Uneseni su podaci od %u studenata.\n", students->number_of_elements);

				break;
			}
			case '2':
			{
				empty_list(queue);

				while(students->number_of_elements) {
					unsigned int index = random_int(students->number_of_elements);
					ListNode* student = pop_node(students, index);
					push_node(queue, student);
				}

				break;
			}
			case '3':
			{
				double x;
				printf("Unesesite vjerovatnocu da student ne bude upisan (x): ");
				scanf("%lf", &x);

				if(x < 0 || x > 0.5)
					printf("Greska pri unosu vjerovatnoce! \n");
				else {
					unsigned int counter = 0;

					while(queue->number_of_elements) {
						bool is_enrolled = random_bool(x);

						ListNode* student = pop_node(queue, 0);

						if(is_enrolled) {
							printf("%s, %s, %d\n", student->name, student->surname, student->current_year + 1);

							free(student);
						} else
							push_node(queue, student);

						counter++;
					}

					printf("\n");
					printf("Broj koraka: %u\n", counter);
				}

				break;
			}
			case '0':
			{
				empty_list(students);
				empty_list(queue);

				free(students);
				free(queue);

				return;
			}
			default:
				printf("Greska pri odabiru opcije! \n");
		}

		printf("\n");
	}

	return 0;
}
