//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//
//typedef struct Apartament Apartament;
//typedef struct Nod Nod;
//
//struct Apartament {
//    int id;
//    int camere;
//    float suprafata;
//    float pret;
//    char* oras;
//};
//
//struct Nod {
//    Apartament info;
//    Nod* stanga;
//    Nod* dreapta;
//};
//
//Apartament citireApartamentDinFisier(FILE* f) {
//    char buffer[100];
//    char sep[3] = ",\n";
//    char* aux;
//    Apartament a;
//    fgets(buffer, 100, f);
//    a.id = atoi(strtok(buffer, sep));
//    a.camere = atoi(strtok(NULL, sep));
//    a.suprafata = atof(strtok(NULL, sep));
//    a.pret = atof(strtok(NULL, sep));
//    aux = strtok(NULL, sep);
//    a.oras = malloc(strlen(aux) + 1);
//    strcpy(a.oras, aux);
//    return a;
//}
//
//void afisareApartament(Apartament a) {
//    printf("\nid: %d", a.id);
//    printf("\nnumar camere: %d", a.camere);
//    printf("\nsuprafata: %.2f", a.suprafata);
//    printf("\npret: %.2f", a.pret);
//    printf("\noras: %s", a.oras);
//}
//
//int inaltimeArbore(Nod* arbore) {
//    if(arbore)
//        return 1 + max(inaltimeArbore(arbore->stanga),inaltimeArbore(arbore->dreapta));
//    return 0;
//}
//
//int gradEchilibru(Nod* arbore) {
//    if(arbore)
//        return inaltimeArbore(arbore->stanga) - inaltimeArbore(arbore->dreapta);
//    return 0;
//}
//
//void rotireStanga(Nod** arbore) {
//    if (*arbore && (*arbore)->dreapta) {
//        Nod* aux = (*arbore)->dreapta;
//        (*arbore)->dreapta = aux->stanga;
//        aux->stanga = *arbore;
//        *arbore = aux;
//    }
//}
//
//void rotireDreapta(Nod** arbore) {
//    if (*arbore && (*arbore)->stanga) {
//        Nod* aux = (*arbore)->stanga;
//        (*arbore)->stanga = aux->dreapta;
//        aux->dreapta = *arbore;
//        *arbore = aux;
//    }
//}
//
//void adaugaApartamentInArbore(Nod** arbore, Apartament a) {
//    if (*arbore) {
//        if (a.id < (*arbore)->info.id)
//            adaugaApartamentInArbore(&(*arbore)->stanga, a);
//        else
//            adaugaApartamentInArbore(&(*arbore)->dreapta, a);
//
//        int gr = gradEchilibru(*arbore);
//        if (gr == 2) {
//            if(gradEchilibru((*arbore)->dreapta)==-1)
//                rotireStanga(arbore);
//            rotireDreapta(arbore);
//        }
//        else if (gr == -2) {
//            if(gradEchilibru((*arbore)->stanga)==1)
//                rotireDreapta(arbore);
//            rotireStanga(arbore);
//        }
//    }
//    else {
//        Nod* nou = malloc(sizeof(Nod));
//        nou->stanga = NULL;
//        nou->dreapta = NULL;
//        nou->info = a;
//        *arbore = nou;
//    }
//}
//
//Nod* citireArboreDinFisier(const char* numeFisier) {
//    FILE* f = fopen(numeFisier, "r");
//    Nod* arbore = NULL;
//    while (!feof(f)) {
//        adaugaApartamentInArbore(&arbore, citireApartamentDinFisier(f));
//    }
//    fclose(f);
//    return arbore;
//}
//
////afisare inordine (srd)
//void afisareArbore(Nod* arbore) {
//    if (arbore) {
//        afisareArbore(arbore->stanga);
//        afisareApartament(arbore->info);
//        afisareArbore(arbore->dreapta);
//    }
//}
//
//void dezalocareArbore(Nod** arbore) {
//    if (*arbore) {
//        dezalocareArbore(&(*arbore)->stanga);
//        dezalocareArbore(&(*arbore)->dreapta);
//
//        free((*arbore)->info.oras);
//        free(*arbore);
//
//        *arbore = NULL;
//    }
//}
//int main() {
//    Nod* arbore = citireArboreDinFisier("apartamente_arbore.txt");
//    afisareArbore(arbore);
//    return 0;
//}