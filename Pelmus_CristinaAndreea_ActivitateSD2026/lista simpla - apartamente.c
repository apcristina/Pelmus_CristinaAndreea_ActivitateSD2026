#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Apartament Apartament;
typedef struct Nod Nod;

struct Apartament {
    int id;
    int camere;
    float suprafata;
    float pret;
    char* oras;
};

struct Nod {
    Nod* next;
    Apartament info;
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

//adaugare la final
void adaugaApartamentInLista(Apartament a, Nod** cap) {
    Nod* nou = malloc(sizeof(Nod));
    nou->next = NULL;
    nou->info = a;
    Nod* p = *cap;
    if ((*cap) != NULL) {
        while (p->next)
            p = p->next;
        p->next = nou;
    }else
        (*cap) = nou;
}

//adaugare la inceput
void adaugaApartamentInListaLaInceput(Apartament a, Nod** cap) {
    Nod* nou = malloc(sizeof(Nod));
    nou->next = *cap;
    nou->info = a;
    *cap = nou;
}

Nod* citesteListaDinFisier(const char *numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* lista = NULL;
    while (!feof(f)) {
        adaugaApartamentInLista(citesteApartamentDinFisier(f), &lista);
    }
    fclose(f);
    return lista;
}

void afisareLista(Nod* cap) {
    while (cap != NULL) {
        afisareApartament(cap->info);
        cap = cap->next;
    }
}

void dezalocareLista(Nod** cap) {
    Nod* p = *cap;
    while (p) {
        Nod* aux = p->next;
        free(p->info.oras);
        free(p);
        p = aux;
    }
    *cap = NULL;
}

//functia sterge apartamentele dintr-un oras primit ca parametru
void stergere(Nod** cap, const char* _oras) {
    while (*cap && strcmp((*cap)->info.oras, _oras) == 0) {
        Nod* aux = *cap;
        *cap = (*cap) -> next;
        free(aux->info.oras);
        free(aux);
    }

    Nod* p = *cap;
    while (p&&p->next) {
        if (strcmp(p->next->info.oras, _oras) == 0) {
            Nod* aux = p->next;
            p->next = aux->next;
            free(aux->info.oras);
            free(aux);
        }
        else
            p = p->next;
    }
}

//functia returneaza un vector format din apartamentele din lista sub un pret primit ca parametru
Apartament* getApartamente(Nod* cap, float _pret, int *dim) {
    *dim = 0;
    Nod* p = cap;
    while (p) {
        if (p->info.pret <= _pret) {
            (*dim) ++;
        }
        p = p->next;
    }
    Apartament *vect = malloc(sizeof(Apartament) * (*dim));
    vect = malloc(sizeof(Apartament) * (*dim));
    int i = 0;
    while (cap) {
        if (cap->info.pret <= _pret) {
            vect[i] = cap->info;
            vect[i].oras = malloc(strlen(cap->info.oras)+1);
            strcpy(vect[i].oras, cap->info.oras);
            i++;
        }
        cap = cap->next;
    }
    return vect;
}

int main() {
    Nod* lista = citesteListaDinFisier("apartamente.txt");
    afisareLista(lista);
    stergere(&lista, "Bucuresti");
    afisareLista(lista);
    int dim;
    Apartament* vect = getApartamente(lista, 100000, &dim);
    printf("\n\n-----------------------------------\nApartamente sub 100000: ");
    for (int i = 0; i < dim; i++)
        afisareApartament(vect[i]);
    dezalocareLista(&lista);
	return 0;
}