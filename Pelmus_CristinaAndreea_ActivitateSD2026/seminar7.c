#define _CRT_SECURE_NO_WARNINGS
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

//tabela de dispersie - vector de liste
//hash - functie matematica ce asigura unicitatea standardizata unui element

typedef struct Nod Nod;
struct Nod {
	Nod* next;
	Masina info;
};

struct HashTable {
	int dim;
	Nod** cap;//alte nume: vector, tabela, lista 
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod *cap) {
	while (cap) {
		//daca scriam while(cap->next) nu ajungea la ultimul, s ar fi oprit la penultimul
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista( Nod**cap, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*cap) == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.cap = malloc(sizeof(Nod*) * ht.dim);
	for (int i = 0; i < ht.dim; i++)
		ht.cap[i] = NULL;
	return ht;
}

//clusterizare=grupare
int calculeazaHash(int id,int dimensiune) {
	return (id * 31) % dimensiune;//inmultit cu nr prim, coliziunea e mai mica
}

void inserareMasinaInTabela(HashTable ht, Masina masina) {
	int hash = calculeazaHash(masina.id, ht.dim);
	if (ht.cap[hash] != NULL) {
		adaugaMasinaInLista(&(ht.cap[hash]), masina);
		return;
	}
	Nod* aux = malloc(sizeof(Nod));
	aux->info = masina;
	aux->next = NULL;
	ht.cap[hash]=aux;
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, masina);
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.cap[i] != NULL) {
			printf("\n\nCLUSTER %d:\n", i + 1);
			afisareListaMasini(ht.cap[i]);
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++) {
		if (ht->cap[i] != NULL) {
			Nod* p = ht->cap[i];
			Nod* next = NULL;
			while (p != NULL) {
				free(p->info.model);
				free(p->info.numeSofer);
				next = p->next;
				free(p);
				p = next;
			}
		}
	}
	free(ht->cap);
	ht->cap = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	int nr = 0;
	for (int i = 0; i < ht.dim; i++)
		if (ht.cap[i] != NULL)nr++;

	float* mediePerCluster = malloc(sizeof(float) * nr);
	for (int i = 0; i < ht.dim; i++) {
		if (ht.cap[i] != NULL) {
			int nrNoduri = 0;
			float suma = 0;
			Nod* aux = ht.cap[i];
			int j = 0;
			while (aux != NULL) {
				nrNoduri++;
				suma += aux->info.pret;
				aux = aux->next;
			}
			mediePerCluster[j++] = suma / nrNoduri;
		}
	}
	return mediePerCluster;
}

Masina getMasinaDupaId(HashTable ht, int id) {
	Masina m;
	int hash = calculeazaHash(id, ht.dim);
	Nod* aux = ht.cap[hash];
	while (aux != NULL) {
		if (aux->info.id == id) {
			m = aux->info;
			m.model = malloc(strlen(aux->info.model));
			strcpy(m.model, aux->info.model);
			m.numeSofer = malloc(strlen(aux->info.numeSofer));
			strcpy(m.numeSofer, aux->info.numeSofer);

			return m;
		}
	}
	m.id = -1;
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(ht);
	afisareMasina(getMasinaDupaId(ht, 3));
	int nrClustere;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0; i < nrClustere; i++)
		printf("media din clusterul %d este: %.2f", i + 1, medii[i]);
	dezalocareTabelaDeMasini(&ht);
	return 0;
}