#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Cafea Cafea;
typedef struct HashTable HashTable;

struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
	};

struct HashTable {
	Cafea* vector;
	int dim;
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

HashTable initializareHashTable( int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = malloc(sizeof(Cafea) * ht.dim);
	for (int i = 0; i < ht.dim; i++) {
		ht.vector[i].denumire = NULL;
		ht.vector[i].origine = NULL;
		ht.vector[i].id = -1;
	}
	return ht;
}

int calculeazaHash(const char* denumire, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(denumire); i++)
		suma += denumire[i];
	return suma % dimensiune;
}

int inserareCafeaInTabela(HashTable* ht, Cafea c) {
	int hash = calculeazaHash(c.denumire, ht->dim);
	if (ht->vector[hash].id == -1) {
		ht->vector[hash] = c;
		ht->vector[hash].denumire = malloc(strlen(c.denumire)+1);
		strcpy(ht->vector[hash].denumire, c.denumire);
		ht->vector[hash].origine= malloc(strlen(c.origine)+1);
		strcpy(ht->vector[hash].origine, c.origine);
		return 1;
	}
	else {
		for (int i = hash + 1; i < ht->dim; i++) {
			if (ht->vector[i].id == -1) {
				ht->vector[i] = c;
				ht->vector[i].denumire = malloc(strlen(c.denumire)+1);
				strcpy(ht->vector[i].denumire, c.denumire);
				ht->vector[i].origine = malloc(strlen(c.origine)+1);
				strcpy(ht->vector[i].origine, c.origine);
				return 1;
			}
		}

		for (int i = 0; i < hash; i++) {
			if (ht->vector[i].id == -1) {
				ht->vector[i] = c;
				ht->vector[i].denumire = malloc(strlen(c.denumire) + 1);
				strcpy(ht->vector[i].denumire, c.denumire);
				ht->vector[i].origine = malloc(strlen(c.origine) + 1);
				strcpy(ht->vector[i].origine, c.origine);
				return 1;
			}
		}
	}
	return 0;
}

HashTable citireCafeleDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(10);
	while (!feof(f)) {
		int ok = inserareCafeaInTabela(&ht, citireCafeaDinFisier(f));
		if (ok == 0) {
			printf("Nu au incaput toate cafelele!");
			break;
		}
		inserareCafeaInTabela(&ht, citireCafeaDinFisier(f));
	}
	fclose(f);
	return ht;
}

void afisareCafele(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
		if (ht.vector[i].id != -1)
			afisareCafea(ht.vector[i]);
}

void dezalocare(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		free(ht->vector[i].denumire);
		free(ht->vector[i].origine);
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

Cafea getCafeaDupaId(HashTable ht, int id) {
	Cafea c;
	c.id = -1;
	for(int i=0;i<ht.dim;i++)
		if (ht.vector[i].id == id) {
			c = ht.vector[i];
			c.denumire = malloc(strlen(ht.vector[i].denumire) + 1);
			strcpy(c.denumire, ht.vector[i].denumire);
			c.origine = malloc(strlen(ht.vector[i].origine) + 1);
			strcpy(c.origine, ht.vector[i].origine);
		}
	return c;
}

int main() {
	HashTable ht = citireCafeleDinFisier("cafele.txt");
	afisareCafele(ht);
	printf("\n\nafisare cafea cu id ul 8: ");
	Cafea c = getCafeaDupaId(ht, 8);
	afisareCafea(c);
	free(c.denumire);
	free(c.origine);
	dezalocare(&ht);
	return 0;
}