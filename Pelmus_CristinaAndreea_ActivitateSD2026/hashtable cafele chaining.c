#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
};

typedef struct Cafea Cafea;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Nod {
	Cafea info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};
Cafea citireCafeaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	Cafea c;
	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	aux = strtok(NULL, sep);
	c.denumire = malloc(strlen(aux)+1);
	strcpy(c.denumire, aux);
	c.pret = atof(strtok(NULL, sep));
	c.gramaj = atoi(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	c.origine = malloc(strlen(aux)+1);
	strcpy(c.origine, aux);
	c.marime = *strtok(NULL, sep);//strtok returneaza char*, ca sa luam primul caracter trb *strtok
	return c;
}

void afisareCafea(Cafea c) {
	printf("\nId: %d", c.id);
	printf("\nDenumire: %s", c.denumire);
	printf("\nPret: %.2f", c.pret);
	printf("\nGramaj: %d", c.gramaj);
	printf("\nOrigine: %s", c.origine);
	printf("\nMarime: %c", c.marime);
}

void adaugaCafeaInLista(Nod** ls, Cafea c) {
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = c;
	if (*ls != NULL) {
		Nod* aux = *ls;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
		*ls = nou;
}

void afiseazaListaCafele(Nod* ls) {
	while (ls != NULL) {
		afisareCafea(ls->info);
		ls = ls->next;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.vector= malloc(sizeof(Nod*) * dimensiune);
	ht.dim = dimensiune;
	for (int i = 0; i < ht.dim; i++) {
		ht.vector[i]= NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return id% dimensiune;
}

void inserareInHashTable(Cafea c, HashTable ht) {
	int hash = calculeazaHash(c.id,ht.dim);
	if (ht.vector[hash]!=NULL) {
		adaugaCafeaInLista(&(ht.vector[hash]), c);
		return;
	}

	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = c;
	ht.vector[hash] = nou;
}


HashTable citireHashTableDinFisier(const char *numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(7);
	while (!feof(f)) {
		inserareInHashTable(citireCafeaDinFisier(f), ht);
	}
	fclose(f);
	return ht;
}

void afisareHashTable(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
		if (ht.vector[i] != NULL) {
			printf("\n\nCluster %d\n", i + 1);
			afiseazaListaCafele(ht.vector[i]);
		}
}

void dezalocareTabela(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++) {
		if (ht->vector[i] != NULL) {
			Nod* aux = ht->vector[i];
			Nod* next = NULL;
			while (aux) {
				free(aux->info.denumire);
				free(aux->info.origine);
				next = aux->next;
				free(aux);
				aux = next;
			}
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

Cafea getCafeaDupaId(HashTable ht, int id) {
	Cafea c;
	c.id = -1;
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vector[i];
		while (p) {
			if (p->info.id == id) {
				c = p->info;
				c.denumire = malloc(strlen(p->info.denumire) + 1);
				strcpy(c.denumire, p->info.denumire);
				c.origine = malloc(strlen(p->info.origine) + 1);
				strcpy(c.origine, p->info.origine);
			}
			p = p->next;
		}
	}
	return c;
}
int main() {
	HashTable ht = citireHashTableDinFisier("cafele.txt");
	afisareHashTable(ht);
	Cafea c = getCafeaDupaId(ht, 8);
	printf("\nCafeaua cu id ul 8 este: ");
	afisareCafea(c);
	free(c.denumire);
	free(c.origine);
	dezalocareTabela(&ht);
	return 0;
}