#include <stdio.h>
#include <string.h>
#include "funcoes.h"

// Consulta o saldo da carteira do usuario
void consultarSaldo(Usuario usuario, Carteira carteira) {

    printf("\n========== SALDO DA CONTA ==========\n");
    printf("Usuario: %s\n", usuario.nome);
    printf("CPF: %s\n", usuario.cpf);

    printf("\nSaldo em reais: R$ %.2f\n", carteira.reais);

    printf("\nCriptomoedas:\n");
    printf("Bitcoin:  %.8f BTC\n", carteira.bitcoin);
    printf("Ethereum: %.8f ETH\n", carteira.ethereum);
    printf("Ripple:   %.8f XRP\n", carteira.ripple);

    printf("====================================\n");
}


// Consulta somente o extrato do usuario autenticado
void consultarExtrato(const char *cpf) {

    FILE *arquivo;

    char linha[500];
    char cpfSalvo[12];
    char dataHora[20];
    char movimentacao[100];

    double cotacao;
    double taxa;
    double reais;
    double bitcoin;
    double ethereum;
    double ripple;

    int encontrou = 0;

    arquivo = fopen("extrato.txt", "r");

    printf("\n=============================== EXTRATO =================================\n");

    if (arquivo == NULL) {
        printf("Nenhuma movimentacao registrada.\n");
        printf("=========================================================================\n");
        return;
    }

    printf("+------------------+----------------------+---------------+--------+------------+------------+------------+------------+\n");
    printf("| DATA / HORA      | MOVIMENTACAO         | COTACAO       | TAXA   | REAL       | BTC        | ETH        | XRP        |\n");
    printf("+------------------+----------------------+---------------+--------+------------+------------+------------+------------+\n");

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha,
                   "%11[^;];%19[^;];%99[^;];%lf;%lf;%lf;%lf;%lf;%lf",
                   cpfSalvo,
                   dataHora,
                   movimentacao,
                   &cotacao,
                   &taxa,
                   &reais,
                   &bitcoin,
                   &ethereum,
                   &ripple) == 9) {

            if (strcmp(cpf, cpfSalvo) == 0) {

                printf("| %-16s | %-20s | %13.6f | %5.2f%% | %10.2f | %10.8f | %10.8f | %10.6f |\n",
                       dataHora,
                       movimentacao,
                       cotacao,
                       taxa,
                       reais,
                       bitcoin,
                       ethereum,
                       ripple);

                encontrou = 1;
            }
        }
    }

    printf("+------------------+----------------------+---------------+--------+------------+------------+------------+------------+\n");

    if (!encontrou) {
        printf("Nenhuma movimentacao encontrada para este usuario.\n");
    }

    fclose(arquivo);

    printf("=========================================================================\n");
}