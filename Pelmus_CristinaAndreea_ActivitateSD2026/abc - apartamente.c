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
//void adaugaApartamentInArbore(Nod** radacina, Apartament a) {
//    if (*radacina == NULL) {
//        Nod* nou = malloc(sizeof(Nod));
//        nou->dreapta = NULL;
//        nou->stanga = NULL;
//        nou->info = a;
//        *radacina = nou;
//    }
//    else {
//        if (a.id < (*radacina)->info.id)
//            adaugaApartamentInArbore(&(*radacina)->stanga, a);
//        else
//            adaugaApartamentInArbore(&(*radacina)->dreapta, a);
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
//void afisareInordine(Nod* arbore) {
//    if (arbore) {
//        afisareInordine(arbore->stanga);
//        afisareApartament(arbore->info);
//        afisareInordine(arbore->dreapta);
//    }
//}
//
//void afisarePreordine(Nod* arbore) {
//    if (arbore) {
//        afisareApartament(arbore->info);
//        afisarePreordine(arbore->stanga);
//        afisarePreordine(arbore->dreapta);
//    }
//}
//
//void afisarePostordine(Nod* arbore) {
//    if (arbore) {
//        afisarePostordine(arbore->stanga);
//        afisarePostordine(arbore->dreapta);
//        afisareApartament(arbore->info);
//    }
//}
//
//Apartament getApartamentDupaId(Nod* radacina, int id) {
//    Apartament a;
//    a.id = 0;
//    if (radacina) {
//        if (id == radacina->info.id) {
//            a = radacina->info;
//            a.oras = malloc(strlen(radacina->info.oras));
//            strcpy(a.oras, radacina->info.oras);
//        }
//        else
//            if (id < radacina->info.id)
//                a=getApartamentDupaId(radacina->stanga, id);
//            else
//                a=getApartamentDupaId(radacina->dreapta, id);
//    }
//    return a;
//}
//
//int determinaNumarNoduri(Nod* radacina) {
//    if (radacina) {
//        int nrStanga = determinaNumarNoduri(radacina->stanga);
//        int nrDreapta = determinaNumarNoduri(radacina->dreapta);
//        return 1 + nrDreapta + nrStanga;
//    }
//    return 0;
//}
//
//int calculeazaInaltimeArbore(Nod* radacina) {
//    if (radacina == NULL) return -1;
//    return 1+max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
//}
//
//float calculeazaPretTotal(Nod* radacina) {
//    float total = 0;
//    if (radacina) {
//        total += radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
//    }
//    return total;
//}
//
//float pretApartamenteOras(Nod* radacina, const char* oras) {
//    float total = 0;
//    if (radacina) {
//        total += pretApartamenteOras(radacina->stanga, oras) + pretApartamenteOras(radacina->dreapta, oras);
//        if (strcmp(radacina->info.oras, oras) == 0)
//            total += radacina->info.pret;
//    }
//    return total;
//}
//
//void dezalocareArbore(Nod** radacina) {
//    if (*radacina) {
//        dezalocareArbore(&(*radacina)->stanga);
//        dezalocareArbore(&(*radacina)->dreapta);
//        free((*radacina)->info.oras);
//        free(*radacina);
//        *radacina = NULL;
//    }
//}
//int main() {
//    Nod* arbore = citireArboreDinFisier("apartamente_arbore.txt");
//    afisareInordine(arbore);
//    Apartament a = getApartamentDupaId(arbore,5);
//    printf("\n\nApartamentul cu id ul 5 este: ");
//    afisareApartament(a);
//    printf("\n\nIn total sunt %d noduri", determinaNumarNoduri(arbore));
//    printf("\n\nInaltimea arborelui este: %d", calculeazaInaltimeArbore(arbore));
//    printf("\n\nPretul total al apartamentelor este: %.2f", calculeazaPretTotal(arbore));
//    printf("\n\nPretul apartamentelor din Bucuresti este: %.2f", pretApartamenteOras(arbore,"Bucuresti"));
//    dezalocareArbore(&arbore);
//	return 0;
//}