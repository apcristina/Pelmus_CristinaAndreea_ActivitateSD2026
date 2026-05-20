#include<string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct Cafea Cafea;
typedef struct Heap Heap;

struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
};

struct Heap{
	Cafea* vector;
	int nrCafele;
	int lungime;
};

Cafea citireCafeaDinFisier(FILE* f) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	Cafea c;
	char* aux;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrCafele = 0;
	heap.vector = malloc(sizeof(Cafea) * lungime);
	return heap;
}

void filtrareHeap(Heap* heap, int pozNod) {
	if (heap) {
		int pozSt = 2 * pozNod + 1;
		int pozDr = 2 * pozNod + 2;
		int pozMax = pozNod;
		if (pozSt<heap->nrCafele && heap->vector[pozSt].id>heap->vector[pozMax].id)
			pozMax = pozSt;
		if (pozDr<heap->nrCafele && heap->vector[pozDr].id>heap->vector[pozMax].id)
			pozMax = pozDr;
		if (pozMax != pozNod) {
			Cafea aux = heap->vector[pozMax];
			heap->vector[pozMax] = heap->vector[pozNod];
			heap->vector[pozNod] = aux;

			if (pozMax <= (heap->nrCafele - 2) / 2)
				filtrareHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDinFisier(const char* numeFisier, int lungime) {
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(lungime);
	while (!feof(f)) {
		heap.vector[heap.nrCafele++]=citireCafeaDinFisier(f);
	}
	fclose(f);
	for (int i = (heap.nrCafele - 2) / 2; i >= 0; i--)
		filtrareHeap(&heap, i);
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrCafele; i++)
		afisareCafea(heap.vector[i]);
}

void afisareHeapAscuns(Heap heap) {
	for (int i =heap.nrCafele;i<heap.lungime ; i++)
		afisareCafea(heap.vector[i]);
}

Cafea getCafeaDupaId(Heap heap, int id) {
	Cafea c;
	c.id = -1;
	for(int i=0;i<heap.nrCafele;i++)
		if (heap.vector[i].id == id) {
			c = heap.vector[i];
			c.denumire = malloc(strlen(heap.vector[i].denumire) + 1);
			strcpy(c.denumire, heap.vector[i].denumire);

			c.origine = malloc(strlen(heap.vector[i].origine) + 1);
			strcpy(c.origine, heap.vector[i].origine);
		}
	return c;
}

Cafea extragereHeap(Heap* heap) {
	Cafea c;
	c.id = -1;
	if (heap) {
		Cafea aux = heap->vector[heap->nrCafele - 1];
		heap->vector[heap->nrCafele-1] = heap->vector[0];
		heap->vector[0] = aux;

		heap->nrCafele--;

		for (int i = (heap->nrCafele - 2) / 2; i >= 0; i--)
			filtrareHeap(heap, i);

		c = heap->vector[heap->nrCafele];
	}
	return c;
}
void dezalocareHeap(Heap* heap) {
	if (heap) {
		for (int i = 0; i < heap->lungime; i++) {
			if (heap->vector[i].denumire)
				free(heap->vector[i].denumire);
			if (heap->vector[i].origine)
				free(heap->vector[i].origine);
		}
		free(heap->vector);
		heap->lungime = 0;
		heap->nrCafele = 0;
		heap = NULL;
	}
}

Cafea getMax(Heap heap) {
	Cafea c;
	c.id = -1;
	if (heap.nrCafele > 0) return heap.vector[0];
	else return c;
}
int main() {
	Heap heap = citireHeapDinFisier("cafele.txt",10);
	afisareHeap(heap);
	printf("\n\nCafeaua cu id ul 5 este: ");
	Cafea c = getCafeaDupaId(heap, 5);
	afisareCafea(c);

	extragereHeap(&heap);
	extragereHeap(&heap);
	printf("\n\nHeap ascuns: ");
	afisareHeapAscuns(heap);

	printf("\n\nCafeaua cu id ul maxim este: ");
	Cafea c1 = getMax(heap);
	afisareCafea(c1);

	free(c.denumire);
	free(c.origine);
	dezalocareHeap(&heap);
}