#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define N 100

int vetor[N];
int resultado = 0;
pthread_mutex_t lock;

void* soma_vetor(void* arg) {
    int id = (int)(size_t)arg;
    int inicio = id * (N / NUM_THREADS);
    int fim = (id + 1) * (N / NUM_THREADS);
    int soma = 0;

    for (int i = inicio; i < fim; i++) {
        soma += vetor[i];
    }

    // Atualiza o resultado global com mutex
    pthread_mutex_lock(&lock);
    resultado += soma;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    // inicializa vetor
    for (int i = 0; i < N; i++) {
        vetor[i] = 1;
    }

    // cria threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, soma_vetor, (void*)(size_t)i);
    }

    // espera threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Soma total (com threads) = %d\n", resultado);

    // versão single-thread (mais rápida nesse caso)
    int soma = 0;
    for (int i = 0; i < N; i++) soma += vetor[i];
    printf("Soma total (single-thread) = %d\n", soma);

    pthread_mutex_destroy(&lock);
    return 0;
}