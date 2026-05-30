#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"

//para executar tem q ter 2 comandos
// gcc -fopenmp jobshop_omp.c -o jobshop_omp.exe
// .\jobshop_omp.exe gg03.jss gg03_paralelo.output 4


void ler_ficheiro(const char *nome_ficheiro, JobShop *js) {
    FILE *file = fopen(nome_ficheiro, "r");
    fscanf(file, "%d %d", &js->num_jobs, &js->num_maquinas);
    for (int i = 0; i < js->num_jobs; i++) {
        js->trabalhos[i].id_job = i;
        for (int j = 0; j < js->num_maquinas; j++) {
            fscanf(file, "%d %d", &js->trabalhos[i].operacoes[j].id_maquina, 
                                  &js->trabalhos[i].operacoes[j].tempo_duracao);
            js->trabalhos[i].operacoes[j].tempo_inicio = -1;
        }
    }
    fclose(file);
}

void gravar_resultado(const char *nome_ficheiro, JobShop *js, int makespan) {
    FILE *file = fopen(nome_ficheiro, "w");
    if (file == NULL) {
        printf("Erro ao criar ficheiro de saida %s\n", nome_ficheiro);
        exit(1);
    }
    fprintf(file, "%d\n", makespan);
    for (int i = 0; i < js->num_jobs; i++) {
        for (int j = 0; j < js->num_maquinas; j++) {
            fprintf(file, "%d ", js->trabalhos[i].operacoes[j].tempo_inicio);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int calcular_escalonamento_paralelo(JobShop *js, int num_threads) {
    int makespan_global = 0;
    int tempo_livre_maquina[MAX_MACHINES] = {0}; 
    omp_lock_t locks_maquinas[MAX_MACHINES];

    for (int i = 0; i < js->num_maquinas; i++) {
        omp_init_lock(&locks_maquinas[i]);
    }

    omp_set_num_threads(num_threads);

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < js->num_jobs; i++) {
        int tempo_atual_job = 0; 

        for (int j = 0; j < js->num_maquinas; j++) {
            int id_maq = js->trabalhos[i].operacoes[j].id_maquina;
            int duracao = js->trabalhos[i].operacoes[j].tempo_duracao;

            omp_set_lock(&locks_maquinas[id_maq]);

            int inicio_real;
            if (tempo_atual_job > tempo_livre_maquina[id_maq]) {
                inicio_real = tempo_atual_job;
            } else {
                inicio_real = tempo_livre_maquina[id_maq];
            }

            js->trabalhos[i].operacoes[j].tempo_inicio = inicio_real;
            tempo_livre_maquina[id_maq] = inicio_real + duracao;

            omp_unset_lock(&locks_maquinas[id_maq]);
            tempo_atual_job = inicio_real + duracao;
        }

        #pragma omp critical
        {
            if (tempo_atual_job > makespan_global) {
                makespan_global = tempo_atual_job;
            }
        }
    }

    for (int i = 0; i < js->num_maquinas; i++) {
        omp_destroy_lock(&locks_maquinas[i]);
    }

    return makespan_global;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <entrada.jss> <saida.output> <numero_threads>\n", argv[0]);
        return 1;
    }

    JobShop meu_jobshop;
    int num_threads = atoi(argv[3]);

    printf("A ler o ficheiro %s...\n", argv[1]);
    ler_ficheiro(argv[1], &meu_jobshop);

    printf("A iniciar escalonamento com %d threads...\n", num_threads);
    int tempo_max = calcular_escalonamento_paralelo(&meu_jobshop, num_threads);
    
    printf("Tempo Maximo: %d\n", tempo_max);

    gravar_resultado(argv[2], &meu_jobshop, tempo_max);

    return 0;
}