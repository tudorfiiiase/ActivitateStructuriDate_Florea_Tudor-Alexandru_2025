#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

// Structura Student
typedef struct Student {
    int nrMatricol;
    int varsta;
    char* nume;
    float medie;
    char initialaFacultate;
    struct Student* next;
} Student;

// Funcție de validare a datelor
int validareStudent(int varsta, float medie, char initialaFacultate) {
    return (varsta >= 18 && medie >= 1.0 && medie <= 10.0 &&
        (initialaFacultate >= 'A' && initialaFacultate <= 'Z'));
}

// Funcție de creare a unui nou student
Student* creareStudent(int nrMatricol, int varsta, const char* nume, float medie, char initialaFacultate) {
    if (!validareStudent(varsta, medie, initialaFacultate)) {
        printf("Date invalide pentru studentul %s!\n", nume);
        return NULL;
    }

    Student* nou = (Student*)malloc(sizeof(Student));
    if (!nou) {
        printf("Eroare alocare memorie!\n");
        return NULL;
    }

    nou->nrMatricol = nrMatricol;
    nou->varsta = varsta;
    nou->medie = medie;
    nou->initialaFacultate = initialaFacultate;

    nou->nume = (char*)malloc(strlen(nume) + 1);
    if (!nou->nume) {
        printf("Eroare alocare nume!\n");
        free(nou);
        return NULL;
    }
    strcpy_s(nou->nume, strlen(nume)+1, nume);

    nou->next = NULL;
    return nou;
}

// Functie de inserare la sfarsitul listei
void inserareSfarsit(Student** head, int nrMatricol, int varsta, const char* nume, float medie, char initialaFacultate) {
    Student* nou = creareStudent(nrMatricol, varsta, nume, medie, initialaFacultate);
    if (!nou) return;

    if (*head == NULL) {
        *head = nou;
    }
    else {
        Student* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nou;
    }
}

// Functie pentru citirea studentilor dintr-un fișier CSV
Student* citireDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) {
        printf("Eroare la deschiderea fișierului!\n");
        return NULL;
    }

    Student* head = NULL;
    char linie[256];

    while (fgets(linie, sizeof(linie), file)) {
        int nrMatricol, varsta;
        char nume[100];
        float medie;
        char initialaFacultate;

        if (scanf_s(linie, "%d,%d,%99[^,],%f,%c", &nrMatricol, &varsta, nume, &medie, &initialaFacultate) == 5) {
            inserareSfarsit(&head, nrMatricol, varsta, nume, medie, initialaFacultate);
        }
    }

    fclose(file);
    printf("Lista a fost încărcată din fișierul %s cu succes!\n", numeFisier);
    return head;
}

// Functie de afisare a listei
void afisareLista(Student* head) {
    if (!head) {
        printf("Lista este goală!\n");
        return;
    }

    Student* temp = head;
    while (temp != NULL) {
        printf("Nr. Matricol: %d | Nume: %s | Varsta: %d | Medie: %.2f | Facultate: %c\n",
            temp->nrMatricol, temp->nume, temp->varsta, temp->medie, temp->initialaFacultate);
        temp = temp->next;
    }
}

// Functie pentru salvarea listei într-un fisier CSV
void salvareInFisier(Student* head, const char* numeFisier) {
    FILE* file = fopen(numeFisier, "w");
    if (!file) {
        printf("Eroare la deschiderea fișierului pentru scriere!\n");
        return;
    }

    Student* temp = head;
    while (temp != NULL) {
        fprintf(file, "%d,%d,%s,%.2f,%c\n", temp->nrMatricol, temp->varsta, temp->nume, temp->medie, temp->initialaFacultate);
        temp = temp->next;
    }

    fclose(file);
    printf("Lista a fost salvată în fișierul %s!\n", numeFisier);
}

// Functie pentru stergerea completă a listei
void stergereLista(Student** head) {
    Student* temp = *head;
    while (temp != NULL) {
        Student* next = temp->next;
        free(temp->nume);
        free(temp);
        temp = next;
    }
    *head = NULL;
    printf("Lista a fost ștearsă complet!\n");
}

// Functia main
int main() {
    Student* lista = citireDinFisier("studenti.csv");

    // Afișare lista inițială
    printf("\nLista inițială de studenți:\n");
    afisareLista(lista);

    // Adăugare manuală a unui student nou
    inserareSfarsit(&lista, 104, 23, "Victor Stan", 8.7, 'E');
    printf("\nLista după inserarea unui student nou:\n");
    afisareLista(lista);

    // Salvare lista în fișier
    salvareInFisier(lista, "studenti_output.csv");

    // Ștergere completă a listei
    stergereLista(&lista);

    return 0;
}












