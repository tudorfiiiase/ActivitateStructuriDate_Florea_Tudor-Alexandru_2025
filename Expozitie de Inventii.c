#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct {
    int id;
    char* denumire;
    char* inventator;
    int an_inventie;
    float valoare;
    char estePremiata; // 'D' sau 'N'
} Inventie;

typedef struct {
    Inventie* inventii;
    int dim;
} VectorInventii;

typedef struct NodLista {
    Inventie inv;
    struct NodLista* next;
} NodLista;

// Functii de initializare si afisare
Inventie initializareInventie(int id, const char* denumire, const char* inventator, int an, float valoare, char estePremiata) {
    Inventie i;
    i.id = id;
    i.denumire = strdup(denumire);
    i.inventator = strdup(inventator);
    i.an_inventie = an;
    i.valoare = valoare;
    i.estePremiata = estePremiata;
    return i;
}

void afiseazaInventie(Inventie i) {
    printf("ID: %d, Denumire: %s, Inventator: %s, An: %d, Valoare: %.2f, Premiata: %c\n",
        i.id, i.denumire, i.inventator, i.an_inventie, i.valoare, i.estePremiata);
}

void afiseazaVectorInventii(VectorInventii v) {
    for (int i = 0; i < v.dim; i++) {
        afiseazaInventie(v.inventii[i]);
    }
}

float calculeazaValoareMedie(VectorInventii v) {
    float suma = 0;
    for (int i = 0; i < v.dim; i++) {
        suma += v.inventii[i].valoare;
    }
    return v.dim > 0 ? suma / v.dim : 0;
}

VectorInventii copiazaInventiiPremiate(VectorInventii v) {
    VectorInventii rezultat = { NULL, 0 };
    for (int i = 0; i < v.dim; i++) {
        if (v.inventii[i].estePremiata == 'D') {
            rezultat.inventii = realloc(rezultat.inventii, (rezultat.dim + 1) * sizeof(Inventie));
            rezultat.inventii[rezultat.dim++] = initializareInventie(
                v.inventii[i].id, v.inventii[i].denumire,
                v.inventii[i].inventator, v.inventii[i].an_inventie,
                v.inventii[i].valoare, v.inventii[i].estePremiata);
        }
    }
    return rezultat;
}

Inventie* getInventieDupaDenumire(VectorInventii v, const char* denumire) {
    for (int i = 0; i < v.dim; i++) {
        if (strcmp(v.inventii[i].denumire, denumire) == 0) {
            return &v.inventii[i];
        }
    }
    return NULL;
}

void stergeVectorInventii(VectorInventii* v) {
    for (int i = 0; i < v->dim; i++) {
        free(v->inventii[i].denumire);
        free(v->inventii[i].inventator);
    }
    free(v->inventii);
    v->inventii = NULL;
    v->dim = 0;
}

Inventie citesteInventieDinFisier(FILE* f) {
    int id, an;
    float valoare;
    char buffer[256], denumire[100], inventator[100], estePremiata;

    if (fgets(buffer, sizeof(buffer), f)) {
        sscanf_s(buffer, "%d,%[^,],%[^,],%d,%f,%c",
            &id, denumire, inventator, &an, &valoare, &estePremiata);
        return initializareInventie(id, denumire, inventator, an, valoare, estePremiata);
    }
    else {
        return initializareInventie(0, "", "", 0, 0, 'N');
    }
}

VectorInventii citesteVectorInventii(const char* numeFisier) {
    VectorInventii v = { NULL, 0 };
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            Inventie i = citesteInventieDinFisier(f);
            if (i.id != 0) {
                v.inventii = realloc(v.inventii, (v.dim + 1) * sizeof(Inventie));
                v.inventii[v.dim++] = i;
            }
        }
        fclose(f);
    }
    return v;
}

// Lista simplu inlantuita
NodLista* adaugaInventieInLista(NodLista* cap, Inventie i) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->inv = initializareInventie(i.id, i.denumire, i.inventator, i.an_inventie, i.valoare, i.estePremiata);
    nou->next = NULL;

    if (!cap) return nou;

    NodLista* tmp = cap;
    while (tmp->next) tmp = tmp->next;
    tmp->next = nou;
    return cap;
}

NodLista* citesteListaInventii(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodLista* cap = NULL;
    if (f) {
        while (!feof(f)) {
            Inventie i = citesteInventieDinFisier(f);
            if (i.id != 0)
                cap = adaugaInventieInLista(cap, i);
        }
        fclose(f);
    }
    return cap;
}

void afiseazaListaInventii(NodLista* cap) {
    while (cap) {
        afiseazaInventie(cap->inv);
        cap = cap->next;
    }
}

void stergeListaInventii(NodLista** cap) {
    while (*cap) {
        NodLista* temp = *cap;
        *cap = (*cap)->next;
        free(temp->inv.denumire);
        free(temp->inv.inventator);
        free(temp);
    }
}

int main() {
    // Creare inventie
    Inventie i1 = initializareInventie(1, "Radioul", "Popov", 1895, 10000.5f, 'D');
    afiseazaInventie(i1);

    // Citire vector
    VectorInventii v = citesteVectorInventii("inventii.txt");
    printf("\n-- Toate inventiile --\n");
    afiseazaVectorInventii(v);

    // Filtrare premiate
    VectorInventii premiate = copiazaInventiiPremiate(v);
    printf("\n-- Inventii premiate --\n");
    afiseazaVectorInventii(premiate);

    // Lista
    NodLista* lista = citesteListaInventii("inventii.txt");
    printf("\n-- Lista de inventii --\n");
    afiseazaListaInventii(lista);

    // Eliberare memorie
    stergeVectorInventii(&v);
    stergeVectorInventii(&premiate);
    stergeListaInventii(&lista);

    // Eliberare individuala
    free(i1.denumire);
    free(i1.inventator);

    return 0;
}





