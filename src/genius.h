#ifndef _GENIUS_H_
#define _GENIUS_H_

#include <stdbool.h>

/*********************************************************************
 * Definições e structs
 */

#define MAX_TIPO_SINAIS 4

enum Dificuldade {
    FACIL = 1,
    MEDIO,
    DIFICIL
};

struct Nivel {
    enum Dificuldade dificuldade;
    unsigned short capacidade;
};

enum TipoSinal {
    VERDE = 1,  // = 1
    VERMELHO,   // = 2
    AMARELO,    // = 3
    AZUL,       // = 4
    INVALIDO    // = 5
};

/**
 * Transições esperadas:
 * 
 * 1. `PRONTO` → `ADICIONANDO_SINAL`, quando a partida começa.
 * 2. `ADICIONANDO_SINAL` → `APRESENTANDO_SEQUENCIA`, após acrescentar um sinal.
 * 3. `APRESENTANDO_SEQUENCIA` → `AGUARDANDO_RESPOSTA`, quando todos os sinais foram apresentados.
 * 4. `AGUARDANDO_RESPOSTA` → `FIM_DE_JOGO`, quando uma resposta válida não corresponde ao sinal esperado.
 * 5. `AGUARDANDO_RESPOSTA` → `RODADA_CONCLUIDA`, quando toda a sequência foi reproduzida corretamente.
 * 6. `RODADA_CONCLUIDA` → `ADICIONANDO_SINAL`, para iniciar a próxima rodada.
 * 7. `FIM_DE_JOGO` → `PRONTO`, se o jogador escolher reiniciar.
 * 8. `FIM_DE_JOGO` → `SAINDO`, se o jogador escolher sair.
 * 
 */
enum Estado {
    // PRONTO: aguardando o início da partida.
    PRONTO,
    // ADICIONANDO_SINAL: gerando e armazenando o novo sinal da rodada.
    ADICIONANDO_SINAL,
    // APRESENTANDO_SEQUENCIA: mostrando a sequência ao jogador.
    APRESENTANDO_SEQUENCIA,
    // AGUARDANDO_RESPOSTA: esperando input do jogador
    AGUARDANDO_RESPOSTA,
     // RODADA_CONCLUIDA: todas as respostas da rodada foram corretas.
    RODADA_CONCLUIDA,
    // FIM_DE_JOGO: uma resposta foi incorreta ou a partida terminou por outra condição.
    FIM_DE_JOGO,
    // SAINDO: encerrando a execução.
    SAINDO
};

struct Sinal {
    // Tipo do sinal que está sendo representado
    enum TipoSinal tipo;
    struct Sinal *prox;
};

/**
 * @brief Sequencia. Armazenar sinais gerados na ordem em que foram adicionados.
 */
struct Sequencia {
    // Primeiro sinal
    struct Sinal* sinal;
    // Informar quantos sinais válidos existem na sequência
    unsigned short comprimento;
    // Definir quantos sinais podem ser armazenados com segurança
    unsigned short capacidade;
};

/**
 * @brief Representação estrutura do jogo
 * 
 */
struct Genius {
    // Armazenar os sinais gerados na ordem em que foram adicionados
    struct Sequencia* seq;
    // Armazenar os sinais inputados pelo jogador
    struct Sequencia* seq_jogador;
    // Representar a progressão da partida
    struct Nivel nivel;
    // Registrar o resultado conforme o critério definido
    unsigned int pontuacao;
    // Identificar a fase/estado de máquina atual da partida
    enum Estado estado;
    // Guardar a melhor pontuação, se essa funcionalidade for implementada
    unsigned int recorde;
};

static struct Nivel nivel_facil = {
    .dificuldade = FACIL,
    .capacidade = 10
};

static struct Nivel nivel_medio = {
    .dificuldade = MEDIO,
    .capacidade = 20
};

static struct Nivel nivel_dificil = {
    .dificuldade = DIFICIL,
    .capacidade = 50
};

/*********************************************************************
 * Interfaces e funções
 */

/**
 * @brief Sinal
 * 
 * Interfaces para tratar sinais
 */
struct Sinal* criar_sinal(enum TipoSinal tipo);
void destruir_sinal(struct Sinal* s);

/**
 * @brief Sequencia
 * 
 * O que podemos fazer:
 * 
 * - iniciar_sequencia
 * - adicionar_sinal
 * - comparar
 * - finalizar_sequencia
 */
struct Sequencia* iniciar_sequencia(struct Nivel nivel, bool gerar_primeiro_sinal);
struct Sequencia* adicionar_sinal(struct Sequencia* seq, enum TipoSinal tipo);
int comparar(struct Sequencia* a, struct Sequencia* b);
enum TipoSinal sinal_da_posicao(struct Sequencia* seq, int posicao);
void finalizar_sequencia(struct Sequencia* seq);

/**
 * @brief Genius
 * 
 * O que podemos fazer:
 * 
 * - iniciar jogo
 * - finalizar jogo
 * - adicionar sinal
 * - apresentar sequencia
 * - aguardar jogador
 * - concluir rodada
 * 
 */
struct Genius* iniciar_jogo(enum Estado estado, struct Nivel nivel, unsigned short pontuacao, unsigned short recorde);
struct Genius* genius_adicionar_sinal(struct Genius* jogo);
struct Genius* genius_apresentar_sequencia(struct Genius* jogo);
struct Genius* genius_aguardar_jogador(struct Genius* jogo);
struct Genius* genius_concluir_rodada(struct Genius* jogo);
void finalizar_jogo(struct Genius* jogo);

int genius_terminal();

#endif