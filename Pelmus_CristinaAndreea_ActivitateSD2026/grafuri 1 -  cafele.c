#include<string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct Cafea Cafea;
typedef struct NodP NodP;
typedef struct NodS NodS;

struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
};

struct NodP {
	NodS* vecini;
	Cafea info;
	NodP* next;
};

struct NodS {
	NodS* next;
	NodP* info;
};

Cafea citireCafeaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	Cafea c;
	char* aux;

	c.id = atoi(strtok(buffer, sep));

	aux = strtok(NULL, sep);
	c.denumire =malloc(strlen(aux)+1);
	strcpy(c.denumire, aux);

	c.pret = atof(strtok(NULL, sep));
	c.gramaj = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.origine = malloc(strlen(aux) + 1);
	strcpy(c.origine, aux);

	c.marime = *strtok(NULL, sep);

	return c;
}

void adaugaInListaPrincipala(NodP** cap, Cafea c) {
	NodP* nou = malloc(sizeof(NodP));
	nou->info = c;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*cap) {
		NodP* p = *cap;
		while (p->next)
			p = p->next;
		p->next = nou;
	}
	else
		*cap = nou;
}

void adaugaInListaSecundara(NodS** cap, NodP* p) {
	NodS* nou = malloc(sizeof(NodS));
	nou->next = NULL;
	nou->info = p;
	if (*cap) {
		NodS* s = *cap;
		while (s->next)
			s = s->next;
		s->next = nou;
	}
	else
		*cap = nou;
}

NodP* citesteGrafDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* graf=NULL;
	while (!feof(f)) {
		adaugaInListaPrincipala(&graf, citireCafeaDinFisier(f));
	}
	fclose(f);
	return graf;
}

NodP* getNodDupaId(NodP* graf, int id) {
	while (graf && graf->info.id != id)
		graf = graf->next;
	return graf;
}
void inserareMuchie(int start, int stop, NodP* graf) {
	NodP *nodStart = getNodDupaId(graf, start);
	NodP* nodStop = getNodDupaId(graf, stop);
	if (nodStart && nodStop) {
		adaugaInListaSecundara(&nodStart->vecini, nodStop);
		adaugaInListaSecundara(&nodStop->vecini, nodStart);
	}
}

void citesteMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");
	int start, stop;
	while (!feof(f)) {
		fscanf(f, "%d %d", &start, &stop);
		inserareMuchie(start, stop, graf);
	}
	fclose(f);
}

void afisareCafea(Cafea c) {
	printf("\n\nid: %d", c.id);
	printf("\ndenumire: %s", c.denumire);
	printf("\npret: %.2f", c.pret);
	printf("\ngramaj: %d", c.gramaj);
	printf("\norigine: %s", c.origine);
	printf("\nmarime: %c", c.marime);
}

void afiseazaVecini(NodP* graf, int id) {
	while (graf && graf->info.id != id)
		graf = graf->next;
	if(graf)
		while (graf->vecini) {
			afisareCafea(graf->vecini->info->info);
			graf->vecini = graf->vecini->next;
		}
}
int main() {
	NodP* graf = citesteGrafDinFisier("cafele.txt");
	citesteMuchiiDinFisier("muchii.txt", graf);
	afiseazaVecini(graf, 5);
}