#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct Nod {
	Nod* stanga;
	Nod* dreapta;
	Masina masina;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore) 
		return 1+max(calculeazaInaltimeArbore(arbore->stanga), calculeazaInaltimeArbore(arbore->dreapta));
	return 0;
}

//valori posibile: -2,-1,0,1,2
char gradEchilibru(Nod* arbore) {
	if (arbore)
		return calculeazaInaltimeArbore(arbore->stanga) - calculeazaInaltimeArbore(arbore->dreapta);
	else
		return 0;
}

void rotireStanga(Nod** arbore) {
	if ( !*arbore || !(*arbore)->dreapta)
		return;

	Nod* aux = (*arbore)->dreapta;
	(*arbore)->dreapta = aux->stanga;
	aux->stanga = *arbore;
	*arbore = aux;
}

void rotireDreapta(Nod** arbore) {
	if (!*arbore || !(*arbore)->stanga)
		return;

	Nod* aux = (*arbore)->stanga;
	(*arbore)->stanga = aux->dreapta;
	aux->dreapta = *arbore;
	*arbore = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina m) {
	if (*arbore) {
		if ((*arbore)->masina.id > m.id) 
			adaugaMasinaInArboreEchilibrat(&(*arbore)->stanga, m);
		else
			adaugaMasinaInArboreEchilibrat(&(*arbore)->dreapta, m);

		int grEch = gradEchilibru(*arbore);
		if (grEch == 2){//dezechilibrat in stanga
		
			if (gradEchilibru((*arbore)->stanga)==-1) {//=>2 rotiri
				rotireStanga(&(*arbore)->stanga);
				rotireDreapta(arbore);
			}
			else {
				rotireDreapta(arbore);
			}

		} else 
		
		if (grEch == -2) {

			if (gradEchilibru((*arbore)->dreapta) == -1) {
				rotireDreapta(&(*arbore)->dreapta);
			}
			rotireStanga(arbore);
		}
			
	}
	else {
		Nod* nou = malloc(sizeof(Nod));
		nou->masina = m;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*arbore = nou;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;//niciodata NU se aloca spatiu aici
	while (!feof(f)) {
		adaugaMasinaInArboreEchilibrat(&arbore, citireMasinaDinFisier(f));
	}
	fclose(f);
	return arbore;
}

//preordine - rsd
void afisareMasiniDinArbore(Nod* arbore) {
	if (arbore) {
		afisareMasina(arbore->masina);
		afisareMasiniDinArbore(arbore->stanga);
		afisareMasiniDinArbore(arbore->dreapta);
	}
}

//parcurgere postordine - sdr
void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->stanga);
		dezalocareArboreDeMasini(&(*arbore)->dreapta);

		free((*arbore)->masina.model);
		free((*arbore)->masina.numeSofer);
		free(*arbore);
		
		*arbore = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer);

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);
	dezalocareArboreDeMasini(&arbore);
	return 0;
}