#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRT_SECURE_NO_WARNINGS
#define MAX_WORDS 1000000
#define MAX_LEN 32

typedef struct {
    char word[MAX_LEN];
    int freq;
} WordFreq;
int cmp(const void* a, const void* b) {
    WordFreq* wa = (WordFreq*)a;
    WordFreq* wb = (WordFreq*)b;

    if (wa->freq != wb->freq)
        return wb->freq - wa->freq; // descrescator după frecventa
    return strcmp(wa->word, wb->word); // lexicografic crescator
}
int main() {
    int n, k;
    scanf_s("%d %d", &n, &k);

    WordFreq dict[MAX_WORDS];
    int dictSize = 0;

    char word[MAX_LEN];
    for (int i = 0; i < n; i++) {
        scanf_s("%s", word);

        // Cauta daca exista deja
        int found = 0;
        for (int j = 0; j < dictSize; j++) {
            if (strcmp(dict[j].word, word) == 0) {
                dict[j].freq++;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy_s(dict[dictSize].word, strlen(word)+1, word);
            dict[dictSize].freq = 1;
            dictSize++;
        }
    }

    // Sorteaza dupa frecventa si apoi lexicografic
    qsort(dict, dictSize, sizeof(WordFreq), cmp);

    for (int i = 0; i < k && i < dictSize; i++) {
        printf("%s %d\n", dict[i].word, dict[i].freq);
    }

    return 0;
}






