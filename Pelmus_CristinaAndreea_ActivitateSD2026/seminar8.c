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

//heap -> structura de date liniara 
//min heap / max heap - scopul de a aduce val minima/maxima cu o complexitate redusa
//arbore = graf conex aciclic
//arbore binar = fiecare nod are maxim 2 fii

//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;//cat are spatiu alocat
	int nrMasini;//cate masini are
	Masina* masini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.masini = malloc(sizeof(Masina) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	if (pozitieNod >= 0 && pozitieNod < heap.lungime) {

		int pozitieStanga = pozitieNod * 2 + 1;//2i+1
		int pozitieDreapta = pozitieNod * 2 + 2;//2i+2
		int pmax = pozitieNod;

		if (pozitieStanga < heap.lungime&&heap.masini[pmax].pret<heap.masini[pozitieStanga].pret) {
			pmax = pozitieStanga;
		}
		if (pozitieDreapta < heap.lungime && heap.masini[pmax].pret < heap.masini[pozitieDreapta].pret) {
			pmax = pozitieDreapta;
		}

		if (pmax != pozitieNod) {
			Masina aux = heap.masini[pmax];
			heap.masini[pmax] = heap.masini[pozitieNod];
			heap.masini[pozitieNod] = aux;

			if (pmax < (heap.nrMasini - 2) / 2) {
				filtreazaHeap(heap, pmax);//autoapelare
			}
		}
	}

}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(f)) {
		heap.masini[heap.nrMasini++] = citireMasinaDinFisier(f);
	}

	fclose(f);

	for (int i = (heap.nrMasini - 2) / 2; i >= 0; i--)
		filtreazaHeap(heap, i);
	
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasini; i++)
		afisareMasina(heap.masini[i]);
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini; i < heap.lungime; i++)
		afisareMasina(heap.masini[i]);
}

Masina extrageMasina(Heap* heap) {//e trimis prin pointer, deoarece modificam nr de masini
	if (heap->nrMasini > 0) {
		Masina aux = heap->masini[0];
		heap->masini[0] = heap->masini[heap->nrMasini - 1];
		heap->masini[heap->nrMasini - 1] = aux;

		heap->nrMasini--;
		for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--)
			filtreazaHeap(*heap, i);

		return heap->masini[heap->nrMasini];
	}
	else {
		Masina m;
		m.id = -1;
		return m;
	}
}


void dezalocareHeap(Heap* heap) {
	//ca la stergerea unui vect
}

int main() {
	Heap h = citireHeapDeMasiniDinFisier("masini.txt");
	afisareHeap(h);

	printf("\nExtragere: ");
	Masina m = extrageMasina(&h);
	afisareMasina(m);

	Masina m1 = extrageMasina(&h);
	afisareMasina(m1);

	Masina m2= extrageMasina(&h);
	afisareMasina(m2);

	printf("\nAfisare heap ascuns:\n");
	afiseazaHeapAscuns(h);

	return 0;
}