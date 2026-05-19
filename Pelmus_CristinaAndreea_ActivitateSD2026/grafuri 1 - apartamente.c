#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Apartament Apartament;
typedef struct NodPrincipal NodP;
typedef struct NodSecundar NodS;

struct Apartament {
    int id;
    int camere;
    float suprafata;
    float pret;
    char* oras;
};

struct NodPrincipal {
    Apartament info;
    NodP* next;
    NodS* vecini;
};

struct NodSecundar {
    NodP* info;
    NodS* next;
};

Apartament citireApartamentDinFisier(FILE* f) {
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

void adaugareInListaPrincipala(NodP** cap, Apartament a) {
    NodP* nou = malloc(sizeof(NodP));
    nou->info = a;
    nou->next = NULL;
    nou->vecini = NULL;
    if (*cap) {
        NodP* p = *cap;
        while (p->next)
            p = p->next;
        p->next = nou;
    }
    else
        *cap = nou;
}

void adaugareInListaSecundara(NodS** cap, NodP* listaP) {
    NodS* nou = malloc(sizeof(NodS));
    nou->info = listaP;
    nou->next = NULL;
    if (*cap) {
        NodS* s = *cap;
        while (s->next)
            s = s->next;
        s->next = nou;
    }
    else
        *cap = nou;
}

NodP * citireApartamenteDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodP* listaP = NULL;
    while (!feof(f)) {
        adaugareInListaPrincipala(&listaP, citireApartamentDinFisier(f));
    }
    fclose(f);
    return listaP;
}

NodP* cautaNodDupaId(int id, NodP* listaP) {
    while (listaP&&listaP->info.id!=id) {
        listaP = listaP->next;
    }
    return listaP;
}

void inserareMuchie(int stop, int start, NodP *listaP) {
    NodP* nodStart = cautaNodDupaId(start, listaP);
    NodP* nodStop = cautaNodDupaId(stop, listaP);

    if (nodStart && nodStop) {
        adaugareInListaSecundara(&nodStart->vecini, nodStop);
        adaugareInListaSecundara(&nodStop->vecini, nodStart);
    }
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
    FILE* f = fopen(numeFisier, "r");
    int start, stop;
    while (!feof(f)) {
        fscanf(f, "%d %d", &start, &stop);
        inserareMuchie(stop, start, graf);
    }
    fclose(f);
}

void dezalocareNoduriGraf(NodP** graf) {
    if (*graf) {
        NodP* p = *graf;
        while (p) {
            NodS* s = p->vecini;
            while (s) {
                NodS* aux = s->next;
                free(s);
                s = aux;
            }
            NodP* aux = p->next;
            free(p->info.oras);
            free(p);
            p = aux;
        }
        *graf = NULL;
    }
}

void afiseazaVecini(int id, NodP* graf) {
    NodP* p = cautaNodDupaId(id, graf);
    NodS* s = p->vecini;
    while (s) {
        afisareApartament(s->info->info);
        s = s->next;
    }
}
int main() {
    NodP* graf = citireApartamenteDinFisier("apartamente.txt");
    citireMuchiiDinFisier("muchii.txt", graf);
    afiseazaVecini(5, graf);
    return 0;
}