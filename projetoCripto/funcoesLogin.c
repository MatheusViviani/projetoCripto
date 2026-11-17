#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "funcoes.h"


// Verifica se o CPF ja esta cadastrado
int cpfExiste(char cpf[]) {

    FILE *arquivo;
    char linha[250];
    char nome[100];
    char cpfSalvo[12];
    char senha[7];

    arquivo = fopen("loginUsu.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%99[^;];%11[^;];%6s",
                   nome, cpfSalvo, senha) == 3) {

            if (strcmp(cpf, cpfSalvo) == 0) {
                fclose(arquivo);
                return 1;
            }
        }
    }

    fclose(arquivo);
    return 0;
}


// Funcao para criar um novo usuario
int criarUsuario(Usuario *usuario) {

    FILE *arquivo;
    FILE *arquivoCarteira;

    char cpf[12];
    int i;

    printf("\n===== CADASTRO DE USUARIO =====\n");

    // Solicita o nome
    printf("Digite seu nome completo: ");
    getchar();
    fgets(usuario->nome, 100, stdin);

    usuario->nome[strcspn(usuario->nome, "\n")] = '\0';

    // Solicita e valida o CPF
    do {

        printf("Digite seu CPF (11 numeros): ");
        scanf("%11s", cpf);

        if (strlen(cpf) != 11) {
            printf("CPF deve possuir exatamente 11 digitos!\n");
            continue;
        }

        for (i = 0; i < 11; i++) {
            if (!isdigit(cpf[i])) {
                break;
            }
        }

        if (i != 11) {
            printf("CPF deve conter somente numeros!\n");
            continue;
        }

        if (cpfExiste(cpf)) {
            printf("Este CPF ja esta cadastrado!\n");
            continue;
        }

        break;

    } while (1);

    strcpy(usuario->cpf, cpf);

    // Solicita e valida a senha
    do {

        printf("Crie uma senha de 6 digitos: ");
        scanf("%6s", usuario->senha);

        if (strlen(usuario->senha) != 6) {
            printf("A senha deve possuir exatamente 6 digitos!\n");
            continue;
        }

        for (i = 0; i < 6; i++) {
            if (!isdigit(usuario->senha[i])) {
                break;
            }
        }

        if (i != 6) {
            printf("A senha deve conter somente numeros!\n");
            continue;
        }

        break;

    } while (1);


    // Salva os dados do usuario
    arquivo = fopen("loginUsu.txt", "a");

    if (arquivo == NULL) {
        printf("Erro ao salvar usuario!\n");
        return 1;
    }

    fprintf(arquivo, "%s;%s;%s\n",
            usuario->nome,
            usuario->cpf,
            usuario->senha);

    fclose(arquivo);


    // Cria a carteira inicial do usuario
    arquivoCarteira = fopen("carteira.txt", "a");

    if (arquivoCarteira == NULL) {
        printf("Erro ao criar carteira!\n");
        return 1;
    }

    fprintf(arquivoCarteira, "%s;0.00;0.000000;0.000000;0.000000\n",
            usuario->cpf);

    fclose(arquivoCarteira);

    printf("\nUsuario cadastrado com sucesso!\n");

    return 0;
}


// Funcao para realizar login
int login(Usuario *usuario) {

    FILE *arquivo;

    char cpfDigitado[12];
    char senhaDigitada[7];

    char linha[250];
    char nomeSalvo[100];
    char cpfSalvo[12];
    char senhaSalva[7];

    printf("\n===== LOGIN =====\n");

    printf("Digite seu CPF: ");
    scanf("%11s", cpfDigitado);

    printf("Digite sua senha: ");
    scanf("%6s", senhaDigitada);

    arquivo = fopen("loginUsu.txt", "r");

    if (arquivo == NULL) {
        printf("Nenhum usuario cadastrado!\n");
        return 1;
    }

    // Procura o usuario no arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        if (sscanf(linha, "%99[^;];%11[^;];%6s",
                   nomeSalvo, cpfSalvo, senhaSalva) == 3) {

            if (strcmp(cpfDigitado, cpfSalvo) == 0 &&
                strcmp(senhaDigitada, senhaSalva) == 0) {

                strcpy(usuario->nome, nomeSalvo);
                strcpy(usuario->cpf, cpfSalvo);
                strcpy(usuario->senha, senhaSalva);

                fclose(arquivo);
                return 0;
            }
        }
    }

    fclose(arquivo);

    return 1;
}