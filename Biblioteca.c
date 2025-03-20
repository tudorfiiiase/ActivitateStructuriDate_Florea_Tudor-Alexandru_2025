#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct {
    int cod;
    char* titlu;
    char* autor;
    int an_publicare;
    float pret;
    char disponibil; // 'D' - disponibil, 'I' - imprumutat
} Carte;

// Functie de initializare a unei carti
Carte* initCarte(int cod, const char* titlu, const char* autor, int an_publicare, float pret, char disponibil) {
    Carte* c = (Carte*)malloc(sizeof(Carte));
    if (!c) {
        printf("Eroare la alocarea memoriei!\n");
        return NULL;
    }

    c->titlu = (char*)malloc(strlen(titlu) + 1);
    c->autor = (char*)malloc(strlen(autor) + 1);
    if (!c->titlu || !c->autor) {
        printf("Eroare la alocarea memoriei pentru titlu sau autor!\n");
        free(c->titlu);
        free(c->autor);
        free(c);
        return NULL;
    }

    strcpy_s(c->titlu, strlen(titlu) + 1, titlu);
    strcpy_s(c->autor, strlen(autor) + 1, autor);
    c->cod = cod;
    c->an_publicare = an_publicare;
    c->pret = pret;
    c->disponibil = disponibil;

    return c;
}

// Functie pentru afisarea unei carti
void afiseazaCarte(const Carte* c) {
    if (c) {
        printf("Cod: %d | Titlu: %s | Autor: %s | An: %d | Pret: %.2f RON | Disponibilitate: %c\n",
            c->cod, c->titlu, c->autor, c->an_publicare, c->pret, c->disponibil);
    }
}

// Functie pentru afisarea unui vector de carti
void afiseazaVectorCarti(Carte** carti, int nr_carti) {
    printf("Lista cărților:\n");
    for (int i = 0; i < nr_carti; i++) {
        afiseazaCarte(carti[i]);
    }
}

// Functie care calculeaza pretul mediu al cartilor
float calculeazaPretMediu(Carte** carti, int nr_carti) {
    if (nr_carti == 0) return 0;
    float suma = 0;
    for (int i = 0; i < nr_carti; i++) {
        suma += carti[i]->pret;
    }
    return suma / nr_carti;
}

// Functie pentru copierea primelor N carti intr-un nou vector
Carte** copiazaPrimeleNCarti(Carte** carti, int nr_carti, int N, int* nr_carti_noi) {
    *nr_carti_noi = (N > nr_carti) ? nr_carti : N;
    Carte** copie = (Carte**)malloc((*nr_carti_noi) * sizeof(Carte*));
    for (int i = 0; i < *nr_carti_noi; i++) {
        copie[i] = initCarte(carti[i]->cod, carti[i]->titlu, carti[i]->autor, carti[i]->an_publicare, carti[i]->pret, carti[i]->disponibil);
    }
    return copie;
}

// Functie care copiaza cartile publicate dupa un anumit an
Carte** copieCartiDupaAn(Carte** carti, int nr_carti, int an, int* nr_carti_noi) {
    *nr_carti_noi = 0;
    for (int i = 0; i < nr_carti; i++) {
        if (carti[i]->an_publicare > an) {
            (*nr_carti_noi)++;
        }
    }
    if (*nr_carti_noi == 0) return NULL;

    Carte** cartiNoi = (Carte**)malloc((*nr_carti_noi) * sizeof(Carte*));
    int index = 0;
    for (int i = 0; i < nr_carti; i++) {
        if (carti[i]->an_publicare > an) {
            cartiNoi[index++] = initCarte(carti[i]->cod, carti[i]->titlu, carti[i]->autor, carti[i]->an_publicare, carti[i]->pret, carti[i]->disponibil);
        }
    }
    return cartiNoi;
}

// Functie pentru cautarea unei carti după titlu
Carte* cautaCarteDupaTitlu(Carte** carti, int nr_carti, const char* titlu_cautat) {
    for (int i = 0; i < nr_carti; i++) {
        if (strcmp(carti[i]->titlu, titlu_cautat) == 0) {
            return carti[i];
        }
    }
    return NULL;
}

// Functie pentru eliberarea memoriei unei carti
void elibereazaCarte(Carte* c) {
    if (c) {
        free(c->titlu);
        free(c->autor);
        free(c);
    }
}

// Functie pentru eliberarea memoriei unui vector de carti
void elibereazaVectorCarti(Carte** carti, int nr_carti) {
    for (int i = 0; i < nr_carti; i++) {
        elibereazaCarte(carti[i]);
    }
    free(carti);
}

// Functia principala
int main() {
    // Initializare carti
    int nr_carti = 5;
    Carte** carti = (Carte**)malloc(nr_carti * sizeof(Carte*));

    carti[0] = initCarte(1, "1984", "George Orwell", 1949, 45.50, 'D');
    carti[1] = initCarte(2, "Mândrie și prejudecată", "Jane Austen", 1813, 39.99, 'I');
    carti[2] = initCarte(3, "Harry Potter", "J.K. Rowling", 1997, 55.75, 'D');
    carti[3] = initCarte(4, "Dune", "Frank Herbert", 1965, 70.00, 'D');
    carti[4] = initCarte(5, "Frankenstein", "Mary Shelley", 1818, 30.00, 'I');

    // Afișarea cartilor
    afiseazaVectorCarti(carti, nr_carti);

    // Calcul si afisare pret mediu
    float pret_mediu = calculeazaPretMediu(carti, nr_carti);
    printf("Prețul mediu al cărților: %.2f RON\n", pret_mediu);

    // Copierea primelor N carti
    int N = 3, nr_carti_copiate;
    Carte** carti_copiate = copiazaPrimeleNCarti(carti, nr_carti, N, &nr_carti_copiate);
    printf("Primele %d cărți copiate:\n", N);
    afiseazaVectorCarti(carti_copiate, nr_carti_copiate);

    // Copierea cartilor publicate după 1950
    int nr_carti_dupa_1950;
    Carte** carti_dupa_1950 = copieCartiDupaAn(carti, nr_carti, 1950, &nr_carti_dupa_1950);
    if (carti_dupa_1950) {
        printf("Cărți publicate după 1950:\n");
        afiseazaVectorCarti(carti_dupa_1950, nr_carti_dupa_1950);
        elibereazaVectorCarti(carti_dupa_1950, nr_carti_dupa_1950);
    }

    // Cautarea unei carti după titlu
    Carte* carte_gasita = cautaCarteDupaTitlu(carti, nr_carti, "Dune");
    if (carte_gasita) {
        printf("Cartea găsită:\n");
        afiseazaCarte(carte_gasita);
    }

    // Dezalocarea memoriei
    elibereazaVectorCarti(carti, nr_carti);
    elibereazaVectorCarti(carti_copiate, nr_carti_copiate);

    return 0;
}




