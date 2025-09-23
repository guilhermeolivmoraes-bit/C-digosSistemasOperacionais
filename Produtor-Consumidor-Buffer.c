// =================================================================================
// Inclusão de Bibliotecas Essenciais
// =================================================================================

// Inclui a biblioteca de Entrada e Saída Padrão (Standard Input/Output).
// É necessária para usar a função `printf()` para exibir mensagens no console.
#include <stdio.h>

// Inclui a biblioteca Padrão (Standard Library).
// É necessária para usar as funções `rand()` (gerar número aleatório) e `srand()` (inicializar o gerador de aleatórios).
#include <stdlib.h>

// Inclui a biblioteca de manipulação de Tempo.
// É necessária para usar a função `time()`, que nos ajuda a criar uma semente aleatória para `srand()`.
#include <time.h>

// Inclui a biblioteca específica da API do Windows.
// É necessária para usar a função `Sleep()`, que pausa a execução do programa.
#include <windows.h>

// =================================================================================
// Definição de Constantes Globais
// =================================================================================

// Define uma constante para o tamanho do buffer. Usar uma constante torna o código mais legível e fácil de modificar.
#define TAMANHO_BUFFER 5

// Define uma constante para o número total de itens que a simulação deve produzir e consumir.
#define NUM_ITEMS 10

// =================================================================================
// Declaração de Variáveis Globais (Simulando a Memória Compartilhada)
// =================================================================================
// Essas variáveis são "globais" (declaradas fora de qualquer função) para que possam ser acessadas
// tanto pelo produtor quanto pelo consumidor.

int buffer[TAMANHO_BUFFER]; // Este é o vetor (array) que funciona como nosso armazém compartilhado.
int contador = 0;           // Conta quantos itens estão DENTRO do buffer em um dado momento. Começa com 0.
int entrada = 0;            // Guarda o índice (a posição) onde o próximo item será INSERIDO pelo produtor. Começa em 0.
int saida = 0;              // Guarda o índice (a posição) de onde o próximo item será RETIRADO pelo consumidor. Começa em 0.

// Variáveis para controlar o fluxo da simulação.
int itens_produzidos_total = 0; // Conta o número total de itens criados desde o início.
int itens_consumidos_total = 0; // Conta o número total de itens consumidos desde o início.


// =================================================================================
// Função do Produtor
// =================================================================================
void produtor() {
    // Exibe uma mensagem informando que o produtor está tentando executar e mostra o estado atual do buffer.
    printf("Produtor: Tenta produzir item. Itens no buffer: %d\n", contador);

    // Verifica se o buffer está cheio. Esta é a principal condição de bloqueio do produtor.
    if (contador == TAMANHO_BUFFER) {
        // Se o contador de itens for igual ao tamanho máximo, o produtor não pode fazer nada.
        printf("Produtor: >>> Buffer CHEIO. Produtor bloqueado... <<<\n\n");
    } else {
        // Se a condição acima for falsa, significa que há espaço no buffer.
        
        // Cria um novo item. Aqui, geramos um número aleatório entre 1 e 100.
        int item = rand() % 100 + 1;

        // Coloca o item recém-criado no buffer, na posição indicada pelo índice 'entrada'.
        buffer[entrada] = item;
        // Informa qual item foi produzido e onde foi colocado.
        printf("Produtor: Item %d produzido e inserido na posicao %d.\n", item, entrada);

        // Atualiza o índice de 'entrada' para a próxima posição. O operador '%' (módulo) cria um efeito "circular".
        // Ex: Se o tamanho é 5 e 'entrada' é 4, (4+1)%5 resulta em 0, fazendo o índice voltar ao início.
        entrada = (entrada + 1) % TAMANHO_BUFFER;
        
        // Incrementa o contador de itens que estão atualmente no buffer.
        contador++;
        
        // Incrementa o contador do total de itens produzidos na simulação.
        itens_produzidos_total++;

        // Exibe o novo estado do buffer.
        printf("Produtor: Estado do buffer atualizado. Itens no buffer: %d\n\n", contador);
    }
}


// =================================================================================
// Função do Consumidor
// =================================================================================
void consumidor() {
    // Exibe uma mensagem informando que o consumidor está tentando executar e mostra o estado atual do buffer.
    printf("Consumidor: Tenta consumir item. Itens no buffer: %d\n", contador);

    // Verifica se o buffer está vazio. Esta é a principal condição de bloqueio do consumidor.
    if (contador == 0) {
        // Se o contador for 0, não há itens para consumir.
        printf("Consumidor: >>> Buffer VAZIO. Consumidor bloqueado... <<<\n\n");
    } else {
        // Se a condição acima for falsa, significa que há pelo menos um item para consumir.
        
        // Pega o item do buffer que está na posição indicada pelo índice 'saida'.
        int item = buffer[saida];
        // Informa qual item foi consumido e de qual posição ele foi retirado.
        printf("Consumidor: Item %d consumido da posicao %d.\n", item, saida);

        // Atualiza o índice de 'saida' para a próxima posição, usando a mesma lógica circular do produtor.
        saida = (saida + 1) % TAMANHO_BUFFER;
        
        // Decrementa o contador de itens que estão atualmente no buffer.
        contador--;
        
        // Incrementa o contador do total de itens consumidos na simulação.
        itens_consumidos_total++;

        // Exibe o novo estado do buffer.
        printf("Consumidor: Estado do buffer atualizado. Itens no buffer: %d\n\n", contador);
    }
}


// =================================================================================
// Função Principal (main) - Ponto de Entrada do Programa
// =================================================================================
int main() {
    // Inicializa o gerador de números aleatórios. Se isso não for feito, 'rand()' sempre geraria a mesma sequência de números.
    // `time(NULL)` pega a hora atual do sistema como "semente", garantindo que a sequência seja diferente a cada execução.
    srand(time(NULL));

    // Imprime as informações iniciais da simulação.
    printf("Iniciando simulacao Produtor-Consumidor em C (versao simples)...\n");
    printf("Tamanho do Buffer: %d\n", TAMANHO_BUFFER);
    printf("Itens a produzir/consumir: %d\n\n", NUM_ITEMS);

    // Este laço é o "coração" da simulação, funcionando como um escalonador de tarefas.
    // Ele continua executando enquanto a meta de produção OU a meta de consumo ainda não foi atingida.
    while (itens_produzidos_total < NUM_ITEMS || itens_consumidos_total < NUM_ITEMS) {
        
        // Sorteia um número: 0 ou 1. Isso vai decidir quem tentará executar na rodada.
        int vez = rand() % 2;

        // Se o número sorteado for 0 E o produtor ainda não produziu todos os itens...
        if (vez == 0 && itens_produzidos_total < NUM_ITEMS) {
            // ...chama a função produtor().
            produtor();
        // Senão, se o consumidor ainda não consumiu todos os itens...
        } else if (itens_consumidos_total < NUM_ITEMS) {
            // ...chama a função consumidor().
            consumidor();
        }

        // Pausa a execução do programa por 1000 milissegundos (ou seja, 1 segundo).
        // Isso serve apenas para podermos acompanhar a saída no console de forma legível.
        Sleep(1000); 
    }

    // Mensagem exibida quando o laço 'while' termina, indicando o fim da simulação.
    printf("Simulacao finalizada.\n");

    // Retorna 0 para o sistema operacional, indicando que o programa foi executado com sucesso.
    return 0;
}
