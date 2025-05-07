#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct {
    int id;
    char* nume;
    char* specie;
    int varsta;
    float greutate;
    char esteVaccinat; // 'D' sau 'N'
} Animal;

typedef struct NodLista {
    Animal info;
    struct NodLista* next;
} NodLista;

Animal initializareAnimal(int id, const char* nume, const char* specie, int varsta, float greutate, char esteVaccinat) {
    Animal a;
    a.id = id;
    a.nume = strdup(nume);
    a.specie = strdup(specie);
    a.varsta = varsta;
    a.greutate = greutate;
    a.esteVaccinat = esteVaccinat;
    return a;
}

void afiseazaAnimal(Animal a) {
    printf("ID: %d | Nume: %s | Specie: %s | Varsta: %d | Greutate: %.2f | Vaccinat: %c\n",
        a.id, a.nume, a.specie, a.varsta, a.greutate, a.esteVaccinat);
}

void stergeAnimal(Animal* a) {
    free(a->nume);
    free(a->specie);
}

// Citire animal din linie
Animal citesteAnimalDinFisier(FILE* f) {
    char buffer[256];
    fgets(buffer, 256, f);
    char* token = strtok(buffer, ",");
    int id = atoi(token);

    token = strtok_s(NULL, strlen(",")+1, ",");
    char* nume = strdup(token);

    token = strtok_s(NULL, strlen(",") + 1, ",");
    char* specie = strdup(token);

    token = strtok_s(NULL, strlen(",") + 1, ",");
    int varsta = atoi(token);

    token = strtok_s(NULL, strlen(",") + 1, ",");
    float greutate = atof(token);

    token = strtok_s(NULL, strlen(",\n") + 1, ",\n");
    char esteVaccinat = token[0];

    Animal a = initializareAnimal(id, nume, specie, varsta, greutate, esteVaccinat);
    free(nume);
    free(specie);
    return a;
}

// Citire vector animale
Animal* citesteVectorAnimale(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;

    Animal* vector = NULL;
    *dim = 0;
    Animal temp;
    while (!feof(f)) {
        temp = citesteAnimalDinFisier(f);
        vector = realloc(vector, ((*dim) + 1) * sizeof(Animal));
        vector[*dim] = temp;
        (*dim)++;
    }
    fclose(f);
    return vector;
}

// Filtrare vaccinate
Animal* copiazaAnimaleVaccinate(Animal* vector, int dim, int* dimNoua) {
    *dimNoua = 0;
    for (int i = 0; i < dim; i++)
        if (vector[i].esteVaccinat == 'D') (*dimNoua)++;

    Animal* nou = malloc((*dimNoua) * sizeof(Animal));
    int j = 0;
    for (int i = 0; i < dim; i++)
        if (vector[i].esteVaccinat == 'D')
            nou[j++] = initializareAnimal(vector[i].id, vector[i].nume, vector[i].specie, vector[i].varsta, vector[i].greutate, vector[i].esteVaccinat);

    return nou;
}

// Afisare vector
void afiseazaVectorAnimale(Animal* v, int n) {
    for (int i = 0; i < n; i++)
        afiseazaAnimal(v[i]);
}

// Eliberare vector
void stergeVectorAnimale(Animal* v, int n) {
    for (int i = 0; i < n; i++)
        stergeAnimal(&v[i]);
    free(v);
}

// Adaugare in lista
void adaugaAnimalInLista(NodLista** cap, Animal a) {
    NodLista* nou = malloc(sizeof(NodLista));
    nou->info = initializareAnimal(a.id, a.nume, a.specie, a.varsta, a.greutate, a.esteVaccinat);
    nou->next = NULL;
    if (*cap == NULL)
        *cap = nou;
    else {
        NodLista* p = *cap;
        while (p->next)
            p = p->next;
        p->next = nou;
    }
}

// Citire lista animale din fisier
NodLista* citesteListaAnimale(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) return NULL;
    NodLista* cap = NULL;
    while (!feof(f)) {
        Animal a = citesteAnimalDinFisier(f);
        adaugaAnimalInLista(&cap, a);
        stergeAnimal(&a);
    }
    fclose(f);
    return cap;
}

// Afisare lista
void afiseazaListaAnimale(NodLista* cap) {
    while (cap) {
        afiseazaAnimal(cap->info);
        cap = cap->next;
    }
}

// Eliberare lista
void stergeListaAnimale(NodLista** cap) {
    while (*cap) {
        NodLista* temp = *cap;
        *cap = (*cap)->next;
        stergeAnimal(&temp->info);
        free(temp);
    }
}

// Functia principala
int main() {
    // 1. Creare animal individual
    Animal a1 = initializareAnimal(100, "Rex", "caine", 3, 27.5, 'D');
    printf("=== Animal creat manual ===\n");
    afiseazaAnimal(a1);

    // 2. Citire vector din fișier
    int dim = 0;
    Animal* vector = citesteVectorAnimale("animale.txt", &dim);
    printf("\n=== Vector animale din fișier ===\n");
    afiseazaVectorAnimale(vector, dim);

    // 3. Filtrare animale vaccinate
    int dimVaccinate = 0;
    Animal* vaccinate = copiazaAnimaleVaccinate(vector, dim, &dimVaccinate);
    printf("\n=== Animale vaccinate ===\n");
    afiseazaVectorAnimale(vaccinate, dimVaccinate);

    // 4. Citire și afișare listă
    NodLista* lista = citesteListaAnimale("animale.txt");
    printf("\n=== Lista animale ===\n");
    afiseazaListaAnimale(lista);

    // 5. Eliberare memorie
    stergeAnimal(&a1);
    stergeVectorAnimale(vector, dim);
    stergeVectorAnimale(vaccinate, dimVaccinate);
    stergeListaAnimale(&lista);

    return 0;
}
//'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C and C++ conformant name: _strdup