#include<string.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct Cafea Cafea;
typedef struct Nod Nod;

struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
	};

struct Nod {
	Cafea info;
	Nod* stanga;
	Nod* dreapta;
};

Cafea citireCafeaDinFisier(FILE *f) {
	char buffer[100];
	fgets(buffer, 100, f);
	char sep[3] = "\n,";
	char* aux;
	Cafea c;
	c.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	c.denumire = malloc(strlen(aux) + 1);
	strcpy(c.denumire, aux);
	c.pret = atof(strtok(NULL, sep));
	c.gramaj = atoi(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	c.origine = malloc(strlen(aux) + 1);
	strcpy(c.origine, aux);
	c.marime = *strtok(NULL, sep);
	return c;
}

void afisareCafea(Cafea c) {
	if (c.id <= 0) {
		printf("\nnu extista cafeaua!");
		return;
	}
	printf("\n\nid: %d", c.id);
	printf("\ndenumire: %s", c.denumire);
	printf("\npret: %.2f", c.pret);
	printf("\ngramaj: %d", c.gramaj);
	printf("\norigine: %s", c.origine);
	printf("\nmarime: %c", c.marime);
}

int inaltimeArbore(Nod* arbore) {
	if (arbore)
		return 1+ max( inaltimeArbore(arbore->stanga), inaltimeArbore(arbore->dreapta));
	return 0;
}

int gradEchilibru(Nod* arbore) {
	if (arbore) {
		return inaltimeArbore(arbore->stanga) - inaltimeArbore(arbore->dreapta);
	}
	return 0;
}

void rotireStanga(Nod** arbore) {
	if (*arbore && (*arbore)->dreapta) {
		Nod* aux = (*arbore)->dreapta;
		(*arbore)->dreapta = aux->stanga;
		aux->stanga = *arbore;
		*arbore = aux;
	}
}

void rotireDreapta(Nod** arbore) {
	if (*arbore && (*arbore)->stanga) {
		Nod* aux = (*arbore)->stanga;
		(*arbore)->stanga = aux->dreapta;
		aux->dreapta = *arbore;
		*arbore = aux;
	}
}

void adaugaInArbore(Nod** arbore, Cafea c) {
	if (*arbore) {
		if (c.id < (*arbore)->info.id)
			adaugaInArbore(&(*arbore)->stanga, c);
		else
			adaugaInArbore(&(*arbore)->dreapta, c);

		if (gradEchilibru(*arbore) == 2) {
			if (gradEchilibru((*arbore)->dreapta) == -1)
				rotireStanga(&(*arbore)->dreapta);
			rotireDreapta(arbore);
		}

		if (gradEchilibru(*arbore) == -2) {
			if (gradEchilibru((*arbore)->stanga) == 1)
				rotireDreapta(&(*arbore)->stanga);
			rotireStanga(arbore);
		}
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->stanga = NULL;
		nou->dreapta = NULL;
		nou->info = c;
		*arbore = nou;

	}
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		adaugaInArbore(&arbore, citireCafeaDinFisier(f));
	}
	fclose(f);
	return arbore;
}

//inordine
void afisareArbore(Nod* arbore) {
	if (arbore) {
		afisareArbore(arbore->stanga);
		afisareCafea(arbore->info);
		afisareArbore(arbore->dreapta);
	}
}

int main() {
	Nod* arbore = citireArboreDinFisier("cafele.txt");
	afisareArbore(arbore);
	return 0;
}