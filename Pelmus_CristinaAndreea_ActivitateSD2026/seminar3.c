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

void afisareMasina(Masina masina) {
	printf("\nid: %d", masina.id);
	printf("\nnr usi: %d", masina.nrUsi);
	printf("\npret: %.2f", masina.pret);
	printf("\nmodel: %s", masina.model);
	printf("\nnume sofer: %s", masina.numeSofer);
	printf("\nserie: %c", masina.serie);
	printf("\n---------------------------------");
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
		afisareMasina(masini[i]);
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	(*nrMasini)++;
	Masina* aux = (Masina*)malloc(sizeof(Masina) * (*nrMasini));
	for (int i = 0; i < (*nrMasini)-1; i++)
		aux[i] = (*masini)[i];//shallow copy
	aux[(*nrMasini)-1] = masinaNoua;
	free(*masini);
	*masini = aux;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[50];//un buffer
	strcpy(linie, fgets(linie, 50, file));
	char delimitator[3] = ",\n";

	Masina m;
	m.id= atoi(strtok(linie, delimitator));//strtok returneaza un char*, pt asta folosind atoi(ascii to int)
	m.nrUsi= atoi(strtok(NULL, delimitator));
	m.pret = atof(strtok(NULL, delimitator));

	char* buffer = strtok(NULL, delimitator);//sirul de caractere returnat de model e pe stiva, nu programatorul se ocupa de asta
	//dezalocam cu free doar ce am alocat noi cu malloc
	
	m.model = malloc(strlen(buffer) * sizeof(char));
	strcpy(m.model, buffer);
	buffer = strtok(NULL, delimitator);
	m.numeSofer = malloc(strlen(buffer) * sizeof(char));
	strcpy(m.numeSofer, buffer);

	buffer= strtok(NULL, delimitator);
	m.serie = buffer[0];

	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	*nrMasiniCitite = 0;
	Masina* masini = NULL;
	while (!feof(file)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));	
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** masini, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++) {
		free((*masini)[i].model);
		free((*masini)[i].numeSofer);
	}
	free(*masini);
}

int main() {
	int nrMasini=0;
	Masina* m1 = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(m1,nrMasini);
	return 0;
}