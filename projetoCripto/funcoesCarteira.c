#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include <time.h>

// Registra uma movimentacao no extrato
static void registrarTransacao(Carteira carteira,
                               const char *movimentacao,
                               double cotacao,
                               double taxa) {

    FILE *arquivo;
    time_t agora;
    struct tm *dataHora;
    char dataHoraTexto[20];

    arquivo = fopen("extrato.txt", "a");

    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel registrar a movimentacao no extrato.\n");
        return;
    }

    // Obtem a data e o horario atuais
    agora = time(NULL);
    dataHora = localtime(&agora);

    if (dataHora == NULL) {
        printf("Aviso: nao foi possivel obter a data e o horario.\n");
        fclose(arquivo);
        return;
    }

    strftime(dataHoraTexto, sizeof(dataHoraTexto),
             "%d-%m-%Y %H:%M", dataHora);

    // Registra os dados da operacao e os saldos apos a movimentacao
    fprintf(arquivo,
            "%s;%s;%s;%.8f;%.2f;%.2f;%.8f;%.8f;%.6f\n",
            carteira.cpf,
            dataHoraTexto,
            movimentacao,
            cotacao,
            taxa,
            carteira.reais,
            carteira.bitcoin,
            carteira.ethereum,
            carteira.ripple);

    fclose(arquivo);
}


// Carrega a carteira de um usuario especifico
int carregarCarteira(const char *cpf, Carteira *carteira) {

    FILE *arquivo;
    char linha[300];
    char cpfSalvo[12];

    carteira->reais = 0.0;
    carteira->bitcoin = 0.0;
    carteira->ethereum = 0.0;
    carteira->ripple = 0.0;

    strcpy(carteira->cpf, cpf);

    arquivo = fopen("carteira.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        Carteira temporaria;

        if (sscanf(linha, "%11[^;];%lf;%lf;%lf;%lf",
                   cpfSalvo,
                   &temporaria.reais,
                   &temporaria.bitcoin,
                   &temporaria.ethereum,
                   &temporaria.ripple) == 5) {

            if (strcmp(cpf, cpfSalvo) == 0) {

                strcpy(carteira->cpf, cpfSalvo);

                carteira->reais = temporaria.reais;
                carteira->bitcoin = temporaria.bitcoin;
                carteira->ethereum = temporaria.ethereum;
                carteira->ripple = temporaria.ripple;

                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}


// Salva a carteira sem apagar os dados dos outros usuarios
int salvarCarteira(Carteira carteira) {

    FILE *original;
    FILE *temporario;

    char linha[300];
    char cpfSalvo[12];

    int encontrou = 0;

    original = fopen("carteira.txt", "r");
    temporario = fopen("carteira.tmp", "w");

    if (temporario == NULL) {
        printf("Erro ao criar arquivo temporario da carteira!\n");

        if (original != NULL) {
            fclose(original);
        }

        return 0;
    }

    // Percorre as carteiras ja cadastradas
    if (original != NULL) {

        while (fgets(linha, sizeof(linha), original) != NULL) {

            if (sscanf(linha, "%11[^;]", cpfSalvo) == 1 &&
                strcmp(cpfSalvo, carteira.cpf) == 0) {

                // Substitui somente a carteira do usuario atual
                fprintf(temporario,
                        "%s;%.2f;%.8f;%.8f;%.8f\n",
                        carteira.cpf,
                        carteira.reais,
                        carteira.bitcoin,
                        carteira.ethereum,
                        carteira.ripple);

                encontrou = 1;

            } else {

                // Mantem os dados dos demais usuarios
                fputs(linha, temporario);
            }
        }

        fclose(original);
    }

    // Se nao encontrou, adiciona uma nova carteira
    if (!encontrou) {

        fprintf(temporario,
                "%s;%.2f;%.8f;%.8f;%.8f\n",
                carteira.cpf,
                carteira.reais,
                carteira.bitcoin,
                carteira.ethereum,
                carteira.ripple);
    }

    fclose(temporario);

    // Substitui o arquivo original com seguranca
    remove("carteira.bak");

    if (rename("carteira.txt", "carteira.bak") != 0) {

        // Se o arquivo original nao existir, ainda podemos criar um novo
        original = fopen("carteira.txt", "r");

        if (original != NULL) {
            fclose(original);
            printf("Erro ao preparar a atualizacao da carteira!\n");
            remove("carteira.tmp");
            return 0;
        }
    }

    if (rename("carteira.tmp", "carteira.txt") != 0) {

        printf("Erro ao atualizar a carteira!\n");

        rename("carteira.bak", "carteira.txt");
        remove("carteira.tmp");

        return 0;
    }

    remove("carteira.bak");

    return 1;
}


// Realiza deposito em reais
void depositar(Carteira *carteira) {

    double valor;

    printf("\n========== DEPOSITO ==========\n");
    printf("Valor do deposito: R$ ");
    scanf("%lf", &valor);

    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    carteira->reais += valor;

    char movimentacao[100];

    sprintf(movimentacao, "+ %.2f REAL", valor);

    registrarTransacao(*carteira, movimentacao, 0.0, 0.0);

    printf("Deposito realizado com sucesso!\n");
    printf("Novo saldo: R$ %.2f\n", carteira->reais);
}


// Realiza saque em reais
void sacar(Carteira *carteira) {

    double valor;

    printf("\n========== SAQUE ==========\n");
    printf("Valor do saque: R$ ");
    scanf("%lf", &valor);

    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    if (valor > carteira->reais) {
        printf("Saldo insuficiente!\n");
        return;
    }

    carteira->reais -= valor;

    char movimentacao[100];

    sprintf(movimentacao, "- %.2f REAL", valor);

    registrarTransacao(*carteira, movimentacao, 0.0, 0.0);

    printf("Saque realizado com sucesso!\n");
    printf("Novo saldo: R$ %.2f\n", carteira->reais);
}


// Compra criptomoedas
void comprarCripto(Carteira *carteira, Cotacoes cotacoes) {

    int opcao;
    double quantidade;
    double cotacao;
    double taxa;
    double custo;
    double total;

    printf("\n========== COMPRAR CRIPTOMOEDAS ==========\n");

    printf("1 - Bitcoin (BTC) - R$ %.2f (com taxa: R$ %.2f)\n",
           cotacoes.bitcoin, cotacoes.bitcoin * 1.02);

    printf("2 - Ethereum (ETH) - R$ %.2f (com taxa: R$ %.2f)\n",
           cotacoes.ethereum, cotacoes.ethereum * 1.01);

    printf("3 - Ripple (XRP) - R$ %.2f (com taxa: R$ %.6f)\n",
           cotacoes.ripple, cotacoes.ripple * 1.01);

    printf("Escolha a criptomoeda: ");
    scanf("%d", &opcao);

    switch (opcao) {

        case 1:
            cotacao = cotacoes.bitcoin;
            taxa = 0.02;
            break;

        case 2:
            cotacao = cotacoes.ethereum;
            taxa = 0.01;
            break;

        case 3:
            cotacao = cotacoes.ripple;
            taxa = 0.01;
            break;

        default:
            printf("Opcao invalida!\n");
            return;
    }

    printf("Quantidade que deseja comprar: ");
    scanf("%lf", &quantidade);

    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    custo = quantidade * cotacao;
    total = custo + (custo * taxa);

    if (total > carteira->reais) {
        printf("Saldo em reais insuficiente!\n");
        printf("Custo total com taxa: R$ %.2f\n", total);
        return;
    }

    carteira->reais -= total;

    switch (opcao) {

        case 1:
            carteira->bitcoin += quantidade;
            break;

        case 2:
            carteira->ethereum += quantidade;
            break;

        case 3:
            carteira->ripple += quantidade;
            break;
    }

    char movimentacao[100];

    switch (opcao) {

        case 1:
            sprintf(movimentacao, "+ %.8f BTC", quantidade);
            break;

        case 2:
            sprintf(movimentacao, "+ %.8f ETH", quantidade);
            break;

        case 3:
            sprintf(movimentacao, "+ %.6f XRP", quantidade);
            break;
    }

    registrarTransacao(*carteira, movimentacao, cotacao, taxa * 100);

    printf("\nCompra realizada com sucesso!\n");
    printf("Valor da compra: R$ %.2f\n", custo);
    printf("Taxa: R$ %.2f\n", custo * taxa);
    printf("Total debitado: R$ %.2f\n", total);
    printf("Saldo em reais: R$ %.2f\n", carteira->reais);
}



// Venda criptomoedas
void venderCripto(Carteira *carteira, Cotacoes cotacoes) {

    int opcao;
    double quantidade;
    double cotacao;
    double taxa;
    double valorBruto;
    double valorTaxa;
    double total;

    printf("\n========== VENDER CRIPTOMOEDAS ==========\n");

    printf("\n1 - Bitcoin (BTC) - Quantidade possuida: %.8f BTC\n",
        carteira->bitcoin);
    printf("    Cotacao: R$ %.2f (com taxa: R$ %.2f)\n",
        cotacoes.bitcoin, cotacoes.bitcoin * (1 - 0.03));

    printf("\n2 - Ethereum (ETH) - Quantidade possuida: %.8f ETH\n",
        carteira->ethereum);
    printf("    Cotacao: R$ %.2f (com taxa: R$ %.2f)\n",
        cotacoes.ethereum, cotacoes.ethereum * (1 - 0.02));

    printf("\n3 - Ripple (XRP) - Quantidade possuida: %.6f XRP\n",
        carteira->ripple);
    printf("    Cotacao: R$ %.2f (com taxa: R$ %.6f)\n",
        cotacoes.ripple, cotacoes.ripple * (1 - 0.01));

    printf("Escolha a criptomoeda: ");
    scanf("%d", &opcao);

    switch (opcao) {

        case 1:
            cotacao = cotacoes.bitcoin;
            taxa = 0.03;
            break;

        case 2:
            cotacao = cotacoes.ethereum;
            taxa = 0.02;
            break;

        case 3:
            cotacao = cotacoes.ripple;
            taxa = 0.01;
            break;

        default:
            printf("Opcao invalida!\n");
            return;
    }

    printf("Quantidade que deseja vender: ");
    scanf("%lf", &quantidade);

    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }

    switch (opcao) {

        case 1:
            if (quantidade > carteira->bitcoin) {
                printf("Saldo de Bitcoin insuficiente!\n");
                return;
            }
            break;

        case 2:
            if (quantidade > carteira->ethereum) {
                printf("Saldo de Ethereum insuficiente!\n");
                return;
            }
            break;

        case 3:
            if (quantidade > carteira->ripple) {
                printf("Saldo de Ripple insuficiente!\n");
                return;
            }
            break;
    }

    valorBruto = quantidade * cotacao;
    valorTaxa = valorBruto * taxa;
    total = valorBruto - valorTaxa;

    switch (opcao) {

        case 1:
            carteira->bitcoin -= quantidade;
            break;

        case 2:
            carteira->ethereum -= quantidade;
            break;

        case 3:
            carteira->ripple -= quantidade;
            break;
    }

    carteira->reais += total;

    char movimentacao[100];

    switch (opcao) {

        case 1:
            sprintf(movimentacao, "- %.8f BTC", quantidade);
            break;

        case 2:
            sprintf(movimentacao, "- %.8f ETH", quantidade);
            break;

        case 3:
            sprintf(movimentacao, "- %.6f XRP", quantidade);
            break;
    }

    registrarTransacao(*carteira, movimentacao, cotacao, taxa * 100);

    printf("\nVenda realizada com sucesso!\n");
    printf("Valor bruto: R$ %.2f\n", valorBruto);
    printf("Taxa: R$ %.2f\n", valorTaxa);
    printf("Total recebido: R$ %.2f\n", total);
    printf("Novo saldo em reais: R$ %.2f\n", carteira->reais);
}