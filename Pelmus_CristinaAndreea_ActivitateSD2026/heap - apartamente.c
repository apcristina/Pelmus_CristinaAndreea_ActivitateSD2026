#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Apartament Apartament;
typedef struct Heap Heap;

struct Apartament {
    int id;
    int camere;
    float suprafata;
    float pret;
    char* oras;
};

struct Heap {
    Apartament* vector;
    int lungime;
    int nrApartamente;
};

Apartament citesteApartamentDinFisier(FILE* f) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Apartament a;
    fgets(buffer, 100, f);
    a.id = atoi(strtok(buffer, sep));
    a.camere = atoi(strtok(NULL, sep));
    a.suprafata = atof(strtok(NULL, sep));
    a.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    a.oras = malloc(strlen(aux) + 1);
    strcpy(a.oras, aux);
    return a;
}

void afisareApartament(Apartament a) {
    printf("\nid: %d", a.id);
    printf("\nnumar camere: %d", a.camere);
    printf("\nsuprafata: %.2f", a.suprafata);
    printf("\npret: %.2f", a.pret);
    printf("\noras: %s", a.oras);
}

Heap initializareHeap(int lungime) {
    Heap heap;
    heap.nrApartamente = 0;
    heap.lungime = lungime;
    heap.vector = malloc(heap.lungime * sizeof(Apartament));
    return heap;
}

void filtreazaHeap(Heap heap, int pozNod) {
    if (pozNod >= 0 && pozNod < heap.lungime) {

        int pozSt = 2 * pozNod + 1;
        int pozDr = 2 * pozNod + 2;
        int pozMax = pozNod;

        if (pozSt<heap.lungime && heap.vector[pozSt].suprafata>heap.vector[pozMax].suprafata)
            pozMax = pozSt;
        if (pozDr<heap.lungime && heap.vector[pozDr].suprafata>heap.vector[pozMax].suprafata)
            pozMax = pozDr;

        if (pozMax != pozNod) {
            Apartament aux = heap.vector[pozMax];
            heap.vector[pozMax] = heap.vector[pozNod];
            heap.vector[pozNod] = aux;

            if (pozMax <= (heap.nrApartamente - 2) / 2)
                filtreazaHeap(heap, pozMax);
        }
    }
}

Heap citireHeapDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Heap heap = initializareHeap(10);
    while (!feof(f)) {
        heap.vector[heap.nrApartamente++] = citesteApartamentDinFisier(f);
    }
    fclose(f);

    for (int i = (heap.lungime - 2) / 2; i >= 0; i--)
        filtreazaHeap(heap, i);

    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrApartamente; i++)
        afisareApartament(heap.vector[i]);
}

void afisareHeapAscuns(Heap heap) {
    for (int i = heap.nrApartamente; i < heap.lungime; i++)
        afisareApartament(heap.vector[i]);
}

Apartament extrageHeap(Heap* heap) {
    if (heap->nrApartamente > 0) {
        Apartament aux = heap->vector[0];
        heap->vector[0] = heap->vector[heap->nrApartamente - 1];
        heap->vector[heap->nrApartamente - 1] = aux;


        heap->nrApartamente--;

        for (int i = (heap->nrApartamente - 2) / 2; i >= 0; i--)
            filtreazaHeap(*heap, i);
        return heap->vector[heap->nrApartamente];
    }
    else
    {
        Apartament a;
        a.id = -1;
        a.oras = NULL;
        return a;
    }
}
int main() {
    Heap heap = citireHeapDinFisier("apartamente.txt");
    afisareHeap(heap);
    extrageHeap(&heap);
    extrageHeap(&heap);
    extrageHeap(&heap);
    printf("\n\nHeap dupa extragere: \n");
    afisareHeap(heap);
    printf("\n\nHeap ascuns: \n");
    afisareHeapAscuns(heap);
	return 0;
}