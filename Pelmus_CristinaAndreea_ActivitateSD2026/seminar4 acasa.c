#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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

struct Nod {
	Cafea info;
	Nod* next;
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

void adaugaCafeaInLista(Nod** cap, Cafea cafeaNoua) {
	Nod* noua = malloc(sizeof(Nod));
	noua->info = cafeaNoua;
	noua->next = NULL;
	Nod* aux = *cap;
	if ((*cap) != NULL) {
		while (aux->next != NULL)
			aux = aux->next;
		aux->next = noua;
	}
	else
		*cap = noua;
}

void afisareListaCafele(Nod* cap) {
	while (cap != NULL) {
		afisareCafea(cap->info);
		cap = cap->next;
	}
}

void adaugaCafeaSortat(Cafea c, Nod** cap) {
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = c;

	if (*cap == NULL || strcmp(c.denumire, (*cap)->info.denumire) < 0)//lista goala sau inserare la inceput
	{
		nou->next = *cap;
		*cap = nou;
		return;
	}

	Nod* p = *cap;
	while (p->next != NULL && strcmp(c.denumire, p->next->info.denumire) > 0)
		p = p->next;//parcurgem lista pana la final sau pana ajungem la 
					//o valoare mai mare decat cea pe care vrem sa o inseram

	nou->next = p->next;
	p->next = nou;

}


Nod* citireListaDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* cap = NULL;
	while (!feof(f))
		//adaugaCafeaInLista(&cap, citireCafeaDinFisier(f));
		adaugaCafeaSortat(citireCafeaDinFisier(f), &cap);
	fclose(f);
	return cap;
}

int nrCafeleOrigine(Nod* cap, const char* origineCautata) {
	int nr = 0;
	while (cap != NULL) {
		if(strcmp(cap->info.origine, origineCautata)==0)
			nr++;
		cap = cap->next;
	}
	return nr;
}

void afisareCafeleGramaj(Nod* cap, int gramajMinim) {
	while (cap != NULL) {
		if (cap->info.gramaj >= gramajMinim)
			afisareCafea(cap->info);
		cap = cap->next;
	}
}

Cafea pretMaxim(Nod* cap) {
	Cafea max = cap->info;
	cap = cap->next;
	while (cap != NULL) {
		if (max.pret < cap->info.pret)
			max = cap->info;//shallow copy
		cap = cap->next;
	}
	return max;
}

Cafea* pretMaximFaraCopieri(Nod* cap) {
	if (cap == NULL) return NULL;
	Cafea* max = &(cap->info);//cafea* retine adresa unei 
	//cafele, cu & extragem adresa cafelei de la nodul curent
	cap = cap->next;
	while(cap != NULL) {
		if (cap->info.pret > (*max).pret) {
			max = &(cap->info);
		}
		cap = cap->next;
	}
	return max;
}

void stergereCafeleOrigine(Nod** cap, const char* origineCautata) {
	while ((*cap) != NULL) {
		if (strcmp((*cap)->info.origine, origineCautata)==0) {
			Nod* aux = *cap;
			*cap = (*cap)->next;
			if (aux->info.origine)
				free(aux->info.origine);
			if (aux->info.denumire)
				free(aux->info.denumire);
		}
		else
			cap = &(*cap)->next;
	}
}


// avem lista A->B->C->NULL
// while(cap!=NULL) => parcurgem A,B,C
// while(cap->next!=NULL) => parcurgem A,B si ramanem pe C

void dezalocareListaCafele(Nod** cap) {
	Nod* aux;
	while ((*cap) != NULL) {
		aux = *cap;
		*cap = (*cap)->next;
		if (aux->info.denumire)
			free(aux->info.denumire);
		if (aux->info.origine)
			free(aux->info.origine);
		free(aux);
	}
	*cap = NULL;
}

int main() {
	Nod* lista = citireListaDinFisier("cafele.txt");
	afisareListaCafele(lista);

	int nr = nrCafeleOrigine(lista, "Brazilia");
	printf("\n\nnr cafele din Brazilia: %d", nr);

	printf("\n\ncafele mai mari de 250g: ");
	afisareCafeleGramaj(lista, 250);

	Cafea max = pretMaxim(lista);
	printf("\n\nMETODA 1: cea mai scumpa cafea este: ");
	afisareCafea(max);

	Cafea* pmax = pretMaximFaraCopieri(lista);
	printf("\n\nMETODA 2: cea mai scumpa cafea este: ");
	afisareCafea(*pmax);

	stergereCafeleOrigine(&lista, "Italia");
	printf("\n\nCafele dupa eliminarea celor provenite din Italia: ");
	afisareListaCafele(lista);

	dezalocareListaCafele(&lista);

	return 0;
}