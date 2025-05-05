#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct {
    int id;
    char* titlu;
    char* regizor;
    int anPremiera;
    float durata;
    char esteSoldOut;
} Spectacol;

typedef struct NodLista {
    Spectacol spectacol;
    struct NodLista* next;
} NodLista;

Spectacol initializareSpectacol(int id, const char* titlu, const char* regizor, int an, float durata, char soldOut) {
    Spectacol s;
    s.id = id;
    s.titlu = strdup(titlu);
    s.regizor = strdup(regizor);
    s.anPremiera = an;
    s.durata = durata;
    s.esteSoldOut = soldOut;
    return s;
}

// Afisare
void afiseazaSpectacol(Spectacol s) {
    printf("ID: %d | Titlu: %s | Regizor: %s | An: %d | Durata: %.1f | SoldOut: %c\n",
        s.id, s.titlu, s.regizor, s.anPremiera, s.durata, s.esteSoldOut);
}

void afiseazaVectorSpectacole(Spectacol* v, int n) {
    for (int i = 0; i < n; i++) {
        afiseazaSpectacol(v[i]);
    }
}

void afiseazaListaSpectacole(NodLista* cap) {
    while (cap) {
        afiseazaSpectacol(cap->spectacol);
        cap = cap->next;
    }
}

// Citire din fisier a unui spectacol
Spectacol citesteSpectacolDinFisier(FILE* f) {
    int id, an;
    float durata;
    char bufferTitlu[100], bufferRegizor[100], soldOut;

    fscanf_s(f, "%d,%99[^,],%99[^,],%d,%f,%c\n", &id, bufferTitlu, bufferRegizor, &an, &durata, &soldOut);
    return initializareSpectacol(id, bufferTitlu, bufferRegizor, an, durata, soldOut);
}

// Citire vector din fisier
Spectacol* citesteVectorSpectacole(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Spectacol* vector = NULL;
    *dim = 0;
    Spectacol s;

    while (!feof(f)) {
        s = citesteSpectacolDinFisier(f);
        vector = realloc(vector, sizeof(Spectacol) * (*dim + 1));
        vector[*dim] = s;
        (*dim)++;
    }

    fclose(f);
    return vector;
}

// Filtrare spectacole sold out
Spectacol* copiazaSpectacoleSoldOut(Spectacol* vector, int dim, int* dimNoua) {
    *dimNoua = 0;
    for (int i = 0; i < dim; i++) {
        if (vector[i].esteSoldOut == 'D') (*dimNoua)++;
    }

    if (*dimNoua == 0) return NULL;

    Spectacol* rezultat = malloc(sizeof(Spectacol) * (*dimNoua));
    int idx = 0;
    for (int i = 0; i < dim; i++) {
        if (vector[i].esteSoldOut == 'D') {
            rezultat[idx++] = initializareSpectacol(vector[i].id, vector[i].titlu,
                vector[i].regizor, vector[i].anPremiera,
                vector[i].durata, vector[i].esteSoldOut);
        }
    }
    return rezultat;
}

// Citire listă din fisier
NodLista* citesteListaSpectacole(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    NodLista* cap = NULL, * ultim = NULL;
    while (!feof(f)) {
        Spectacol s = citesteSpectacolDinFisier(f);
        NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
        nou->spectacol = s;
        nou->next = NULL;

        if (!cap)
            cap = nou;
        else
            ultim->next = nou;
        ultim = nou;
    }

    fclose(f);
    return cap;
}

// Eliberare memorie
void stergeVectorSpectacole(Spectacol** v, int* dim) {
    if (*v) {
        for (int i = 0; i < *dim; i++) {
            free((*v)[i].titlu);
            free((*v)[i].regizor);
        }
        free(*v);
        *v = NULL;
        *dim = 0;
    }
}

void stergeListaSpectacole(NodLista** cap) {
    while (*cap) {
        NodLista* tmp = *cap;
        *cap = (*cap)->next;
        free(tmp->spectacol.titlu);
        free(tmp->spectacol.regizor);
        free(tmp);
    }
}

int main() {
    // 1. Creare spectacol
    Spectacol s1 = initializareSpectacol(10, "Test", "Anonim", 2023, 130.5f, 'N');
    printf("Spectacol creat manual:\n");
    afiseazaSpectacol(s1);

    // 2. Citire vector din fisier
    int nrSpectacole = 0;
    Spectacol* vector = citesteVectorSpectacole("spectacole.txt", &nrSpectacole);
    printf("\nSpectacole din fișier:\n");
    afiseazaVectorSpectacole(vector, nrSpectacole);

    // 3. Filtrare sold-out
    int nrSoldOut = 0;
    Spectacol* soldOut = copiazaSpectacoleSoldOut(vector, nrSpectacole, &nrSoldOut);
    printf("\nSpectacole SOLD OUT:\n");
    afiseazaVectorSpectacole(soldOut, nrSoldOut);

    // 4. Citire si afisare lista
    NodLista* lista = citesteListaSpectacole("spectacole.txt");
    printf("\nLista spectacole (din fișier):\n");
    afiseazaListaSpectacole(lista);

    // 5. Eliberare memorie
    stergeVectorSpectacole(&vector, &nrSpectacole);
    stergeVectorSpectacole(&soldOut, &nrSoldOut);
    stergeListaSpectacole(&lista);
    free(s1.titlu);
    free(s1.regizor);

    return 0;
}






