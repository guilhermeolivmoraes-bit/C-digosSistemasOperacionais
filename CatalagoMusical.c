/*
------------------------------------------------------------
            Catálogo Musical - Sistemas Operacionais
------------------------------------------------------------
Disciplina: Sistemas Operacionais
Atividade: Manipulação de Arquivos com System Calls
Tema: Catálogo Musical (Linux / POSIX)
Aluno: Guilherme Oliveira Moraes
Professor: Juvenal Neto
Data: 10/09/2025

Descrição:
Este programa implementa um catálogo musical simples
utilizando chamadas de sistema (System Calls) do Linux
para manipulação de arquivos.

Funcionalidades:
1. Cadastrar músicas (artista, álbum, faixa).
2. Listar músicas cadastradas.

System Calls utilizadas:
- open   -> Cria ou abre um arquivo.
- write  -> Escreve dados no arquivo.
- read   -> Lê dados do arquivo.
- close  -> Fecha o arquivo.

Arquivo de armazenamento: "catalogo.txt"
------------------------------------------------------------
*/

#include <stdio.h>      // Funções de entrada e saída (printf, scanf, fgets, etc.)
#include <stdlib.h>     // Funções utilitárias (exit, etc.)
#include <unistd.h>     // System calls: read, write, close
#include <fcntl.h>      // System calls: open, flags O_CREAT, O_APPEND, O_RDONLY
#include <string.h>     // Manipulação de strings (strlen, strcspn, etc.)

#define ARQUIVO "catalogo.txt"  // Nome do arquivo do catálogo
#define TAM 256                 // Tamanho máximo das strings

// -----------------------------------------------------------
// Função: cadastrarMusica
// Objetivo: Cadastrar uma nova música no catálogo
// -----------------------------------------------------------
void cadastrarMusica() {
    int fd;                        // File descriptor (identificador do arquivo)
    char artista[TAM], album[TAM], faixa[TAM], buffer[TAM * 3];

    // Abre (ou cria) o arquivo em modo escrita + append
    fd = open(ARQUIVO, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    // Entrada de dados do usuário
    printf("Digite o nome do artista: ");
    fgets(artista, TAM, stdin);
    artista[strcspn(artista, "\n")] = 0; // remove '\n'

    printf("Digite o nome do álbum: ");
    fgets(album, TAM, stdin);
    album[strcspn(album, "\n")] = 0;

    printf("Digite o nome da faixa: ");
    fgets(faixa, TAM, stdin);
    faixa[strcspn(faixa, "\n")] = 0;

    // Monta a string formatada
    snprintf(buffer, sizeof(buffer), "Artista: %s | Álbum: %s | Faixa: %s\n", artista, album, faixa);

    // Escreve no arquivo
    if (write(fd, buffer, strlen(buffer)) == -1) {
        perror("Erro ao escrever no arquivo");
    } else {
        printf("Música cadastrada com sucesso!\n");
    }

    // Fecha o arquivo
    close(fd);
}

// -----------------------------------------------------------
// Função: listarMusicas
// Objetivo: Listar todas as músicas do catálogo
// -----------------------------------------------------------
void listarMusicas() {
    int fd;
    char buffer[TAM];
    ssize_t bytes;

    // Abre o arquivo em modo leitura
    fd = open(ARQUIVO, O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir arquivo");
        return;
    }

    printf("\n--- Catálogo Musical ---\n");

    // Lê o arquivo em blocos de até TAM-1 bytes
    while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0'; // garante término da string
        printf("%s", buffer);
    }

    printf("\n-------------------------\n");

    // Fecha o arquivo
    close(fd);
}

// -----------------------------------------------------------
// Função Principal (main)
// Exibe o menu e chama as funções do catálogo
// -----------------------------------------------------------
int main() {
    int opcao;

    do {
        printf("\n=== Catálogo Musical ===\n");
        printf("1. Cadastrar música\n");
        printf("2. Listar músicas\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // consome o '\n' do scanf

        switch (opcao) {
            case 1:
                cadastrarMusica();
                break;
            case 2:
                listarMusicas();
                break;
            case 0:
                printf("Encerrando programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
