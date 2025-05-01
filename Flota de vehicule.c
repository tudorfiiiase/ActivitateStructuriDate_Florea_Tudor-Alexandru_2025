#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#define LOG_FILE "log_vehicule.txt"

// Structura Vehicul
typedef struct Vehicul {
    int cod_unic;
    char* marca;
    int an_fabricatie;
    float kilometraj;
    char tip_vehicul;
    double pret_achizitie;
    struct Vehicul* next;
} Vehicul;

// Cache pentru statistici
typedef struct {
    double valoare_totala_flota;
    int cache_valid;
} CacheStatistici;

CacheStatistici cache = { 0, 0 };

// Functie pentru logging
void logEvent(const char* mesaj) {
    FILE* logFile = fopen(LOG_FILE, "a");
    if (logFile) {
        fprintf(logFile, "%s\n", mesaj);
        fclose(logFile);
    }
}

// Functie de initializare vehicul
Vehicul* initVehicul(int cod, const char* marca, int an, float km, char tip, double pret) {
    if (an < 1900) {
        printf("Eroare: Anul trebuie să fie >= 1900!\n");
        logEvent("Eroare: an_fabricatie invalid!");
        return NULL;
    }

    Vehicul* v = (Vehicul*)malloc(sizeof(Vehicul));
    v->marca = (char*)malloc(strlen(marca) + 1);
    strcpy_s(v->marca, strlen(marca) + 1, marca);
    v->cod_unic = cod;
    v->an_fabricatie = an;
    v->kilometraj = km;
    v->tip_vehicul = tip;
    v->pret_achizitie = pret;
    v->next = NULL;

    logEvent("Vehicul nou adăugat în flotă.");
    cache.cache_valid = 0;  // Resetare cache
    return v;
}

// Inserare vehicul în lista ordonata
Vehicul* inserareOrdonata(Vehicul* head, Vehicul* newVehicul) {
    if (!head || newVehicul->kilometraj < head->kilometraj) {
        newVehicul->next = head;
        return newVehicul;
    }

    Vehicul* current = head;
    while (current->next && current->next->kilometraj < newVehicul->kilometraj) {
        current = current->next;
    }

    newVehicul->next = current->next;
    current->next = newVehicul;

    cache.cache_valid = 0;  // Resetare cache
    return head;
}

// Afisare vehicul
void afiseazaVehicul(Vehicul* v) {
    if (v) {
        printf("Cod: %d, Marca: %s, An: %d, Km: %.2f, Tip: %c, Pret: %.2f\n",
            v->cod_unic, v->marca, v->an_fabricatie, v->kilometraj, v->tip_vehicul, v->pret_achizitie);
    }
}

// Afisare lista vehicule
void afiseazaLista(Vehicul* head) {
    while (head) {
        afiseazaVehicul(head);
        head = head->next;
    }
}

// Calcul valoare totala flota (folosind cache)
double valoareTotalaFlota(Vehicul* head) {
    if (cache.cache_valid) {
        return cache.valoare_totala_flota;
    }

    double total = 0;
    Vehicul* temp = head;
    while (temp) {
        total += temp->pret_achizitie;
        temp = temp->next;
    }

    cache.valoare_totala_flota = total;
    cache.cache_valid = 1;

    logEvent("Calcul valoare totală flotă.");
    return total;
}

// Verificare memorie neeliberata
void verificareMemorie(Vehicul* head) {
    if (head) {
        printf("Memorie neeliberată detectată!\n");
        logEvent("Eroare: Memorie neeliberată detectată!");
    }
    else {
        printf("Toată memoria a fost eliberată corect.\n");
    }
}

// Eliberare memorie lista
void elibereazaLista(Vehicul** head) {
    while (*head) {
        Vehicul* temp = *head;
        *head = (*head)->next;
        free(temp->marca);
        free(temp);
    }
    cache.cache_valid = 0;
    logEvent("Memorie eliberată pentru flotă.");
}

// Funcția main
int main() {
    logEvent("Programul a început execuția.");

    Vehicul* lista = NULL;
    lista = inserareOrdonata(lista, initVehicul(1, "Toyota", 2015, 120000, 'A', 15000.50));
    lista = inserareOrdonata(lista, initVehicul(2, "Volvo", 2010, 250000, 'C', 30000.75));
    lista = inserareOrdonata(lista, initVehicul(3, "Scania", 2018, 100000, 'T', 85000.00));
    lista = inserareOrdonata(lista, initVehicul(4, "Ford", 2020, 50000, 'A', 20000.00));

    printf("\nLista vehicule:\n");
    afiseazaLista(lista);

    // Calcul valoare totală flota (foloseste cache)
    double valoare = valoareTotalaFlota(lista);
    printf("\nValoare totală flotă: %.2f\n", valoare);

    // Verificare memorie neeliberata
    verificareMemorie(lista);

    // Eliberare memorie
    elibereazaLista(&lista);

    // Verificare dupa eliberare
    verificareMemorie(lista);

    logEvent("Programul s-a încheiat corect.");
    return 0;
}



