#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Cafea Cafea;
typedef struct Nod Nod;
typedef struct Queue Queue;

// la coada avem: inserare la un capat, extragere la celalalt capat
struct Cafea {
	int id;
	char* denumire;
	float pret;
	int gramaj;
	char* origine;
	char marime;
	};

//coada prin LDI
struct Nod {
	Cafea info;
	Nod* next;
	Nod* prev;
};

struct Queue {
	Nod* ultim;
	Nod* prim;
};

Cafea citireCafeaDinFisier(FILE *f) {
	char buffer[100];
	fgets(buffer, 100, f);
	char sep[3] = "\n,";
	char* aux;
	Cafea c;
	c.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	c.denumire = malloc(strlen(aux) + 1);
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
	if (c.id <= 0) {
		printf("\nnu extista cafeaua!");
		return;
	}
	printf("\n\nid: %d", c.id);
	printf("\ndenumire: %s", c.denumire);
	printf("\npret: %.2f", c.pret);
	printf("\ngramaj: %d", c.gramaj);
	printf("\norigine: %s", c.origine);
	printf("\nmarime: %c", c.marime);
}

//adaugare la inceput in ld
void enqueue(Queue* queue, Cafea c) {
	Nod* nou=malloc(sizeof(Nod));
	nou->info = c;
	nou->prev= NULL;
	nou->next = queue->prim;

	if (queue->prim != NULL) {
		queue->prim->prev = nou;
	}
	else
		queue->ultim = nou;

	queue->prim = nou;
}

//scoatere de la sfarsit in ld
Cafea dequeue(Queue* queue) {
	Cafea c;
	if (queue->prim == NULL)
		c.id = -1;

	c = queue->ultim->info;//shallow copy

	if (queue->prim == queue->ultim) {//daca exista doar un nod
		free(queue->prim);
		queue->prim = NULL;
		queue->ultim = NULL;
	}
	else {
		queue->ultim = queue->ultim->prev;
		free(queue->ultim->next);
		queue->ultim->next = NULL;
	}
	return c;
}

Queue citireCoadaDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Queue q;
	q.prim = NULL;
	q.ultim = NULL;
	while (!feof(f)) {
		enqueue(&q, citireCafeaDinFisier(f));
	}
	fclose(f);
	return q;
}

void dezalocareCoada(Queue* q) {
	
	while (q->prim != NULL) {
		Nod* aux = q->prim;
		q->prim = q->prim->next;

		free(aux->info.denumire);
		free(aux->info.origine);
		free(aux);
	}

	//folosind functia dequeue:
	//while(q->prim!=NULL){
	//Cafea c=dequeue(q);
	//free(c.denumire);
	//free(c.origine);
	//}
	q->ultim = NULL;
}

Cafea getCafeaDupaId(Queue* q, int id) {
	Cafea rez;
	rez.id = -1;
	Queue aux;
	aux.prim = NULL;
	aux.ultim = NULL;
	while (q->prim != NULL) {
		Cafea c = dequeue(q);
		if (c.id == id) {
			rez = c;
			rez.denumire = malloc(strlen(c.denumire) + 1);
			strcpy(rez.denumire, c.denumire);
			rez.origine = malloc(strlen(c.origine) + 1);
			strcpy(rez.origine, c.origine);
		}
		enqueue(&aux, dequeue(q));
	}
	q->prim = aux.prim;
	q->ultim = aux.ultim;

	return rez;
}

int main() {
	Queue q = citireCoadaDinFisier("cafele.txt");
	afisareCafea(dequeue(&q));
	afisareCafea(dequeue(&q));

	Cafea c = getCafeaDupaId(&q,5);
	afisareCafea(c);
	free(c.denumire);
	free(c.origine);
	dezalocareCoada(&q);
	return 0;
}