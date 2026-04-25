#include<string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct Cafea Cafea;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
};

struct Nod {
	Cafea info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* ultim;
	Nod* prim;
};

Cafea citireCafeaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);//in buffer ramane prima linie din fisier
	Cafea c;
	char* aux;//folosit pt citirea stringurilor 

	c.id = atoi(strtok(buffer, sep));

	aux = strtok(NULL, sep);
	c.denumire =malloc(strlen(aux)+1);
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
	printf("\n\nid: %d", c.id);
	printf("\ndenumire: %s", c.denumire);
	printf("\npret: %.2f", c.pret);
	printf("\ngramaj: %d", c.gramaj);
	printf("\norigine: %s", c.origine);
	printf("\nmarime: %c", c.marime);
}

void afisareListaCafele(ListaDubla ld) {
	while (ld.prim != NULL) {
		afisareCafea(ld.prim->info);
		ld.prim = ld.prim->next;
	}
}

void afisareListaCafeleSfarsit(ListaDubla ld) {
	while (ld.ultim) {
		afisareCafea(ld.ultim->info);
		ld.ultim = ld.ultim->prev;
	}
}

void adaugaCafeaInListaLaFinal(ListaDubla* ld, Cafea cafea) {
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = cafea;
	nou->prev = ld->ultim;
	if (ld->ultim == NULL)
		ld->prim = nou;
	else
		ld->ultim->next = nou;
	ld->ultim = nou;
}

void adaugaCafeaInListaLaInceput(ListaDubla* ld, Cafea cafea) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = cafea;
	nou->prev = NULL;
	nou->next = ld->prim;
	if (ld ->prim== NULL)
		ld->ultim = nou;
	else
		ld->prim->prev = nou;
	ld->prim= nou;
}

ListaDubla citireCafeleDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;
		while (!feof(f)) {
			adaugaCafeaInListaLaFinal(&ld, citireCafeaDinFisier(f));
		}
		fclose(f);
		return ld;
	}
}

void dezalocare(ListaDubla* ld) {
	while (ld->prim && ld->prim->next) {
		free(ld->prim->info.denumire);
		free(ld->prim->info.origine);
		ld->prim = ld->prim->next;
		free(ld->prim->prev);
	}

	if (ld->prim) {
		free(ld->prim->info.denumire);
		free(ld->prim->info.origine);
		free(ld->prim);
	}

	ld->prim = ld->ultim = NULL;
}

void afisareCafeaPretMinim(ListaDubla ld) {
	if (ld.prim == NULL) return;
	Cafea cafeaIeftina = ld.prim->info;
	ld.prim = ld.prim->next;
	while (ld.prim != NULL) {
		if (ld.prim->info.pret < cafeaIeftina.pret) {
			cafeaIeftina = ld.prim->info;
		}
		ld.prim = ld.prim->next;
	}
	printf("\n\n\nCea mai ieftina cafea este: ");
	afisareCafea(cafeaIeftina);
}

//daca sunt 2 cu acelasi nr maxim, o va afisa pe prima gasita
char* getOrigineNrMaximCafele(ListaDubla ld) {
	if (ld.prim == NULL || ld.ultim == NULL)return NULL;
	
	Nod* p = ld.prim;
	int nrMax = 0;
	char* origineMax = NULL;
	while (p) {
		int nr = 0;
		Nod* q = ld.prim;
		while (q) {
			if (strcmp(p->info.origine, q->info.origine) == 0)
				nr++;
			q = q->next;
		}
		if (nr > nrMax) {
			nrMax = nr;
			free(origineMax);
			origineMax = malloc(strlen(p->info.origine) + 1);
			strcpy(origineMax, p->info.origine);
		}
		p = p->next;
	}
	return origineMax;
}

void stergeCafeaDupaId(ListaDubla* ld, int id) {
	if (ld->prim && ld->prim->info.id==id) {//daca nodul e primul
			free(ld->prim->info.denumire);
			free(ld->prim->info.origine);
			ld->prim = ld->prim->next;
			if (ld->prim) {
				free(ld->prim->prev);
				ld->prim->prev = NULL;
			}
			else
				ld->ultim = NULL;
		return;
	}

	Nod* aux = ld->prim;
	while (aux && aux->info.id != id)
		aux = aux->next;//daca id ul nu a fost gasit, se iese din bucla cand aux=null
	if (aux) {
		if (aux->prev) {//daca nodul mai are un nod inainte, atunci se face legatura dintre predecesor si succesor
			//adica predecesorul lui aux va retine adresa succesorului lui aux
			aux->prev->next = aux->next;
		}
		if (aux->next) {//daca nodul mai are vreun nod dupa el, atunci nodul urmator va retine
			//adresa predecesorului lui aux
			aux->next->prev = aux->prev;
		}
		else//daca nu are next ins ca e ultimul
		{
			aux->prev->next = NULL;
			ld->ultim = aux->prev;//ultimul devine cel dinaintea lui aux
		}
		free(aux->info.denumire);
		free(aux->info.origine);
		free(aux);
	}

}


int main() {
	ListaDubla ld = citireCafeleDinFisier("cafele.txt");
	afisareListaCafele(ld);
	
	afisareCafeaPretMinim(ld);

	Cafea c1;
	c1.id = 11;
	c1.denumire = malloc(strlen("Ice Latte") + 1);
	strcpy(c1.denumire, "Ice Latte");
	c1.gramaj = 100;
	c1.marime = 'S';
	c1.pret = 4.5;
	c1.origine = malloc(strlen("Romania") + 1);
	strcpy(c1.origine, "Romania");
	adaugaCafeaInListaLaInceput(&ld, c1);

	afisareCafeaPretMinim(ld);

	char* o = getOrigineNrMaximCafele(ld);
	printf("\nCea mai des intalnita origine la cafele este: %s",o);
	free(o);

	stergeCafeaDupaId(&ld, 11);
	afisareListaCafeleSfarsit(ld);
	dezalocare(&ld);
	return 0;
}