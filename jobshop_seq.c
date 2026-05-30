#include <stdio.h>
#include <stdlib.h>

#define MAX_JOBS 100      // Limite arrays
#define MAX_MACHINES 100

typedef struct {
    int id_maquina;      
    int tempo_duracao;   
    int tempo_inicio;    
} Operacao;

typedef struct {
    int id_job;
    Operacao operacoes[MAX_MACHINES]; // Como cada job passa por todas as máquinas 1 vez
} Trabalho;

typedef struct {
    int num_jobs;
    int num_maquinas;
    Trabalho trabalhos[MAX_JOBS];
} JobShop;