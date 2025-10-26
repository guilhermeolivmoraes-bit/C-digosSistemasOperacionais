#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estrutura para passar os dados de entrada para os threads
typedef struct {
    int *numeros;
    int tamanho;
} DadosThread;

// Função para calcular a média
// Retorna um ponteiro para um double alocado dinamicamente
void* calcular_media(void* arg) {
    DadosThread* dados = (DadosThread*) arg;
    int soma = 0;
    for (int i = 0; i < dados->tamanho; i++) {
        soma += dados->numeros[i];
    }

    // Aloca memória para o resultado, pois não podemos retornar o endereço de uma variável local
    double* resultado = malloc(sizeof(double));
    *resultado = (double)soma / dados->tamanho;

    pthread_exit(resultado);
}

// Função para calcular o mínimo
// Retorna um ponteiro para um int alocado dinamicamente
void* calcular_minimo(void* arg) {
    DadosThread* dados = (DadosThread*) arg;
    int valor_minimo = dados->numeros[0];
    for (int i = 1; i < dados->tamanho; i++) {
        if (dados->numeros[i] < valor_minimo) {
            valor_minimo = dados->numeros[i];
        }
    }

    int* resultado = malloc(sizeof(int));
    *resultado = valor_minimo;

    pthread_exit(resultado);
}

// Função para calcular o máximo
// Retorna um ponteiro para um int alocado dinamicamente
void* calcular_maximo(void* arg) {
    DadosThread* dados = (DadosThread*) arg;
    int valor_maximo = dados->numeros[0];
    for (int i = 1; i < dados->tamanho; i++) {
        if (dados->numeros[i] > valor_maximo) {
            valor_maximo = dados->numeros[i];
        }
    }
    
    int* resultado = malloc(sizeof(int));
    *resultado = valor_maximo;
    
    pthread_exit(resultado);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s num1 num2 num3 ...\n", argv[0]);
        return 1;
    }

    int tamanho = argc - 1;
    // Adicionamos o (int *) para ser compatível com C++
    int *numeros = (int *)malloc(tamanho * sizeof(int));
    if (numeros == NULL) {
        printf("Falha ao alocar memória.\n");
        return 1;
    }

    for (int i = 0; i < tamanho; i++) {
        numeros[i] = atoi(argv[i + 1]);
    }

    DadosThread dados;
    dados.numeros = numeros;
    dados.tamanho = tamanho;

    pthread_t tid_media, tid_minimo, tid_maximo;

    // Criação das threads
    pthread_create(&tid_media, NULL, calcular_media, (void*)&dados);
    pthread_create(&tid_minimo, NULL, calcular_minimo, (void*)&dados);
    pthread_create(&tid_maximo, NULL, calcular_maximo, (void*)&dados);

    // Variáveis para receber os ponteiros de retorno das threads
    void* resultado_media;
    void* resultado_minimo;
    void* resultado_maximo;

    // Espera pelas threads terminarem e coleta os resultados
    pthread_join(tid_media, &resultado_media);
    pthread_join(tid_minimo, &resultado_minimo);
    pthread_join(tid_maximo, &resultado_maximo);

    // Converte os ponteiros void* para o tipo correto e acessa o valor
    double media = *(double*)resultado_media;
    int minimo = *(int*)resultado_minimo;
    int maximo = *(int*)resultado_maximo;

    // Exibe os resultados
    printf("O valor medio e %.2f\n", media);
    printf("O valor minimo e %d\n", minimo);
    printf("O valor maximo e %d\n", maximo);

    // Libera a memória alocada
    free(numeros);
    free(resultado_media);
    free(resultado_minimo);
    free(resultado_maximo);

    return 0;
}