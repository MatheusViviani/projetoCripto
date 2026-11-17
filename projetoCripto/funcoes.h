#ifndef FUNCOES_H
#define FUNCOES_H

// Estrutura com os dados do usuario
typedef struct {
    char nome[100];
    char cpf[12];
    char senha[7];
} Usuario;


// Estrutura com os dados da carteira
typedef struct {
    char cpf[12];
    double reais;
    double bitcoin;
    double ethereum;
    double ripple;
} Carteira;


// Estrutura com as cotacoes das criptomoedas
typedef struct {
    double bitcoin;
    double ethereum;
    double ripple;
} Cotacoes;


// FUNCOES DE LOGIN
int criarUsuario(Usuario *usuario);
int login(Usuario *usuario);


// FUNCOES DE CONSULTA
void consultarSaldo(Usuario usuario, Carteira carteira);
void consultarExtrato(const char *cpf);


// FUNCOES DA CARTEIRA
void depositar(Carteira *carteira);
void sacar(Carteira *carteira);
void comprarCripto(Carteira *carteira, Cotacoes cotacoes);
void venderCripto(Carteira *carteira, Cotacoes cotacoes);


// FUNCOES DE COTACAO
void atualizarCotacao(Cotacoes *cotacoes);


// FUNCOES DE ARQUIVOS
int carregarCarteira(const char *cpf, Carteira *carteira);
int salvarCarteira(Carteira carteira);

int carregarCotacoes(Cotacoes *cotacoes);
int salvarCotacoes(Cotacoes cotacoes);

#endif