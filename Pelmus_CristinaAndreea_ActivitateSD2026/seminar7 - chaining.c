#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Zbor Zbor;
struct Zbor {
	int id;
	char* plecare;
	char* destinatie;
	char* companie;
	float durata;
	float pret;
};

typedef struct Nod Nod;
struct Nod {
	Nod* next;
	Zbor info;
};

typedef struct HashTable HashTable;
struct HashTable {
	int dim;
	Nod** vector;
};

Zbor citireZborDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = "\n;";
	fgets(buffer, 100, f);
	char* aux;
	Zbor z;
	aux = strtok(buffer, sep);
	z.id = atoi(aux);

	aux = strtok(NULL, sep);
	z.plecare = malloc(strlen(aux) + 1);
	strcpy(z.plecare, aux);

	aux = strtok(NULL, sep);
	z.destinatie = malloc(strlen(aux) + 1);
	strcpy(z.destinatie, aux);

	aux = strtok(NULL, sep);
	z.companie = malloc(strlen(aux) + 1);
	strcpy(z.companie, aux);

	z.durata = atof(strtok(NULL, sep));
	z.pret = atof(strtok(NULL, sep));

	return z;
}

void afisareZbor(Zbor z) {
	printf("\nId: %d", z.id);
	printf("\nPlecare: %s", z.plecare);
	printf("\nDestnatie: %s", z.destinatie);
	printf("\nCompanie: %s", z.companie);
	printf("\nDurata: %.2f", z.durata);
	printf("\nPret: %.2f", z.pret);
}

void afisareListaZboruri(Nod* cap) {
	while (cap) {
		afisareZbor(cap->info);
		cap = cap->next;
	}
}

void adaugaZborInLista(Nod** cap, Zbor zNou) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = zNou;
	nou->next = NULL;
	if ((*cap) == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next!=NULL)
			aux = aux->next;
		aux->next = nou;
	}
}

HashTable initializareHashTable(int dimensiune) {//dimesiunea vectorului
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = malloc(sizeof(Nod*) * ht.dim);
	for (int i = 0; i < ht.dim; i++)
		ht.vector[i] = NULL;
	return ht;
}

int calculeazaHash(char* destinatie, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(destinatie); i++)
		suma += destinatie[i];
	return suma*11%dimensiune;
}

void inserareZborInTabela(HashTable ht, Zbor z) {
	int hash = calculeazaHash(z.companie, ht.dim);//index in vectorul de liste
	if (ht.vector[hash] != NULL) {//daca lista de pe pozitia indicata de hash nu e goala
		adaugaZborInLista(&(ht.vector[hash]), z);//se apeleaza functia pt adaugare in lista cu primul param
		//adresa primul nod din lista
		return;
	}

	Nod* nou = malloc(sizeof(Nod));
	nou->info = z;
	nou->next = NULL;
	ht.vector[hash] = nou;
}

HashTable citireZboruriDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(7);
	while (!feof(f)) {
		Zbor z = citireZborDinFisier(f);
		inserareZborInTabela(ht, z);
	}
	fclose(f);
	return ht;
}

void afisareTabelaZboruri(HashTable ht) {
	for( int i = 0; i< ht.dim;i++)
		if (ht.vector[i] != NULL) {
			printf("\n\nCluster %d:\n", i + 1);
			afisareListaZboruri(ht.vector[i]);
		}
}

void dezalocareTabelaZboruri(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		if (ht->vector[i] != NULL) {
			Nod* p = ht->vector[i];
			Nod* next = NULL;
			while (p != NULL) {
				free(p->info.companie);
				free(p->info.destinatie);
				free(p->info.plecare);
				next = p->next;
				free(p);
				p = next;
			}
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

int nrTotalDeZboruri(HashTable ht) {
	int nr = 0;
		for(int i=0;i<ht.dim;i++)
			if (ht.vector[i] != NULL) {
				Nod* p = ht.vector[i];
				while (p != NULL) {
					nr++;
					p = p->next;
				}
			}
	return nr;
}

Zbor getZborDupaId(HashTable ht, int id) {
	Zbor z;
	z.id = -1;
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vector[i];

		while (p != NULL) {
			if (p->info.id == id) {
				z = p->info;

				z.plecare = malloc(strlen(p->info.plecare) + 1);
				strcpy(z.plecare, p->info.plecare);
				z.destinatie = malloc(strlen(p->info.destinatie) + 1);
				strcpy(z.destinatie, p->info.destinatie);
				z.companie = malloc(strlen(p->info.companie) + 1);
				strcpy(z.companie, p->info.companie);

				return z;
			}
			p = p->next;
		}
	}
	return z;
}

int main() {
	HashTable ht = citireZboruriDinFisier("zboruri.txt");
	afisareTabelaZboruri(ht);

	int nrZboruri = nrTotalDeZboruri(ht);
	printf("\n\nSunt %d zboruri!", nrZboruri);

	Zbor z1 = getZborDupaId(ht, 7);
	printf("\nZborul cu id-ul 7 este: ");
	afisareZbor(z1);

	dezalocareTabelaZboruri(&ht);
	return 0;
}