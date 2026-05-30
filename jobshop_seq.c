#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

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

int calcular_escalonamento(JobShop *js) {
    int tempo_global = 0;

    for (int i = 0; i < js->num_jobs; i++) {
        for (int j = 0; j < js->num_maquinas; j++) {
            js->trabalhos[i].operacoes[j].tempo_inicio = tempo_global;
            tempo_global += js->trabalhos[i].operacoes[j].tempo_duracao;
        }
    }
    return tempo_global;
}

int main() {
    
    JobShop meu_jobshop;
    
    ler_ficheiro("gg03.jss", &meu_jobshop);
    int tempo_max = calcular_escalonamento(&meu_jobshop);
    printf("Tempo Maximo: %d\n", tempo_max);

    return 0;
}