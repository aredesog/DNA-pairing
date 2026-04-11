/* Preencher os campos abaixo
Participantes do grupo
Nome: Marcos Felipe dos Reis Bento          Matrícula: 2025.1.08.018
Nome: Guilherme de Oliveira Aredes          Matrícula: 2025.1.08.026
Nome:                                       Matrícula:
Nome:                                       Matrícula:
Nome:                                       Matrícula:
*/

#include "algoritmos.h"
#include <stdlib.h>  // Para usar malloc e free (alocação dinâmica)
#include <string.h>  // Para usar strlen (tamanho da string)

#define MATCH 2  // Se for par complementar (A-T, C-G)
#define MISMACH -1  // Penalidade para bases que não se combinam
#define GAP -2  // Penalidade para a inserção de um espaço (buraco) no alinhamento

/**
 * Retorna o maior valor entre três inteiros.
 * Essencial para decidir qual caminho (diagonal, cima ou esquerda) 
 * oferece a melhor pontuação na matriz.
 */
int max(int a, int b, int c){ 
    int max = a;
    if(b > max){
        max = b;
    }
    if(c > max){
        max = c;
    }
    return max;
}

/**
 * Compara dois caracteres e verifica se são pares complementares de DNA.
 * Se forem complementares, retorna o bônus de MATCH, senão, MISMATCH.
 */
int pontos(char a, char b){
    if((a == 'A' && b == 'T') ||
       (a == 'T' && b == 'A') ||
       (a == 'C' && b == 'G') ||
       (a == 'G' && b == 'C')){
        return MATCH;
    }
    return MISMACH;
}

/**
 * Algoritmo de Programação Dinâmica
 * Calcula a pontuação máxima de alinhamento entre duas sequências.
 */
long int programacao_dinamica(char *s1, char *s2) {
    long int pontuacao = 0;

    int m = strlen(s1);  // Tamanho da primeira string
    int n = strlen(s2);  // Tamanho da segunda string
    
     //Aloca um array de ponteiros (as linhas)
    int **matriz = (int**)malloc((m + 1) * sizeof(int *));
    
    for(int i = 0; i <= m; i++){
        // Aloca cada linha da matriz
        matriz[i] = (int *)malloc((n + 1) * sizeof(int));
    }

    // Inicialização da primeira coluna (alinhando s1 com GAPs)
    for(int i = 0; i <= m; i++){
        matriz[i][0] = i * GAP;
    }
    // Inicialização da primeira linha (alinhando s2 com GAPs)
    for(int j = 0; j <= n; j++){
        matriz[0][j] = j * GAP;
    } 
    
    // Preenchimento da matriz comparando caractere por caractere
    for(int i = 1; i <= m; i++){
        for(int j = 1; j <= n; j++){
            // Opção 1: Vir da diagonal (combinação/alinhamento das bases atuais)
            int diagonal = matriz[i - 1][j - 1] + pontos(s1[i - 1], s2[j - 1]);
            
            // Opção 2: Vir de cima (insere um GAP na sequência s2)
            int em_cima = matriz[i - 1][j] + GAP;
            
            // Opção 3: Vir da esquerda (insere um GAP na sequência s1)
            int esquerda = matriz[i][j - 1] + GAP;
            
            // A célula atual recebe o melhor resultado possível entre as 3 direções
            matriz[i][j] = max(diagonal, em_cima, esquerda);
        }
    }
    
    // O resultado final do alinhamento global está no último quadrante da matriz
    pontuacao = matriz[m][n];

    // Libera memória alocada
    for (int i = 0; i <= m; i++) {
        free(matriz[i]);
    }
    free(matriz);


    return pontuacao;
}


/**
 * Algoritmo Guloso (Greedy)
 * Realiza o alinhamento de forma gulosa, sempre escolhendo a melhor opção local.
 * A cada passo, compara: alinhamento de bases, gap em s1 ou gap em s2.
 */
long int guloso(char *s1, char *s2) {
    long int pontuacao = 0;
    
    int m = strlen(s1);  // Tamanho da primeira string
    int n = strlen(s2);  // Tamanho da segunda string
    
    int i = 0, j = 0;  // Índices para percorrer as strings
    
    // Percorre ambas as strings de forma simultânea
    while (i < m && j < n) {
        // Opção 1: Alinhar as bases atuais (pode ser match ou mismatch)
        int alinhamento = pontos(s1[i], s2[j]);
        
        // Opção 2: Inserir gap em s1 (pular um caractere em s1)
        int gap_s1 = GAP;
        
        // Opção 3: Inserir gap em s2 (pular um caractere em s2)
        int gap_s2 = GAP;
        
        // Escolhe a opção que oferece o maior ganho imediato (guloso)
        if (alinhamento >= gap_s1 && alinhamento >= gap_s2) {
            // Melhor alinhar as bases atuais
            pontuacao += alinhamento;
            i++;
            j++;
        } else if (gap_s1 >= gap_s2) {
            // Melhor inserir gap em s1
            pontuacao += gap_s1;
            i++;
        } else {
            // Melhor inserir gap em s2
            pontuacao += gap_s2;
            j++;
        }
    }
    
    // Processa caracteres restantes da string s1 com gaps
    while (i < m) {
        pontuacao += GAP;
        i++;
    }
    
    // Processa caracteres restantes da string s2 com gaps
    while (j < n) {
        pontuacao += GAP;
        j++;
    }

    /* Esta retorno também é obrigatório e não deve ser retirado*/
    return pontuacao;
}