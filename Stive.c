#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define CRT_SECURE_NO_WARNINGS
#define MAX_LEN 1000

typedef struct {
    char data[MAX_LEN];
    int top;
} Stack;

void init(Stack* s) { s->top = -1; }
int isEmpty(Stack* s) { return s->top == -1; }
void push(Stack* s, char c) { s->data[++s->top] = c; }
char pop(Stack* s) { return s->data[s->top--]; }
char peek(Stack* s) { return s->data[s->top]; }

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int isValidExpression(const char* expr) {
    Stack stiva;
    init(&stiva);

    char prev = '\0';  // tipul precedent: 'o' = operand, 'p' = paranteză, '+'
    int i = 0;
    while (expr[i]) {
        char c = expr[i];

        if (isspace(c)) {
            i++;
            continue;
        }

        if (isdigit(c)) {
            // operand valid
            if (prev == 'o') {
                // două operanzi consecutivi fără operator (ex: 23 45)
                return 0;
            }
            prev = 'o';
            while (isdigit(expr[i])) i++; // salt peste numărul complet
            continue;
        }

        if (c == '(') {
            if (prev == 'o') return 0; // ex: 3(4+5)
            push(&stiva, c);
            prev = '(';
        }
        else if (c == ')') {
            if (isEmpty(&stiva)) return 0; // închidere fără deschidere
            if (prev == '(' || isOperator(prev)) return 0; // ex: () sau +)
            pop(&stiva);
            prev = ')';
        }
        else if (isOperator(c)) {
            if (prev == '\0' || isOperator(prev) || prev == '(') return 0; // ex: +3, *+, (+3
            prev = c;
        }
        else {
            return 0; // caracter necunoscut
        }

        i++;
    }

    // nu trebuie să se termine cu operator
    if (isOperator(prev) || prev == '(') return 0;

    return isEmpty(&stiva); // valid doar dacă toate parantezele au fost închise
}
int main() {
    const char* expresii[] = {
        "(3 + 4) * (5 - 2)",    // valid
        "3 + * 4",             // invalid
        "(3 + 4))",            // invalid
        "(*3 + 4)",            // invalid
        "3 +",                 // invalid
        "((2 + 3) * 5)",       // valid
        "2 + (3 * (4 - 1))",   // valid
        "(3 + (4 - 5)",        // invalid
        NULL
    };

    for (int i = 0; expresii[i]; i++) {
        printf("\"%s\" -> %s\n", expresii[i],
            isValidExpression(expresii[i]) ? "VALIDĂ" : "INVALIDĂ");
    }

    return 0;
}









