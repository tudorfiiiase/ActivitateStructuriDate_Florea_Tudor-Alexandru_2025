#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct {
    int id;
    char* nume;
    char* tip;
    int capacitate;
    float durata;
    bool esteDeschisa;
} Attraction;

typedef struct {
    Attraction* vector;
    int dim;
} VectorAttractions;

typedef struct NodLista {
    Attraction atr;
    struct NodLista* next;
} NodLista;

//Functii de baza

Attraction initializareAttraction(int id, const char* nume, const char* tip, int capacitate, float durata, bool deschisa) {
    Attraction a;
    a.id = id;
    a.capacitate = capacitate;
    a.durata = durata;
    a.esteDeschisa = deschisa;

    a.nume = malloc(strlen(nume) + 1);
    strcpy_s(a.nume, strlen(nume)+1, nume);

    a.tip = malloc(strlen(tip) + 1);
    strcpy_s(a.tip, strlen(tip)+1, tip);

    return a;
}

void afiseazaAttraction(Attraction a) {
    printf("ID: %d | Nume: %s | Tip: %s | Capacitate: %d | Durata: %.2f | Deschisa: %s\n",
        a.id, a.nume, a.tip, a.capacitate, a.durata, a.esteDeschisa ? "Da" : "Nu");
}

void afiseazaVectorAttractions(Attraction* v, int dim) {
    for (int i = 0; i < dim; i++) {
        afiseazaAttraction(v[i]);
    }
}

//Filtrare vector

VectorAttractions copiazaAttractionsDeschise(Attraction* vector, int dim) {
    VectorAttractions rezultat;
    rezultat.vector = NULL;
    rezultat.dim = 0;

    for (int i = 0; i < dim; i++) {
        if (vector[i].esteDeschisa && vector[i].durata > 4.0f) {
            rezultat.vector = realloc(rezultat.vector, (rezultat.dim + 1) * sizeof(Attraction));
            rezultat.vector[rezultat.dim] = initializareAttraction(
                vector[i].id,
                vector[i].nume,
                vector[i].tip,
                vector[i].capacitate,
                vector[i].durata,
                vector[i].esteDeschisa
            );
            rezultat.dim++;
        }
    }

    return rezultat;
}

void stergeVectorAttractions(VectorAttractions* v) {
    for (int i = 0; i < v->dim; i++) {
        free(v->vector[i].nume);
        free(v->vector[i].tip);
    }
    free(v->vector);
    v->vector = NULL;
    v->dim = 0;
}

//Fisier text CSV

Attraction citesteAttractionDinFisier(FILE* f) {
    int id, capacitate, deschisa;
    float durata;
    char nume[100], tip[100];

    fscanf_s(f, "%d,%99[^,],%99[^,],%d,%f,%d\n", &id, nume, tip, &capacitate, &durata, &deschisa);
    return initializareAttraction(id, nume, tip, capacitate, durata, deschisa);
}

Attraction* citesteVectorAttractions(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Attraction* v = NULL;
    *dim = 0;

    while (!feof(f)) {
        Attraction a = citesteAttractionDinFisier(f);
        v = realloc(v, (*dim + 1) * sizeof(Attraction));
        v[*dim] = a;
        (*dim)++;
    }

    fclose(f);
    return v;
}

//Lista simpla inlantuita

NodLista* adaugaAttractionInLista(NodLista* cap, Attraction a) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->atr = initializareAttraction(a.id, a.nume, a.tip, a.capacitate, a.durata, a.esteDeschisa);
    nou->next = NULL;

    if (!cap) return nou;

    NodLista* p = cap;
    while (p->next) p = p->next;
    p->next = nou;
    return cap;
}

NodLista* citesteListaAttractions(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    NodLista* cap = NULL;
    while (!feof(f)) {
        Attraction a = citesteAttractionDinFisier(f);
        cap = adaugaAttractionInLista(cap, a);
    }

    fclose(f);
    return cap;
}

void afiseazaListaAttractions(NodLista* cap) {
    while (cap) {
        afiseazaAttraction(cap->atr);
        cap = cap->next;
    }
}

void stergeListaAttractions(NodLista** cap) {
    while (*cap) {
        NodLista* temp = *cap;
        *cap = (*cap)->next;
        free(temp->atr.nume);
        free(temp->atr.tip);
        free(temp);
    }
}

int main() {
    printf("=== Demonstrație atracții ===\n\n");

    // 1. Creare atractie
    Attraction a = initializareAttraction(1, "Montagne Russe", "montagne russe", 200, 5.0f, true);
    printf(">> Atracție creată manual:\n");
    afiseazaAttraction(a);
    printf("\n");

    // 2. Citire vector atractii din fisier
    int dim = 0;
    Attraction* v = citesteVectorAttractions("attractii.txt", &dim);
    printf(">> Vector de atracții din fișier:\n");
    afiseazaVectorAttractions(v, dim);
    printf("\n");

    // 3. Filtrare: atractii cu capacitate > 100
    VectorAttractions filtrate;
    filtrate.vector = NULL;
    filtrate.dim = 0;

    for (int i = 0; i < dim; i++) {
        if (v[i].capacitate > 100) {
            filtrate.vector = realloc(filtrate.vector, (filtrate.dim + 1) * sizeof(Attraction));
            filtrate.vector[filtrate.dim] = initializareAttraction(
                v[i].id, v[i].nume, v[i].tip, v[i].capacitate, v[i].durata, v[i].esteDeschisa
            );
            filtrate.dim++;
        }
    }

    printf(">> Atracții cu capacitate > 100 persoane/oră:\n");
    afiseazaVectorAttractions(filtrate.vector, filtrate.dim);
    printf("\n");

    // 4. Citire si afisare lista atractii
    NodLista* lista = citesteListaAttractions("attractii.txt");
    printf(">> Lista atracții din fișier:\n");
    afiseazaListaAttractions(lista);
    printf("\n");

    // 5. Eliberare memorie
    stergeVectorAttractions(&filtrate);
    for (int i = 0; i < dim; i++) {
        free(v[i].nume);
        free(v[i].tip);
    }
    free(v);

    stergeListaAttractions(&lista);
    free(a.nume);
    free(a.tip);

    printf(">>> Toată memoria a fost eliberată cu succes.\n");

    return 0;
}




