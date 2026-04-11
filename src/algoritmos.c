/* Preencher os campos abaixo
Participantes do grupo
Nome: Marcos Felipe dos Reis Bento          Matrícula: 2025.1.08.018
Nome: Guilherme de Oliveira Aredes          Matrícula: 2025.1.08.026
Nome: Érik Alexandre Vieira Peres           Matrícula: 2025.1.08.005 
Nome: Diogo Silva Salgado                   Matrícula: 2024.1.08.007
Nome: Carlos Eduardo Pereira dos Santos     Matrícula: 2024.2.08.001
*/

#include "algoritmos.h"
#include <stdlib.h>  // Para usar malloc e free (alocação dinâmica)
#include <string.h>  // Para usar strlen (tamanho da string)

#define MATCH 2  // Se for par complementar (A-T, C-G)
#define MISMACH -1  // Penalidade para bases que não se combinam
#define GAP -2  // Penalidade para a inserção de um espaço (buraco) no alinhamento

size_t memoria_programacao_dinamica = 0;
size_t memoria_guloso = 0;

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

    memoria_programacao_dinamica = (size_t)(m + 1) * sizeof(int *);
    
     //Aloca um array de ponteiros (as linhas)
    int **matriz = (int**)malloc((m + 1) * sizeof(int *));
    
    for(int i = 0; i <= m; i++){
        // Aloca cada linha da matriz
        matriz[i] = (int *)malloc((n + 1) * sizeof(int));
        memoria_programacao_dinamica += (size_t)(n + 1) * sizeof(int);
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
 // Para usar o strlen
long int guloso(char *s1, char *s2) {
    long int pontuacao = 0;
    memoria_guloso = 0;

    int i = 0, j = 0;
    int tam_s1 = strlen(s1);
    int tam_s2 = strlen(s2);

    while (i < tam_s1 && j < tam_s2) {
        // Calcula a pontuação de alinhar s1[i] com s2[j] (Par "pareado", +2)
        int alinhar;
        if ((s1[i] == 'A' && s2[j] == 'T') || (s1[i] == 'T' && s2[j] == 'A') ||
            (s1[i] == 'C' && s2[j] == 'G') || (s1[i] == 'G' && s2[j] == 'C')) {
            alinhar = 2;
        } else {
            //Par errado (-1)
            alinhar = -1;
        }

        int gap_s1 = -2, gap_s2 = -2; // Custo dos gaps

        /* O if abaixo escolhe a opção de maior ganho imediato.
        No caso, "alinhar" sempre é maior que gap_s1 e gap_s2, sendo a melhor solução imediata.
        Pensando pelo lado lógico do problema, a comparação é redundante, pois
        o valor de alinhar (-1 ou 2) sempre será maior que o do gap (-2).
        Ou seja, poderíamos apenas incrementar i e j direto, sem o if.
        Mas é interessante manter a comparação para tornar o entendimento mais claro. */

        if (alinhar >= gap_s1 && alinhar >= gap_s2) { // alinhar é sempre > -2
            // Alinha s1[i] com s2[j] e avança nas duas strings
            pontuacao += alinhar;
            i++; j++;
        }
    }

    /* Redundante para as instâncias de exemplo, onde as duas strings sempre têm o mesmo tamanho.
   Caso contrário, "emparelha" os caracteres restantes da string maior com gaps. */
    while (i < tam_s1) {
        pontuacao += -2;
        i++;
    }

    // Mesma ideia do que foi dito no comentário anterior, mas pra s2.
    while (j < tam_s2) {
        pontuacao += -2;
        j++;
    }

    /* Esta retorno também é obrigatório e não deve ser retirado*/
    return pontuacao;
}