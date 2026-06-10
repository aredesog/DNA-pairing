#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <stddef.h>

// Funções principais
long int programacao_dinamica(char *s1, char *s2);
long int guloso(char *s1, char *s2);

// Métricas de memória
extern size_t memoria_programacao_dinamica;
extern size_t memoria_guloso;

// Funções auxiliares
int max(int a, int b, int c);
int pontos(char a, char b);

#endif // ALGORITMOS_H
