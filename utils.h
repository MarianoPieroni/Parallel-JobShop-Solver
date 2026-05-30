#ifndef UTILS_H
#define UTILS_H

#define MAX_JOBS 100
#define MAX_MACHINES 100

typedef struct {
    int id_maquina;
    int tempo_duracao;
    int tempo_inicio; 
} Operacao;

typedef struct {
    int id_job;
    Operacao operacoes[MAX_MACHINES]; 
} Trabalho;

typedef struct {
    int num_jobs;
    int num_maquinas;
    Trabalho trabalhos[MAX_JOBS];
} JobShop;

void ler_ficheiro(const char *nome_ficheiro, JobShop *js);
int calcular_escalonamento(JobShop *js);

#endif