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
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	Masina info;
	Nod* prev;
	Nod* next;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
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

void afisareListaMasini(ListaDubla  lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniSfarsit(ListaDubla  lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla*lista,  Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->ultim;
	if (lista->ultim != NULL) {
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}
	lista->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla*lista,  Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = lista->prim;
	if (lista->prim != NULL) {
		lista->prim->prev = nou;
	}
	else {
		lista->ultim = nou;
	}
	lista->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}
		fclose(f);
		return ld;
	}
}

void dezalocareLDMasini(ListaDubla *ld) {
	while (ld->prim&&ld->prim->next) {
		free(ld->prim->info.model);
		free(ld->prim->info.numeSofer);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.model);
		free(ld->prim->info.numeSofer);
		free(ld->prim);
	}
	ld->prim = ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla ld) {
	if (ld.prim == NULL && ld.ultim == NULL) return 0;
	float suma = 0;
	int nrMasini = 0;
	while (ld.prim) {
		suma += ld.prim->info.pret;
		nrMasini++;
		ld.prim = ld.prim->next;
	}
	return suma/nrMasini;
}

//de incercat si cu pointer
char* getNumeSoferMasinaScumpa(ListaDubla ld) {
	if (ld.prim == NULL && ld.ultim == NULL) return NULL;
	float max = ld.prim->info.pret;
	ld.prim = ld.prim->next;
	char numeSoferMaxim[100];
	strcpy(numeSoferMaxim, ld.prim->info.numeSofer);
	while (ld.prim) {
		if (ld.prim->info.pret > max) {
			max = ld.prim->info.pret;
			strcpy(numeSoferMaxim, ld.prim->info.numeSofer);
		}
		ld.prim = ld.prim->next;
	}
	return numeSoferMaxim;
}

void stergeMasinaDupaID(ListaDubla* ld, int id) {
	if (ld->prim && ld->prim->info.id) {
		if (ld->prim->info.id == id) {
			free(ld->prim->info.model);
			free(ld->prim->info.numeSofer);
			ld->prim = ld->prim->next;
			if (ld->prim) {
				free(ld->prim->prev);
			}
			else {
				free(ld->ultim);
				ld->ultim = NULL;
			}
			return;
		}
		
	}
	Nod* p = ld->prim;
	while (p && p->info.id != NULL) {
		p = p->next;
	}
	if (p) {
		if (p->prev) {
			p->prev->next = p->next;
		}
		if(p->next) {
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			ld->ultim = p->prev;
		}
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

int main() {
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	ListaDubla ld = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(ld);
	float medie = calculeazaPretMediu(ld);
	printf("\npretul mediu al masinilor este: %.2f", medie);
	char nume[100];
	strcpy(nume,getNumeSoferMasinaScumpa(ld));
	printf("\nnumele soferului cu masina cea mai scumpa este: %s\n\n",nume);
	stergeMasinaDupaID(&ld, 1);
	printf("\nmasini supa stergere: \n");
	afisareListaMasini(ld);
	dezalocareLDMasini(&ld);
	return 0;
}