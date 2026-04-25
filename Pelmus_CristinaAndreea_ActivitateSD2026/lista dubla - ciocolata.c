#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Ciocolata {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* producator;
	char tip;
};

typedef struct Ciocolata Ciocolata;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod{
	Nod* next;
	Nod* prev;
	Ciocolata info;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

Ciocolata citireCiocolataDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);

	char* aux;
	Ciocolata c;

	aux = strtok(buffer, sep);
	c.id = atoi(aux);

	aux = strtok(NULL, sep);
	c.denumire = malloc(strlen(aux) + 1);
	strcpy(c.denumire, aux);

	c.pret = atof(strtok(NULL, sep));
	c.gramaj = atoi(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.producator = malloc(strlen(aux) + 1);
	strcpy(c.producator, aux);

	c.tip = *strtok(NULL, sep);

	return c;
}

void afisareCiocolata(Ciocolata c) {
	printf("\nId: %d", c.id);
	printf("\nDenumire: %s", c.denumire);
	printf("\nPret: %.2f", c.pret);
	printf("\nGramaj: %d", c.gramaj);
	printf("\nProducator: %s", c.producator);
	printf("\nTip: %c\n", c.tip);
}

void adaugaCiocolataInListaFinal(Ciocolata c, ListaDubla* ld) {
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->prev = ld->ultim;
	nou->info = c;
	if (ld->ultim == NULL) {
		ld->prim = nou;
	}
	else
		ld->ultim->next = nou;
	ld->ultim = nou;
}

void adaugaCiocolataInListaInceput(Ciocolata c, ListaDubla* ld) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = c;
	nou->prev = NULL;
	nou->next = ld->prim;
	if (ld->prim == NULL)
		ld->ultim = nou;
	else
		ld->prim->prev = nou;
	ld->prim = nou;
}

ListaDubla citireListaDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla ld;
	ld.ultim = NULL;
	ld.prim = NULL;
	while (!feof(f))
		adaugaCiocolataInListaFinal(citireCiocolataDinFisier(f), &ld);
	fclose(f);
	return ld;
}

void afisareLista(ListaDubla ld) {
	while (ld.prim != NULL) {
		afisareCiocolata(ld.prim->info);
		ld.prim = ld.prim->next;
	}
}

void dezalocareLista(ListaDubla* ld) {
	while (ld->prim&&ld->prim->next) {
		ld->prim = ld->prim->next;
		free(ld->prim->prev->info.denumire);
		free(ld->prim->prev->info.producator);
		free(ld->prim->prev);
	}
	if (ld->prim) {
		free(ld->prim->info.denumire);
		free(ld->prim->info.producator);
		free(ld->prim);
	}
	ld->prim = ld->ultim = NULL;
}

void stergeCiocolataDupaId(ListaDubla* ld, int id) {
	if(ld->prim&&ld->prim->info.id==id)
	while (ld->prim) {
		if (ld->prim->info.id == id) {
			free(ld->prim->info.denumire);
			free(ld->prim->info.producator);
			ld->prim = ld->prim->next;
			if (ld->prim)
				free(ld->prim->prev);
			else
				ld->ultim = NULL;
		}
		return;
	}

	Nod* p = ld->prim;
	while (p&&p->info.id!=id) {
		p = p->next;
	}
	if (p) {
		if (p->prev) {
			p->prev->next = p->next;
		}
		if (p->next) {
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			ld->ultim = p->prev;
		}
		free(p->info.denumire);
		free(p->info.producator);
		free(p);
	}
}
int main() {
	ListaDubla ld = citireListaDinFisier("ciocolata.txt");
	afisareLista(ld);
	dezalocareLista(&ld);
	return 0;
}