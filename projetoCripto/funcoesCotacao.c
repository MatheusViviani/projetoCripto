#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"


// Carrega as cotacoes do arquivo
int carregarCotacoes(Cotacoes *cotacoes) {

    FILE *arquivo;

    arquivo = fopen("cotacao.txt", "r");

    // Define cotacoes iniciais caso o arquivo nao exista
    if (arquivo == NULL) {

        cotacoes->bitcoin = 350000.00;
        cotacoes->ethereum = 12000.00;
        cotacoes->ripple = 3.00;

        salvarCotacoes(*cotacoes);

        return 0;
    }

    if (fscanf(arquivo, "%lf;%lf;%lf",
               &cotacoes->bitcoin,
               &cotacoes->ethereum,
               &cotacoes->ripple) != 3) {

        fclose(arquivo);

        cotacoes->bitcoin = 350000.00;
        cotacoes->ethereum = 12000.00;
        cotacoes->ripple = 3.00;

        salvarCotacoes(*cotacoes);

        return 0;
    }

    fclose(arquivo);

    return 1;
}


// Salva as cotacoes no arquivo
int salvarCotacoes(Cotacoes cotacoes) {

    FILE *arquivo;

    arquivo = fopen("cotacao.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao salvar as cotacoes!\n");
        return 0;
    }

    fprintf(arquivo, "%.2f;%.2f;%.6f\n",
            cotacoes.bitcoin,
            cotacoes.ethereum,
            cotacoes.ripple);

    fclose(arquivo);

    return 1;
}


// Atualiza as cotacoes aleatoriamente

void atualizarCotacao(Cotacoes *cotacoes) {

    static int inicializado = 0;

    int quantidade;
    int rodada;

    double variacaoBTC;
    double variacaoETH;
    double variacaoXRP;

    if (!inicializado) {
        srand((unsigned int)time(NULL));
        inicializado = 1;
    }

    printf("\n========== ATUALIZACAO DE COTACOES ==========\n");
    printf("Quantas vezes deseja atualizar? (1 a 10): ");
    scanf("%d", &quantidade);

    if (quantidade < 1 || quantidade > 10) {
        printf("\nQuantidade invalida! Escolha um valor entre 1 e 10.\n");
        return;
    }

    for (rodada = 1; rodada <= quantidade; rodada++) {

        // Gera variacoes entre -5% e +5%
        variacaoBTC = (rand() % 1001 - 500) / 10000.0;
        variacaoETH = (rand() % 1001 - 500) / 10000.0;
        variacaoXRP = (rand() % 1001 - 500) / 10000.0;

        // Aplica as variacoes sobre as cotacoes atuais
        cotacoes->bitcoin *= (1.0 + variacaoBTC);
        cotacoes->ethereum *= (1.0 + variacaoETH);
        cotacoes->ripple *= (1.0 + variacaoXRP);

        printf("\n========== RODADA %d ==========\n", rodada);

        printf("Bitcoin:  R$ %.2f (%+.2f%%)\n",
               cotacoes->bitcoin, variacaoBTC * 100);

        printf("Ethereum: R$ %.2f (%+.2f%%)\n",
               cotacoes->ethereum, variacaoETH * 100);

        printf("Ripple:   R$ %.6f (%+.2f%%)\n",
               cotacoes->ripple, variacaoXRP * 100);
    }

    printf("\n========== RESULTADO FINAL ==========\n");
    printf("Total de atualizacoes: %d\n", quantidade);
    printf("Bitcoin:  R$ %.2f\n", cotacoes->bitcoin);
    printf("Ethereum: R$ %.2f\n", cotacoes->ethereum);
    printf("Ripple:   R$ %.6f\n", cotacoes->ripple);
    printf("=====================================\n");
}