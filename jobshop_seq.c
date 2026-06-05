#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <omp.h>
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

int main(int argc, char *argv[]) {
  if (argc < 3) {
        printf("Uso: %s <entrada.jss> <saida.output>\n", argv[0]);
        return 1;
    }

    const char *ficheiro_entrada = argv[1];
    const char *ficheiro_saida = argv[2];
    
    int num_repeticoes = 10; // Exigência da Parte C do enunciado
    double tempo_total_acumulado = 0.0;
    int tempo_max = 0;
    JobShop meu_jobshop;

    printf("A executar a versao sequencial (%d vezes para tirar a media)...\n", num_repeticoes);

    // Ciclo de medição de desempenho limpo
    for (int i = 0; i < num_repeticoes; i++) {
        ler_ficheiro(ficheiro_entrada, &meu_jobshop);

        //inicio da medição
        double tempo_inicio = omp_get_wtime();

        tempo_max = calcular_escalonamento(&meu_jobshop);

        //Fim da medição
        double tempo_fim = omp_get_wtime();
        
        tempo_total_acumulado += (tempo_fim - tempo_inicio);
    }

    double tempo_medio_seq = tempo_total_acumulado / num_repeticoes;

    printf("\nRESULTADOS SEQUENCIAL");
    printf("Tempo Maximo (Makespan): %d\n", tempo_max);
    printf("Tempo Medio de Execucao: %f segundos\n", tempo_medio_seq);

    gravar_resultado(ficheiro_saida, &meu_jobshop, tempo_max);
    return 0;
}