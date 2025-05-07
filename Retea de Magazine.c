#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct {
    int id;
    char* nume;
    char* adresa;
    int numarProduse;
    float suprafata;
    char esteDeschis; // 'D' sau 'I'
} Magazin;

typedef struct Nod {
    Magazin info;
    struct Nod* next;
} NodLista;

Magazin initializareMagazin(int id, const char* nume, const char* adresa, int nrProduse, float suprafata, char esteDeschis) {
    Magazin m;
    m.id = id;
    m.nume = strdup(nume);
    m.adresa = strdup(adresa);
    m.numarProduse = nrProduse;
    m.suprafata = suprafata;
    m.esteDeschis = esteDeschis;
    return m;
}

// Afisare
void afiseazaMagazin(Magazin m) {
    printf("ID: %d | Nume: %s | Adresa: %s | Produse: %d | Suprafata: %.2f | %s\n",
        m.id, m.nume, m.adresa, m.numarProduse, m.suprafata,
        m.esteDeschis == 'D' ? "Deschis" : "Inchis");
}

void afiseazaVectorMagazine(Magazin* v, int dim) {
    for (int i = 0; i < dim; i++)
        afiseazaMagazin(v[i]);
}

void stergeVectorMagazine(Magazin* v, int* dim) {
    for (int i = 0; i < *dim; i++) {
        free(v[i].nume);
        free(v[i].adresa);
    }
    free(v);
    *dim = 0;
}

// Citire din fisier
Magazin citesteMagazinDinFisier(FILE* f) {
    int id, nrProduse;
    char bufferNume[100], bufferAdresa[100], deschis[2];
    float suprafata;
    fscanf_s(f, "%d,%99[^,],%99[^,],%d,%f,%1s\n", &id, bufferNume, bufferAdresa, &nrProduse, &suprafata, deschis);
    return initializareMagazin(id, bufferNume, bufferAdresa, nrProduse, suprafata, deschis[0]);
}

Magazin* citesteVectorMagazine(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Eroare deschidere fisier.\n");
        *dim = 0;
        return NULL;
    }

    Magazin* vector = NULL;
    *dim = 0;
    while (!feof(f)) {
        Magazin m = citesteMagazinDinFisier(f);
        vector = (Magazin*)realloc(vector, (*dim + 1) * sizeof(Magazin));
        vector[*dim] = m;
        (*dim)++;
    }

    fclose(f);
    return vector;
}

// Copiaza doar magazinele cu peste 50 produse
Magazin* filtreazaMagazinePeste50Produse(Magazin* v, int dim, int* dimNoua) {
    Magazin* rezultat = NULL;
    *dimNoua = 0;

    for (int i = 0; i < dim; i++) {
        if (v[i].numarProduse > 50) {
            rezultat = (Magazin*)realloc(rezultat, (*dimNoua + 1) * sizeof(Magazin));
            rezultat[*dimNoua] = initializareMagazin(
                v[i].id, v[i].nume, v[i].adresa, v[i].numarProduse, v[i].suprafata, v[i].esteDeschis);
            (*dimNoua)++;
        }
    }

    return rezultat;
}

// Operatii cu lista
NodLista* adaugaMagazinInLista(NodLista* cap, Magazin m) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->info = initializareMagazin(m.id, m.nume, m.adresa, m.numarProduse, m.suprafata, m.esteDeschis);
    nou->next = NULL;

    if (!cap)
        return nou;
    NodLista* temp = cap;
    while (temp->next)
        temp = temp->next;
    temp->next = nou;
    return cap;
}

NodLista* citesteListaMagazine(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    NodLista* cap = NULL;
    while (!feof(f)) {
        Magazin m = citesteMagazinDinFisier(f);
        cap = adaugaMagazinInLista(cap, m);
    }
    fclose(f);
    return cap;
}

void afiseazaListaMagazine(NodLista* cap) {
    while (cap) {
        afiseazaMagazin(cap->info);
        cap = cap->next;
    }
}

void stergeListaMagazine(NodLista** cap) {
    while (*cap) {
        NodLista* temp = *cap;
        *cap = (*cap)->next;
        free(temp->info.nume);
        free(temp->info.adresa);
        free(temp);
    }
}

int main() {
    printf(" DEMO Magazin \n");

    // 1. Creare manuala
    Magazin demo = initializareMagazin(100, "Demo Market", "Strada Test 1", 99, 88.5, 'D');
    printf("\n> Magazin creat manual:\n");
    afiseazaMagazin(demo);
    free(demo.nume);
    free(demo.adresa);

    // 2. Citire vector din fisier
    int dim = 0;
    Magazin* vector = citesteVectorMagazine("magazine.txt", &dim);
    printf("\n> Vector citit din fisier:\n");
    afiseazaVectorMagazine(vector, dim);

    // 3. Filtrare magazine cu peste 50 produse
    int dimFiltrat = 0;
    Magazin* filtrat = filtreazaMagazinePeste50Produse(vector, dim, &dimFiltrat);
    printf("\n> Magazine cu peste 50 produse:\n");
    afiseazaVectorMagazine(filtrat, dimFiltrat);

    // 4. Citire lista din fisier
    NodLista* lista = citesteListaMagazine("magazine.txt");
    printf("\n> Lista citita din fisier:\n");
    afiseazaListaMagazine(lista);

    // 5. Eliberare memorie
    stergeVectorMagazine(vector, &dim);
    stergeVectorMagazine(filtrat, &dimFiltrat);
    stergeListaMagazine(&lista);

    return 0;
}
//'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C and C++ conformant name: _strdup.