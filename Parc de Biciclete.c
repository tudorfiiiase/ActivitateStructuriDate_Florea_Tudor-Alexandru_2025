#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct {
    int id;
    char* model;
    int anFabricatie;
    float pret;
    int esteElectrica;
} Bicicleta;

typedef struct NodLista {
    Bicicleta b;
    struct NodLista* next;
} NodLista;

// Initializare bicicleta
Bicicleta initializareBicicleta(int id, const char* model, int an, float pret, int electrica) {
    Bicicleta b;
    b.id = id;
    b.model = (char*)malloc(strlen(model) + 1);
    strcpy_s(b.model, strlen(model)+1, model);
    b.anFabricatie = an;
    b.pret = pret;
    b.esteElectrica = electrica;
    return b;
}

// Afisare bicicleta
void afiseazaBicicleta(Bicicleta b) {
    printf("ID: %d | Model: %s | An: %d | Pret: %.2f | Electrica: %d\n",
        b.id, b.model, b.anFabricatie, b.pret, b.esteElectrica);
}

// Afisare vector
void afiseazaVectorBiciclete(Bicicleta* v, int dim) {
    for (int i = 0; i < dim; i++)
        afiseazaBicicleta(v[i]);
}

// Citire vector din fisier
Bicicleta* citesteVectorBiciclete(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului.\n");
        *dim = 0;
        return NULL;
    }

    fscanf_s(f, "%d", dim);
    Bicicleta* v = (Bicicleta*)malloc(sizeof(Bicicleta) * (*dim));

    for (int i = 0; i < *dim; i++) {
        int id, an, electrica;
        float pret;
        char model[100];
        fscanf_s(f, "%d,%99[^,],%d,%f,%d", &id, model, &an, &pret, &electrica);
        v[i] = initializareBicicleta(id, model, an, pret, electrica);
    }

    fclose(f);
    return v;
}

// Filtrare biciclete electrice
Bicicleta* copiazaBicicleteElectrice(Bicicleta* v, int dim, int* dimNoua) {
    Bicicleta* rezultat = (Bicicleta*)malloc(sizeof(Bicicleta) * dim);
    *dimNoua = 0;

    for (int i = 0; i < dim; i++) {
        if (v[i].esteElectrica) {
            rezultat[*dimNoua] = initializareBicicleta(v[i].id, v[i].model, v[i].anFabricatie, v[i].pret, v[i].esteElectrica);
            (*dimNoua)++;
        }
    }
    return rezultat;
}

// Adaugare bicicleta in lista
void adaugaBicicletaInLista(NodLista** cap, Bicicleta b) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->b = initializareBicicleta(b.id, b.model, b.anFabricatie, b.pret, b.esteElectrica);
    nou->next = NULL;

    if (*cap == NULL)
        *cap = nou;
    else {
        NodLista* temp = *cap;
        while (temp->next)
            temp = temp->next;
        temp->next = nou;
    }
}

// Citire lista din fisier
NodLista* citesteListaBiciclete(const char* numeFisier) {
    int dim;
    Bicicleta* v = citesteVectorBiciclete(numeFisier, &dim);
    if (!v) return NULL;

    NodLista* lista = NULL;
    for (int i = 0; i < dim; i++) {
        adaugaBicicletaInLista(&lista, v[i]);
    }

    // Eliberam vectorul temporar
    for (int i = 0; i < dim; i++) {
        free(v[i].model);
    }
    free(v);
    return lista;
}

// Afisare lista
void afiseazaListaBiciclete(NodLista* cap) {
    NodLista* temp = cap;
    while (temp) {
        afiseazaBicicleta(temp->b);
        temp = temp->next;
    }
}

// Eliberare vector
void stergeVectorBiciclete(Bicicleta** v, int* dim) {
    for (int i = 0; i < *dim; i++) {
        free((*v)[i].model);
    }
    free(*v);
    *v = NULL;
    *dim = 0;
}

// Eliberare lista
void stergeListaBiciclete(NodLista** cap) {
    while (*cap) {
        NodLista* temp = *cap;
        *cap = (*cap)->next;
        free(temp->b.model);
        free(temp);
    }
}

int main() {
    // 1. Creare manuala bicicleta
    Bicicleta b1 = initializareBicicleta(100, "TestBike", 2024, 3200.0, 1);
    printf("Bicicleta creata manual:\n");
    afiseazaBicicleta(b1);
    free(b1.model);  // Eliberam imediat

    // 2. Citire vector din fisier
    int dim = 0;
    Bicicleta* biciclete = citesteVectorBiciclete("biciclete.txt", &dim);
    if (!biciclete) return 1;

    printf("\nVectorul citit din fisier:\n");
    afiseazaVectorBiciclete(biciclete, dim);

    // 3. Filtrare biciclete electrice
    int dimElectrice = 0;
    Bicicleta* electrice = copiazaBicicleteElectrice(biciclete, dim, &dimElectrice);
    printf("\nBiciclete electrice:\n");
    afiseazaVectorBiciclete(electrice, dimElectrice);

    // 4. Citire si afisare lista
    printf("\nLista inlantuita din fisier:\n");
    NodLista* lista = citesteListaBiciclete("biciclete.txt");
    afiseazaListaBiciclete(lista);

    // 5. Eliberare memorie
    stergeVectorBiciclete(&biciclete, &dim);
    stergeVectorBiciclete(&electrice, &dimElectrice);
    stergeListaBiciclete(&lista);

    return 0;
}





