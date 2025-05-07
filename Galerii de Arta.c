#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct {
    int cod;
    char* titlu;
    char* autor;
    int an_creatie;
    float pret;
    char esteExpus;
} Tablou;

//Lista simplu inlantuita
typedef struct NodLista {
    Tablou tablou;
    struct NodLista* next;
} NodLista;

Tablou initializareTablou(int cod, const char* titlu, const char* autor, int an, float pret, char esteExpus) {
    Tablou t;
    t.cod = cod;
    t.titlu = strdup(titlu);
    t.autor = strdup(autor);
    t.an_creatie = an;
    t.pret = pret;
    t.esteExpus = esteExpus;
    return t;
}

// Citire tablou din fisier
Tablou citesteTablouDinFisier(FILE* f) {
    char buffer[256];
    fgets(buffer, sizeof(buffer), f);

    char* token = strtok(buffer, ",");
    int cod = atoi(token);

    token = strtok_s(NULL, strlen(",")+1, ",");
    char* titlu = strdup(token);

    token = strtok_s(NULL, strlen(",") + 1, ",");
    char* autor = strdup(token);

    token = strtok_s(NULL, strlen(",") + 1, ",");
    int an = atoi(token);

    token = strtok_s(NULL, strlen(",") + 1, ",");
    float pret = atof(token);

    token = strtok_s(NULL, strlen(",\n") + 1, ",\n");
    char esteExpus = token[0];

    Tablou t = initializareTablou(cod, titlu, autor, an, pret, esteExpus);

    free(titlu);
    free(autor);

    return t;
}

void adaugaTablouInLista(NodLista** cap, Tablou t) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->tablou = t;
    nou->next = NULL;

    if (*cap == NULL) {
        *cap = nou;
    }
    else {
        NodLista* temp = *cap;
        while (temp->next)
            temp = temp->next;
        temp->next = nou;
    }
}

NodLista* citesteListaTablouri(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        perror("Eroare la deschiderea fișierului");
        return NULL;
    }

    int nr;
    fscanf_s(f, "%d\n", &nr);
    NodLista* lista = NULL;
    for (int i = 0; i < nr; i++) {
        Tablou t = citesteTablouDinFisier(f);
        adaugaTablouInLista(&lista, t);
    }

    fclose(f);
    return lista;
}

void afiseazaTablou(Tablou t) {
    printf("Cod: %d | Titlu: %s | Autor: %s | An: %d | Pret: %.2f | %s\n",
        t.cod, t.titlu, t.autor, t.an_creatie, t.pret,
        t.esteExpus == 'E' ? "Expus" : "Neexpus");
}

void afiseazaListaTablouri(NodLista* cap) {
    NodLista* temp = cap;
    while (temp) {
        afiseazaTablou(temp->tablou);
        temp = temp->next;
    }
}

void stergeListaTablouri(NodLista** cap) {
    while (*cap) {
        NodLista* temp = *cap;
        *cap = (*cap)->next;

        free(temp->tablou.titlu);
        free(temp->tablou.autor);
        free(temp);
    }
}

NodLista* copiazaTablouriExpuse(NodLista* cap) {
    NodLista* listaNoua = NULL;

    NodLista* temp = cap;
    while (temp) {
        if (temp->tablou.esteExpus == 'E') {
            Tablou t = temp->tablou;
            adaugaTablouInLista(&listaNoua, t);
        }
        temp = temp->next;
    }

    return listaNoua;
}

int main() {
    // 1. Crearea unui tablou
    Tablou tablou1 = initializareTablou(1, "Steaua nopții", "Van Gogh", 1889, 15000.0, 'E');

    // 2. Citirea unui vector de tablouri din fisier
    NodLista* listaTablouri = citesteListaTablouri("tablouri.txt");

    printf("=== Lista Tablouri ===\n");
    afiseazaListaTablouri(listaTablouri);

    // 3. Filtrarea tablourilor expuse
    NodLista* listaTablouriExpuse = copiazaTablouriExpuse(listaTablouri);

    printf("\n=== Lista Tablouri Expuse ===\n");
    afiseazaListaTablouri(listaTablouriExpuse);

    // 4. Citirea si afisarea tabloului creat
    printf("\n=== Tablou Creat ===\n");
    afiseazaTablou(tablou1);

    // 5. Eliberarea corecta a memoriei
    stergeListaTablouri(&listaTablouri);
    stergeListaTablouri(&listaTablouriExpuse);

    return 0;
}
//'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C and C++ conformant name: _strdup.