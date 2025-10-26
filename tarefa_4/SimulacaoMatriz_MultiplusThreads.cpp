#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LARGURA 4000
#define ALTURA 4000
#define NUM_THREADS 4

unsigned char imagem[ALTURA][LARGURA]; // imagem simulada

void* inverter_parte(void* arg) {
    int id = (int)(size_t)arg;
    int inicio = id * (ALTURA / NUM_THREADS);
    int fim = (id + 1) * (ALTURA / NUM_THREADS);

    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < LARGURA; j++) {
            imagem[i][j] = 255 - imagem[i][j]; // inverte pixel
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // inicializa a "imagem"
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            imagem[i][j] = rand() % 256;
        }
    }

    // cria threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, inverter_parte, (void*)(size_t)i);
    }

    // espera threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Imagem processada com sucesso!\n");
    return 0;
}