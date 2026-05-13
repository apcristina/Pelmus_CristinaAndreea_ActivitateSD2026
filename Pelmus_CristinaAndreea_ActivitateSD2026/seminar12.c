#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//heap, avl,abc, ce am fct azi

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

typedef struct NodP NodP;
typedef struct NodS NodS;
struct NodP {
	Masina info;
	NodP* next;
	NodS* vecini;
};

struct NodS{
	NodP* info;
	NodS* next;
};

void inserareListaPrincipala(NodP** graf, Masina m ) {
	NodP* nou = malloc(sizeof(NodP));
	nou->next=NULL;
	nou->vecini = NULL;
	nou->info = m;

	if ((*graf) == NULL) {
		(*graf) = nou;
	}
	else {
		NodP* p = (*graf);
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = nou;
	}
}

void inserareListaSecundara(NodS** cap, NodP* vecin) {
	NodS*nou = malloc(sizeof(NodS));
	nou->next = NULL;
	nou->info = vecin;
	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		NodS* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}

NodP* cautaNodDupaID(NodP* graf, int id) {
		while (graf != NULL&&graf->info.id != id) {
			graf = graf->next;
		}
		return graf;
	return NULL;
}

void inserareMuchie(NodP*graf, int idStart, int idStop) {
	NodP* nodStart=cautaNodDupaID(graf, idStart);
	NodP* nodStop=cautaNodDupaID(graf, idStop);
	if (nodStart && nodStop) {
		inserareListaSecundara(&nodStart->vecini, nodStop);
		inserareListaSecundara(&nodStop->vecini, nodStart);
	}
}


void* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;
	while (!feof(f)) {
		inserareListaPrincipala(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP*graf) {
	FILE* f = fopen(numeFisier, "r");
	int start;
	int stop;
	while (!feof(f)) {
		fscanf(f, "%d %d", &start, &stop);
		inserareMuchie(graf, start, stop);
	}
	fclose(f);
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
}

void afiseazaVecini(int id,NodP*graf) {
	NodP* p = cautaNodDupaID(graf, id);
	NodS* s = p->vecini;
	while (s) {
		afisareMasina(s->info->info);
		s = s->next;
	}
}

int main() {

	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);
	afiseazaVecini(3, graf);
	return 0;
}