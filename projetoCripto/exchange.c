#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main() {

    Usuario usuario;
    Carteira carteira;
    Cotacoes cotacoes;

    int opcao;
    int loginRealizado = 0;

    // Menu inicial
    do {

        printf("\n===== EXCHANGE DE CRIPTOMOEDAS =====\n");
        printf("1 - Fazer login\n");
        printf("2 - Criar conta\n");
        printf("3 - Sair\n");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:

                // Tenta realizar o login
                while (login(&usuario) != 0) {
                    printf("\nCPF ou senha incorretos! Tente novamente.\n");
                }

                loginRealizado = 1;
                break;

            case 2:

                // Cria um novo usuario
                criarUsuario(&usuario);
                break;

            case 3:

                printf("\nEncerrando a exchange...\n");
                return 0;

            default:

                printf("\nOpcao invalida!\n");
                break;
        }

    } while (loginRealizado == 0);


    // Carrega os dados do usuario autenticado
    carregarCarteira(usuario.cpf, &carteira);
    carregarCotacoes(&cotacoes);


    // Menu principal
    do {

        printf("\n===== BEM-VINDO A EXCHANGE =====\n");
        printf("Usuario: %s\n", usuario.nome);

        printf("\n1 - Consultar saldo\n");
        printf("2 - Consultar extrato\n");
        printf("3 - Depositar reais\n");
        printf("4 - Sacar reais\n");
        printf("5 - Comprar criptomoedas\n");
        printf("6 - Vender criptomoedas\n");
        printf("7 - Atualizar cotacoes\n");
        printf("8 - Sair da conta\n");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:

                consultarSaldo(usuario, carteira);
                break;

            case 2:

                consultarExtrato(usuario.cpf);
                break;

            case 3:

                depositar(&carteira);
                salvarCarteira(carteira);
                break;

            case 4:

                sacar(&carteira);
                salvarCarteira(carteira);
                break;

            case 5:

                comprarCripto(&carteira, cotacoes);
                salvarCarteira(carteira);
                break;

            case 6:

                venderCripto(&carteira, cotacoes);
                salvarCarteira(carteira);
                break;

            case 7:

                atualizarCotacao(&cotacoes);
                salvarCotacoes(cotacoes);
                break;

            case 8:

                printf("\nVoce saiu da sua conta!\n");
                break;

            default:

                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 8);

    return 0;
}