#include "genius.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>

/*********************************************************************
 * Implementações
 */

/* SINAIL ************************************************************/

/**
 * @brief struct Sinal* criar_sinal(enum TipoSinal tipo)
 * 
 * Criar um novo sinal do tipo passado.
 * 
 * O sinal criado mantem o `prox` ainda NULL. Para fazer o link
 * entre sinais deve-se tratar no chamador.
 */
struct Sinal* criar_sinal(enum TipoSinal tipo) {
    struct Sinal* s = (struct Sinal*)malloc(sizeof(struct Sinal));
    s->tipo = tipo;
    s->prox = NULL;
    return s;
}

/**
 * @brief void destruir_sinal(struct Sinal* s)
 * 
 * Liberar recursos de um sinal criado.
 */
void destruir_sinal(struct Sinal* s) {
    while(s) {
        struct Sinal* proximo = s->prox;
        free(s);
        s = proximo;
    }
}

/* SEQUENCIA ********************************************************/

struct Sequencia* iniciar_sequencia(struct Nivel nivel, bool gerar_primeiro_sinal) {
    struct Sequencia* seq = (struct Sequencia*)malloc(sizeof(struct Sequencia));
    seq->capacidade = nivel.capacidade;
    seq->comprimento = 0;
    seq->sinal = NULL;
    if (gerar_primeiro_sinal) {
        srand(time(NULL));
        enum TipoSinal tipo = (enum TipoSinal)(rand() % MAX_TIPO_SINAIS);
        seq->sinal = criar_sinal(tipo);
        seq->comprimento = 1;
    }
    return seq;
}

struct Sequencia* adicionar_sinal(struct Sequencia* seq, enum TipoSinal tipo) {
    if (seq->comprimento >= seq->capacidade) {
        return NULL;
    }

    struct Sinal* sinal = criar_sinal(tipo);

    if (seq->sinal == NULL) {
        seq->sinal = sinal;
    } else {
        if (seq->sinal->prox == NULL) {
            seq->sinal->prox = sinal;
        } else {
            struct Sinal* proximo_sinal = seq->sinal->prox;
            struct Sinal* ultimo_sinal = proximo_sinal;
            while (proximo_sinal != NULL) {
                ultimo_sinal = proximo_sinal;
                proximo_sinal = proximo_sinal->prox;
            }
            ultimo_sinal->prox = sinal;
        }
    }

    seq->comprimento++;

    return seq;
}

int comparar(struct Sequencia* a, struct Sequencia* b) {
    if (a->comprimento != b->comprimento) {
        return a->comprimento > b->comprimento ? 1 : -1;
    }

    // aqui os comprimentos são iguais
    // então temos que validar os sinais
    unsigned short comprimento_comum = a->comprimento;
    unsigned short comprimento = 0;
    struct Sinal* sinal_a = a->sinal;
    struct Sinal* sinal_b = b->sinal;
    while (comprimento < comprimento_comum) {
        if (sinal_a->tipo != sinal_b->tipo) {
            return sinal_a->tipo > sinal_b->tipo ? 1 : -1;
        }
        sinal_a = sinal_a->prox;
        sinal_b = sinal_b->prox;
        comprimento++;
    }

    // iguais
    return 0;
}

enum TipoSinal sinal_da_posicao(struct Sequencia* seq, int posicao) {
    struct Sinal* sinal = seq->sinal;
    for (int i = 1; i < posicao; i++) {
        sinal = sinal->prox;
    }
    if (sinal == NULL) {
        return INVALIDO;
    }
    return sinal->tipo;
}

void finalizar_sequencia(struct Sequencia* seq) {
    destruir_sinal(seq->sinal);
    seq->capacidade = 0;
    seq->comprimento = 0;
    free(seq);
}

/* GENIUS - JOGO ****************************************************/

struct Genius* iniciar_jogo(enum Estado estado, struct Nivel nivel, unsigned short pontuacao, unsigned short recorde) {
    struct Genius* jogo = (struct Genius*)malloc(sizeof(struct Genius));
    jogo->estado = estado;
    jogo->nivel = nivel;
    jogo->pontuacao = pontuacao;
    jogo->recorde = recorde;
    jogo->seq = iniciar_sequencia(nivel, 1);
    jogo->seq_jogador = iniciar_sequencia(nivel, 0);
    return jogo;
}

void finalizar_jogo(struct Genius* jogo) {
    finalizar_sequencia(jogo->seq);
    finalizar_sequencia(jogo->seq_jogador);
    jogo->estado = FIM_DE_JOGO;
    jogo->nivel = nivel_facil;
    jogo->pontuacao = 0;
    jogo->recorde = 0;
    free(jogo);
}

struct Genius* genius_adicionar_sinal(struct Genius* jogo) {
    return jogo;
}

struct Genius* genius_apresentar_sequencia(struct Genius* jogo) {
    jogo->estado = APRESENTANDO_SEQUENCIA;

    char* tipo_sinal_str;
    struct Sinal* sinal_atual = jogo->seq->sinal;

    while (sinal_atual != NULL) {
        switch (sinal_atual->tipo) {
            case VERDE:
                tipo_sinal_str = "\x1b[32m\u2588\x1b[0m";
            break;
            case VERMELHO:
                tipo_sinal_str = "\x1b[31m\u2588\x1b[0m";
            break;
            case AZUL:
                tipo_sinal_str = "\x1b[34m\u2588\x1b[0m";
            break;
            case AMARELO:
                tipo_sinal_str = "\x1b[33m\u2588\x1b[0m";
            break;
            case INVALIDO:
                jogo->estado = SAINDO;
        }
        printf("\r%s%s%s ", tipo_sinal_str, tipo_sinal_str, tipo_sinal_str);
        fflush(stdout);
        sleep(2);
        sinal_atual = sinal_atual->prox;
    }

    printf("\rSua vez ");
    fflush(stdout);

    jogo->estado = AGUARDANDO_RESPOSTA;

    return jogo;
}

struct Genius* genius_aguardar_jogador(struct Genius* jogo) {
    jogo->estado = AGUARDANDO_RESPOSTA;

    // contador de quantas "teclas" apertadas pelo jogador
    int contador = 0;

    while (jogo->seq_jogador->comprimento < jogo->seq->comprimento) {
        char ch = getchar();

        if (ch >= '1' && ch <= '4') {
            contador++;
            enum TipoSinal resposta = ch - '0';
            switch (resposta) {
                case VERDE:
                    resposta = VERDE;
                    adicionar_sinal(jogo->seq_jogador, VERDE);
                break;
                case VERMELHO:
                    resposta = VERMELHO;
                    adicionar_sinal(jogo->seq_jogador, VERMELHO);
                break;
                case AMARELO:
                    resposta = AMARELO;
                    adicionar_sinal(jogo->seq_jogador, AMARELO);
                break;
                case AZUL:
                    resposta = AZUL;
                    adicionar_sinal(jogo->seq_jogador, AZUL);
                break;
                case INVALIDO:
                    resposta = INVALIDO;
                    jogo->estado = FIM_DE_JOGO;
                    break;
                break;
            }
            
            // compara posicao do sinal na jogo->seq com o tipo de sinal inserido pelo jogador
            enum TipoSinal sinal_seq_comparar = sinal_da_posicao(jogo->seq, contador);
            if (resposta != sinal_seq_comparar) {
                jogo->estado = FIM_DE_JOGO;
                break;
            }
        }
    }

    if (jogo->seq_jogador->comprimento == jogo->seq->comprimento) {
        jogo->estado = RODADA_CONCLUIDA;
    }

    return jogo;
}

struct Genius* genius_concluir_rodada(struct Genius* jogo) {
    srand(time(NULL));
    enum TipoSinal tipo = (enum TipoSinal)(rand() % MAX_TIPO_SINAIS);
    struct Sequencia* seq = adicionar_sinal(jogo->seq, tipo);
    if (seq == NULL) {
        // finalizou o jogo pois completou a capacidade do nivel
        jogo->estado = SAINDO;
    }
    return jogo;
}

/** PONTOS DE ENTRADA ********************************************** */

int genius_terminal() {
    printf("******\nGENIUS\n******\n\n");
    
    char opcao;
    enum Dificuldade dificuldade;
    struct Nivel nivel;
    unsigned short pontuacao = 0;
    unsigned short recorde = 0;
    struct Genius* jogo;
    
    printf("Quer jogar? Digite sim ou não (S/n): ");
    scanf("%c", &opcao);

    if (opcao == 's' || opcao == 'S') {
        printf("\nEscolha a dificuldade:\n(1) FÁCIL\t(2) MÉDIO\t(3) DIFÍCIL: ");
        scanf("%d", &dificuldade);

        printf("\n---\nUtilize o teclado para jogar:\n(1) VERDE\t(2) VERMELHO\t(3) AMARELO\t(4) AZUL\n---\n\n");

        switch (dificuldade) {
            case FACIL:
                nivel = nivel_facil;
            break;
            case MEDIO:
                nivel = nivel_medio;
            break;
            case DIFICIL:
                nivel = nivel_dificil;
            break;
            default:
                nivel = nivel_facil;
        }

        jogo = iniciar_jogo(
            PRONTO,
            nivel,
            pontuacao,
            recorde
        );

        // teste
        // adicionar_sinal(jogo->seq, VERDE);
        // adicionar_sinal(jogo->seq, AMARELO);
        // adicionar_sinal(jogo->seq, VERMELHO);
        // adicionar_sinal(jogo->seq, AZUL);
        // adicionar_sinal(jogo->seq, VERDE);
        //

        while(jogo->estado != SAINDO) {
            switch (jogo->estado) {
                case APRESENTANDO_SEQUENCIA:
                    genius_apresentar_sequencia(jogo);
                break;
                case AGUARDANDO_RESPOSTA:
                    genius_aguardar_jogador(jogo);
                break;
                case RODADA_CONCLUIDA:
                    genius_concluir_rodada(jogo);
                break;
                default:
                    jogo->estado = APRESENTANDO_SEQUENCIA;
            }
        }

        finalizar_jogo(jogo);
    }

    return 0;
}