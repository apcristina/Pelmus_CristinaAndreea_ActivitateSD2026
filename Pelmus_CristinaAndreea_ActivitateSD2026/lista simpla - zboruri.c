#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Zbor Zbor;
typedef struct Nod Nod;

struct Zbor {
	int id;
	char* plecare;
	char* destinatie;
	char* companie;
	float durata;
	float pret;
};

struct Nod {
	Zbor info;
	Nod* next;
};

Zbor citireZborFisier(FILE * f){
	char buffer[100];
	fgets(buffer, 100, f);
	char sep[3] = "\n;";
	char *aux;
	Zbor z;

	z.id = atoi(strtok(buffer, sep));

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
	if (z.id <= 0) {
		printf("\nNu exista zbor!!!");
		return;
	}
	printf("\n\nid: %d", z.id);
	printf("\nplecare: %s", z.plecare);
	printf("\ndestinatie: %s", z.destinatie);
	printf("\ncompanie: %s", z.companie);
	printf("\ndurata: %.2f ore", z.durata);
	printf("\npret: %.2f", z.pret);
}

void adaugaZborFinal(Nod** ls, Zbor z) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = z;//shallow copy
	nou->next = NULL;
	if (*ls != NULL) {
		Nod* p = *ls;
		while (p->next != NULL)
			p = p->next;
		p->next = nou;
	}
	else
		*ls = nou;
}

void adaugaZborInceput(Nod** ls, Zbor z) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = z;
	nou->next = *ls;
	*ls = nou;
}

void afisareZboruri(Nod* ls) {
	while (ls != NULL) {
		afisareZbor(ls->info);
		ls = ls->next;
	}
}

Nod* citireZboruriFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* ls=NULL;
	while (!feof(f)) {
		adaugaZborFinal(&ls, citireZborFisier(f));
	}
	fclose(f);
	return ls;
}

void dezalocareZboruri(Nod** ls) {
	Nod* p = *ls;
	while (p) {
		Nod* aux = p->next;

		free(p->info.plecare);
		free(p->info.destinatie);
		free(p->info.companie);
		free(p);

		p = aux;
	}
	*ls = NULL;
}

Zbor getZborScurt(Nod* ls) {
	if (ls != NULL) {
		Zbor zborMin = ls->info;
		ls = ls->next;
		while (ls != NULL) {
			if (ls->info.durata < zborMin.durata)
				zborMin = ls->info;
			ls = ls->next;
		}
		return zborMin;
	}
	else {
		Zbor z;
		z.id = -1;
		return z;
	}
}

void stergeZboruriDintrOCompanie(Nod** ls, const char* companie) {
	Nod* p = *ls;
	Nod* prev = NULL;
	while (p) {
		Nod* aux = p->next;
		if (strcmp(p->info.companie, companie) == 0) {

			if (prev == NULL) //daca e primul
				*ls = p->next; //capul listei merge pe pozitia urmatoare
			else
				prev->next = p->next;//daa nu e primul, predecesorul trece la succesor

			free(p->info.companie);
			free(p->info.destinatie);
			free(p->info.plecare);
			free(p);
		}
		else
			prev = p;//daca compania nu e cea data, nodul precedent devine cel curent

		p = aux;//nodul curent trece la urmatorul care e salvat in aux
	}
}

Nod* copiazaZboruriIeftine(Nod* ls, float prag) {
	Nod* lsIeftine = NULL;
	while (ls != NULL) {
		if (ls->info.pret <= prag) {
			//adaugaZborFinal(&lsIeftine, ls->info); -> shallow copy, dar nu tocmai ce trebuie
			Zbor z = ls->info;
			z.plecare = malloc(strlen(ls->info.plecare) + 1);
			strcpy(z.plecare, ls->info.plecare);
			z.destinatie = malloc(strlen(ls->info.destinatie) + 1);
			strcpy(z.destinatie, ls->info.destinatie);
			z.companie = malloc(strlen(ls->info.companie) + 1);
			strcpy(z.companie, ls->info.companie);

			adaugaZborFinal(&lsIeftine, z);

		}
		ls = ls->next;
	}
	return lsIeftine;
}

void sorteazaListaPreturi(Nod* ls) {//bubble sort
	if (ls == NULL) return;

	int ok;
	Nod* p;
	//pt optimizare am putea sa luam un aux: 
	//Nod* aux = NULL; - ultimul pe care il verificam

	do {
		ok = 0;//preupunem ca lista e sortata
		p = ls;//plecam mereu de la inceputul listei

		while (p->next != NULL) {
			if (p->info.pret > p->next->info.pret) {
				Zbor aux = p->info;
				p->info = p->next->info;
				p->next->info = aux;
				ok = 1;
			}
			p = p->next;
		}
	} while (ok);//daca am facut cel putin o schimbare reverificam
}

int main() {
	Nod* ls = citireZboruriFisier("zboruri.txt");
	afisareZboruri(ls);

	printf("\nCel mai scurt zbor este: ");
	afisareZbor(getZborScurt(ls));

	stergeZboruriDintrOCompanie(&ls, "Tarom");
	printf("\n========================\n");
	printf("\ndupa stergerea zborurilor companiei Tarom: ");
	afisareZboruri(ls);

	Nod* lsIeftine = copiazaZboruriIeftine(ls, 100);
	printf("\n========================\n");
	printf("\nZboruri sub 100 fara Tarom: ");
	afisareZboruri(lsIeftine);

	printf("\nZboruri sub 100 fara Tarom - dupa sortare:");
	sorteazaListaPreturi(lsIeftine);
	afisareZboruri(lsIeftine);

	dezalocareZboruri(&ls);
	dezalocareZboruri(&lsIeftine);

	//verificare dezalocare: 
	printf("\nCel mai scurt zbor este: ");
	afisareZbor(getZborScurt(ls));
	return 0;
}