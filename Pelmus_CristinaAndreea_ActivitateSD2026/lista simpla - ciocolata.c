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

struct Nod{
	Nod* next;
	Ciocolata info;
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

//adaugare la sfarsit
void adaugaCiocolataInLista(Nod** ls, Ciocolata c) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	if (*ls != NULL) {
		Nod* aux = *ls;
		while (aux->next)
			aux = aux->next;
		aux->next = nou;
	}
	else
		*ls = nou;
}

//adaugare la inceput
void adaugaCiocolataInListaLaInceput(Nod** ls, Ciocolata c) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = c;
	nou->next = *ls;
	*ls = nou;
}

void afisareLista(Nod* ls) {
	while (ls != NULL) {
		afisareCiocolata(ls->info);
		ls = ls->next;
	}
}

Nod* citireListaDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* ls = NULL;
	while (!feof(f))
		adaugaCiocolataInLista(&ls, citireCiocolataDinFisier(f));
	fclose(f);
	return ls;
}

void dezalocareLista(Nod** ls) {
	Nod* next = NULL;
	Nod* aux = *ls;
	while (aux) {
		free(aux->info.denumire);
		free(aux->info.producator);
		*ls = aux->next;
		free(aux);
		aux = *ls;
	}
	*ls = NULL;
}

//void stergeCiocolataDinTipul(Nod** ls, char tip) {
//	while (*ls && (*ls)->info.tip == tip) {
//		Nod* aux = *ls;
//		free(aux->info.denumire);
//		free(aux->info.producator);
//		*ls = aux->next;
//		free(aux);
//	}
//
//	Nod* curent = *ls;
//	while (curent&&curent->next) {
//		if (curent->info.tip == tip) {
//			Nod* aux = curent->next;
//			curent->next = aux->next;
//			free(aux->info.denumire);
//			free(aux->info.producator);
//			free(aux);
//		}
//		else
//			curent = curent->next;
//	}
//}

void stergeCiocolataDinTipul2(Nod** ls, char tip) {
	while (*ls != NULL) {
		if ((*ls)->info.tip == tip) {
			Nod* aux = *ls;
			*ls = (*ls)->next;
			free(aux->info.denumire);
			free(aux->info.producator);
			free(aux);
		}
		else
			ls = &(*ls)->next;
	}
}

void stergeCiocolataDupaId(Nod** ls, int id) {
	while (*ls != NULL) {
		if ((*ls)->info.id == id) {
			Nod* aux = *ls;
			*ls = (*ls)->next;
			free(aux->info.denumire);
			free(aux->info.producator);
			free(aux);
		}
		else
			ls = &(*ls)->next;
	}
} 
int main() {
	Nod* ls = citireListaDinFisier("ciocolata.txt");
	afisareLista(ls);
	//stergeCiocolataDinTipul(&ls, 'L');
	stergeCiocolataDinTipul2(&ls, 'L');
	stergeCiocolataDupaId(&ls, 10);
	printf("\nDupa stergere: ");
	afisareLista(ls);
	dezalocareLista(&ls);
	return 0;
}