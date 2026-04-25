//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//
//typedef struct Cafea Cafea;
//typedef struct Nod Nod;
//
//struct Cafea {
//	int id;
//	char* denumire;
//	float pret;
//	int gramaj;
//	char* origine;
//	char marime;
//	};
//
////stiva prin LS
//
//struct Nod {
//	Cafea info;
//	Nod* next;
//};
//
//Cafea citireCafeaDinFisier(FILE *f) {
//	char buffer[100];
//	fgets(buffer, 100, f);
//	char sep[3] = "\n,";
//	char* aux;
//	Cafea c;
//	c.id = atoi(strtok(buffer, sep));
//	aux = strtok(NULL, sep);
//	c.denumire = malloc(strlen(aux) + 1);
//	strcpy(c.denumire, aux);
//	c.pret = atof(strtok(NULL, sep));
//	c.gramaj = atoi(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	c.origine = malloc(strlen(aux) + 1);
//	strcpy(c.origine, aux);
//	c.marime = *strtok(NULL, sep);
//	return c;
//}
//
//void afisareCafea(Cafea c) {
//	if (c.id <= 0) {
//		printf("\nnu extista cafeaua!");
//		return;
//	}
//	printf("\n\nid: %d", c.id);
//	printf("\ndenumire: %s", c.denumire);
//	printf("\npret: %.2f", c.pret);
//	printf("\ngramaj: %d", c.gramaj);
//	printf("\norigine: %s", c.origine);
//	printf("\nmarime: %c", c.marime);
//}
//
////adaugare la inceput in ls
//void pushStack(Nod** stack, Cafea c) {
//	Nod* nou = malloc(sizeof(Nod));
//	nou->info = c;
//	nou->next = *stack;
//	*stack = nou;
//}
//
////eliminare de la inceput
//Cafea popStack(Nod** stack) {
//	Cafea c;
//	c.id = -1;
//	if (*stack != NULL) {
//		Nod* aux = *stack;
//		*stack = (*stack)->next;
//		c = aux->info;
//		free(aux);
//	}
//	return c;
//}
//
//char emptyStack(Nod* stack) {
//	//1 - stiva e goala
//	//0 - stiva nu e goala
//	return !stack;
//}
//
//Nod* citireCafeleInStiva(const char* numeFisier) {
//	FILE* f = fopen(numeFisier, "r");
//	Nod* stack = NULL;
//	while (!feof(f)) {
//		pushStack(&stack, citireCafeaDinFisier(f));
//	}
//	fclose(f);
//	return stack;
//}
//
//void dezalocareStiva(Nod** stack) {
//	while (*stack != NULL) {
//		Nod* aux = *stack;
//		*stack = (*stack)->next;
//		free(aux->info.denumire);
//		free(aux->info.origine);
//		free(aux);
//	}
//}
//
//int size(Nod** stack) {
//	int nr = 0;
//	Nod* aux = NULL;
//	while (*stack != NULL) {
//		pushStack(&aux, popStack(stack));
//		nr++;
//	}
//	while (aux != NULL)
//		pushStack(stack, popStack(&aux));
//
//	return nr;
//}
//
//float calculPretTotal(Nod** stack) {
//	Nod* aux = NULL;
//	float suma = 0;
//
//	while (*stack != NULL) {
//		Cafea c = popStack(stack);
//		suma +=c.pret;
//		pushStack(&aux, c);
//	}
//
//	while (aux != NULL)
//		pushStack(stack, popStack(&aux));
//
//	return suma;
//}
//
//int main() {
//	Nod* s = citireCafeleInStiva("cafele.txt");
//	afisareCafea(popStack(&s));
//	afisareCafea(popStack(&s));
//	float total = calculPretTotal(&s);
//	printf("\nPretul total este: %.2f!", total);
//	int nr = size(&s);
//	printf("\nStiva contine %d cafele!", nr);
//	return 0;
//}