//#include<string.h>
//#include<stdio.h>
//#include<stdlib.h>
//
//typedef struct Cafea Cafea;
//typedef struct Nod Nod;
//
//struct Cafea {
//	int id;
//	char* denumire;
//	float pret;
//	int gramaj;
//	char* origine;
//	char marime;
//	};
//
//struct Nod {
//	Cafea cafea;
//	Nod* stanga;
//	Nod* dreapta;
//};
//
//Cafea citireCafeaDinFisier(FILE *f) {
//	char buffer[100];
//	fgets(buffer, 100, f);
//	char sep[3] = "\n,";
//	char* aux;
//	Cafea c;
//	c.id = atoi(strtok(buffer, sep));
//	aux = strtok(NULL, sep);
//	c.denumire = malloc(strlen(aux) + 1);
//	strcpy(c.denumire, aux);
//	c.pret = atof(strtok(NULL, sep));
//	c.gramaj = atoi(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	c.origine = malloc(strlen(aux) + 1);
//	strcpy(c.origine, aux);
//	c.marime = *strtok(NULL, sep);
//	return c;
//}
//
//void afisareCafea(Cafea c) {
//	if (c.id <= 0) {
//		printf("\nnu extista cafeaua!");
//		return;
//	}
//	printf("\n\nid: %d", c.id);
//	printf("\ndenumire: %s", c.denumire);
//	printf("\npret: %.2f", c.pret);
//	printf("\ngramaj: %d", c.gramaj);
//	printf("\norigine: %s", c.origine);
//	printf("\nmarime: %c", c.marime);
//}
//
//void adaugaMasinaInArbore(Nod** arbore, Cafea c) {
//	if (*arbore == NULL) {
//		Nod* nou = malloc(sizeof(Nod));
//		nou->dreapta = NULL;
//		nou->stanga= NULL;
//		nou->cafea = c;
//		*arbore = nou;
//		return;
//	}
//
//	//&(*arbore)->stanga - prima data se face dereferentierea, apoi accesam stanga, apoi se extrage adresa
//	if (c.id < (*arbore)->cafea.id)
//		adaugaMasinaInArbore( &(*arbore)->stanga,c);
//	else
//		adaugaMasinaInArbore(&(*arbore)->dreapta,c);
//}
//
//Nod* citireArboreDinFisier(const char* numeFisier) {
//	FILE* f = fopen(numeFisier, "r");
//	Nod* arbore=NULL;
//	while (!feof(f)) {
//		adaugaMasinaInArbore(&arbore, citireCafeaDinFisier(f));
//	}
//	fclose(f);
//	return arbore;
//}
//
////afisare preordine
//void afisareArbore(Nod* arbore) {
//	if (arbore) {
//		afisareCafea(arbore->cafea);
//		afisareArbore(arbore->stanga);
//		afisareArbore(arbore->dreapta);
//	}
//}
//
//void dezalocareArbore(Nod** arbore) {
//	if (*arbore) {
//		dezalocareArbore(&(*arbore)->stanga);
//		dezalocareArbore(&(*arbore)->dreapta);
//
//		free((*arbore)->cafea.denumire);
//		free((*arbore)->cafea.origine);
//		free(*arbore);
//
//		*arbore = NULL;
//	}
//}
//
//Cafea getCafeaDupaId(Nod* arbore, int id) {
//	Cafea c;
//	if (arbore != NULL) {
//		if (arbore->cafea.id == id) {
//			c = arbore->cafea;
//			c.denumire = malloc(strlen(arbore->cafea.denumire) + 1);
//			strcpy(c.denumire, arbore->cafea.denumire);
//			c.origine= malloc(strlen(arbore->cafea.origine) + 1);
//			strcpy(c.origine, arbore->cafea.origine);
//		}
//		else {
//			if (arbore->cafea.id < id)
//				getCafeaDupaId(arbore->dreapta, id);
//			else
//				getCafeaDupaId(arbore->stanga, id);
//		}
//	}else
//	c.id = -1;
//	return c;
//}
//
//int nrNoduri(Nod* arbore) {
//	int nr = 0;
//	if (arbore) {
//		nr = nr + 1 + nrNoduri(arbore->stanga) + nrNoduri(arbore->dreapta);
//	}
//	return nr;
//}
//
//int inaltimeArbore(Nod* arbore) {
//	int n = 0;
//	if (arbore == NULL) return 0;
//	return 1 + max(inaltimeArbore(arbore->stanga), inaltimeArbore(arbore->dreapta));
//}
//
//float calculeazaPretTotal(Nod* arbore) {
//	float total = 0;
//	if (arbore) {
//		total = arbore->cafea.pret + calculeazaPretTotal(arbore->stanga) + calculeazaPretTotal(arbore->dreapta);
//	}
//	return total;
//}
//
//int main() {
//	Nod* arbore = citireArboreDinFisier("cafele_arbore.txt");
//	afisareArbore(arbore);
//	Cafea c = getCafeaDupaId(arbore,7);
//	printf("\n\nCafeaua cu id-ul 7 este: ");
//	afisareCafea(c);
//	printf("\n\nSunt %d noduri in arbore!", nrNoduri(arbore));
//	printf("\nArborele are inaltimea egala cu %d", inaltimeArbore(arbore));
//	printf("\nPretul total al cafelelor din arbore este: %.2f\n",calculeazaPretTotal(arbore));
//	dezalocareArbore(&arbore);
//	return 0;
//}