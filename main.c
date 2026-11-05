// 1. Carregue os dados do arquivo para o vetor normalmente...
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

static int *vetor_auxiliar;

// Merge Sort: junta duas partes já ordenadas.
static void intercalar(int vetor[], size_t inicio, size_t meio, size_t fim)
{
    size_t esquerda = inicio;
    size_t direita = meio;
    size_t posicao = inicio;

    while (esquerda < meio && direita < fim) {
        if (vetor[esquerda] <= vetor[direita]) {
            vetor_auxiliar[posicao++] = vetor[esquerda++];
        } else {
            vetor_auxiliar[posicao++] = vetor[direita++];
        }
    }

    while (esquerda < meio) {
        vetor_auxiliar[posicao++] = vetor[esquerda++];
    }
    while (direita < fim) {
        vetor_auxiliar[posicao++] = vetor[direita++];
    }

    for (posicao = inicio; posicao < fim; posicao++) {
        vetor[posicao] = vetor_auxiliar[posicao];
    }
}

static void mergeSort(int vetor[], size_t inicio, size_t fim)
{
    if (fim - inicio < 2) {
        return;
    }

    size_t meio = inicio + (fim - inicio) / 2;
    mergeSort(vetor, inicio, meio);
    mergeSort(vetor, meio, fim);
    intercalar(vetor, inicio, meio, fim);
}

// Confere se o vetor está em ordem crescente ou com valores repetidos.
static int estaOrdenado(int vetor[], size_t total_elementos)
{
    for (size_t i = 1; i < total_elementos; i++) {
        if (vetor[i - 1] > vetor[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    FILE *arquivo = fopen("listaOdenada.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir listaOdenada.txt\n");
        return EXIT_FAILURE;
    }

    // Conta os números para saber o tamanho do vetor.
    size_t total_elementos = 0;
    int valor;
    int resultado;
    while ((resultado = fscanf(arquivo, "%d", &valor)) == 1) {
        total_elementos++;
    }

    if (resultado != EOF) {
        printf("Erro: o arquivo deve conter somente números inteiros.\n");
        fclose(arquivo);
        return EXIT_FAILURE;
    }

    rewind(arquivo);

    int *vetor = malloc(total_elementos * sizeof(int));
    vetor_auxiliar = malloc(total_elementos * sizeof(int));

    if (total_elementos > 0 &&
        (vetor == NULL || vetor_auxiliar == NULL)) {
        printf("Erro ao reservar memória.\n");
        free(vetor);
        free(vetor_auxiliar);
        fclose(arquivo);
        return EXIT_FAILURE;
    }

    // Carrega os números antes de iniciar as medições.
    for (size_t i = 0; i < total_elementos; i++) {
        if (fscanf(arquivo, "%d", &vetor[i]) != 1) {
            printf("Erro ao ler os números do arquivo.\n");
            free(vetor);
            free(vetor_auxiliar);
            fclose(arquivo);
            return EXIT_FAILURE;
        }
    }
    fclose(arquivo);

    // 2. Dispare o cronômetro EXATAMENTE antes de chamar a ordenação:
    clock_t inicio = clock();

    mergeSort(vetor, 0, total_elementos);

    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (!estaOrdenado(vetor, total_elementos)) {
        printf("Erro ao ordenar os números.\n");
        free(vetor);
        free(vetor_auxiliar);
        return EXIT_FAILURE;
    }

    printf("Tempo de ordenação: %f segundos (%f ms)\n",
           tempo_gasto, tempo_gasto * 1000.0);

    free(vetor);
    free(vetor_auxiliar);
    return EXIT_SUCCESS;
}
