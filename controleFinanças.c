#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_REGISTROS 100
#define MAX_INVESTIMENTOS 50
#define TAM_TEXTO 100
#define TAM_RELATORIO 15000

typedef struct {
    char descricao[TAM_TEXTO];
    char tipo[20];
    char status[30];
    double valor;
} RegistroRelatorio;

typedef struct {
    char nome[TAM_TEXTO];
    double valorInicial;
    double valorAtual;
    double rendimento;
} Investimento;

int main() {

    RegistroRelatorio registros[MAX_REGISTROS];
    Investimento investimentos[MAX_INVESTIMENTOS];

    int qtdRegistros = 0;
    int qtdInvestimentos = 0;

    int opcao = -1;

    double totalEntradas = 0;
    double totalSaidas = 0;
    double saldo = 0;

    char relatorio[TAM_RELATORIO] = "";
    int posicao;

    char entrada[64];   // buffer auxiliar para ler qualquer entrada

    do {

        printf("\n===== RELATORIO BANCARIO =====\n");
        printf("1 - Cadastrar movimentacao\n");
        printf("2 - Cadastrar investimento\n");
        printf("3 - Exibir movimentacoes\n");
        printf("4 - Gerar resumo bancario\n");
        printf("5 - Calcular rendimentos\n");
        printf("6 - Gerar relatorio em texto\n");
        printf("0 - Sair\n");

        printf("\nEscolha: ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL ||
            sscanf(entrada, "%d", &opcao) != 1) {
            opcao = -1;   // entrada invalida -> cai no default
        }

        switch (opcao) {

            case 1:

                if (qtdRegistros < MAX_REGISTROS) {

                    printf("\nDescricao: ");
                    fgets(registros[qtdRegistros].descricao,
                          TAM_TEXTO, stdin);
                    registros[qtdRegistros].descricao[
                        strcspn(registros[qtdRegistros].descricao, "\n")
                    ] = '\0';

                    printf("Tipo (Entrada ou Saida): ");
                    fgets(registros[qtdRegistros].tipo, 20, stdin);
                    registros[qtdRegistros].tipo[
                        strcspn(registros[qtdRegistros].tipo, "\n")
                    ] = '\0';

                    // remove espacos no final do tipo
                    int t = (int)strlen(registros[qtdRegistros].tipo);
                    while (t > 0 && registros[qtdRegistros].tipo[t - 1] == ' ') {
                        registros[qtdRegistros].tipo[t - 1] = '\0';
                        t--;
                    }

                    // converte o tipo para minusculas
                    for (int k = 0; registros[qtdRegistros].tipo[k] != '\0'; k++) {
                        if (registros[qtdRegistros].tipo[k] >= 'A' &&
                            registros[qtdRegistros].tipo[k] <= 'Z') {
                            registros[qtdRegistros].tipo[k] += 32;
                        }
                    }

                    printf("Status (Pago, Pendente etc.): ");
                    fgets(registros[qtdRegistros].status, 30, stdin);
                    registros[qtdRegistros].status[
                        strcspn(registros[qtdRegistros].status, "\n")
                    ] = '\0';

                    printf("Valor: R$ ");
                    fgets(entrada, sizeof(entrada), stdin);
                    for (int k = 0; entrada[k] != '\0'; k++) {
                        if (entrada[k] == ',') entrada[k] = '.';   // aceita virgula
                    }
                    registros[qtdRegistros].valor = atof(entrada);

                    qtdRegistros++;

                    printf("\nMovimentacao cadastrada!\n");

                } else {

                    printf("\nLimite de registros atingido!\n");

                }

                break;


            case 2:

                if (qtdInvestimentos < MAX_INVESTIMENTOS) {

                    printf("\nNome do investimento: ");
                    fgets(investimentos[qtdInvestimentos].nome,
                          TAM_TEXTO, stdin);
                    investimentos[qtdInvestimentos].nome[
                        strcspn(investimentos[qtdInvestimentos].nome, "\n")
                    ] = '\0';

                    printf("Valor inicial: R$ ");
                    fgets(entrada, sizeof(entrada), stdin);
                    for (int k = 0; entrada[k] != '\0'; k++) {
                        if (entrada[k] == ',') entrada[k] = '.';
                    }
                    investimentos[qtdInvestimentos].valorInicial = atof(entrada);

                    printf("Valor atual: R$ ");
                    fgets(entrada, sizeof(entrada), stdin);
                    for (int k = 0; entrada[k] != '\0'; k++) {
                        if (entrada[k] == ',') entrada[k] = '.';
                    }
                    investimentos[qtdInvestimentos].valorAtual = atof(entrada);

                    investimentos[qtdInvestimentos].rendimento = 0;

                    qtdInvestimentos++;

                    printf("\nInvestimento cadastrado!\n");

                } else {

                    printf("\nLimite de investimentos atingido!\n");

                }

                break;


            case 3:

                if (qtdRegistros == 0) {

                    printf("\nNenhuma movimentacao cadastrada.\n");

                } else {

                    printf("\n===== MOVIMENTACOES =====\n");

                    for (int i = 0; i < qtdRegistros; i++) {

                        printf("\nRegistro %d\n", i + 1);
                        printf("Descricao: %s\n", registros[i].descricao);
                        printf("Tipo: %s\n", registros[i].tipo);
                        printf("Status: %s\n", registros[i].status);
                        printf("Valor: R$ %.2f\n", registros[i].valor);
                    }
                }

                break;


            case 4:

                totalEntradas = 0;
                totalSaidas = 0;

                for (int i = 0; i < qtdRegistros; i++) {

                    if (strcmp(registros[i].tipo, "entrada") == 0) {
                        totalEntradas += registros[i].valor;
                    } else if (strcmp(registros[i].tipo, "saida") == 0) {
                        totalSaidas += registros[i].valor;
                    }

                }

                saldo = totalEntradas - totalSaidas;

                printf("\n===== RESUMO BANCARIO =====\n");
                printf("Total de entradas: R$ %.2f\n", totalEntradas);
                printf("Total de saidas: R$ %.2f\n", totalSaidas);
                printf("Saldo: R$ %.2f\n", saldo);

                break;


            case 5:

                if (qtdInvestimentos == 0) {

                    printf("\nNenhum investimento cadastrado.\n");

                } else {

                    printf("\n===== RENDIMENTOS =====\n");

                    for (int i = 0; i < qtdInvestimentos; i++) {

                        investimentos[i].rendimento =
                            investimentos[i].valorAtual -
                            investimentos[i].valorInicial;

                        printf("\nInvestimento: %s\n", investimentos[i].nome);
                        printf("Valor inicial: R$ %.2f\n", investimentos[i].valorInicial);
                        printf("Valor atual: R$ %.2f\n", investimentos[i].valorAtual);
                        printf("Rendimento: R$ %.2f\n", investimentos[i].rendimento);

                    }

                }

                break;


            case 6:

                if (qtdRegistros == 0 && qtdInvestimentos == 0) {
                    printf("\nNada cadastrado ainda. Use as opcoes 1 e 2 primeiro.\n");
                    break;
                }

                relatorio[0] = '\0';
                posicao = 0;

                // Cabecalho do relatorio
                posicao += snprintf(relatorio + posicao,
                                    TAM_RELATORIO - posicao,
                                    "===== RELATORIO BANCARIO =====\n\n");

                // Movimentacoes bancarias
                posicao += snprintf(relatorio + posicao,
                                    TAM_RELATORIO - posicao,
                                    "--- MOVIMENTACOES ---\n");

                for (int i = 0; i < qtdRegistros; i++) {

                    int tamanho = snprintf(
                        relatorio + posicao,
                        TAM_RELATORIO - posicao,
                        "Descricao: %s | Tipo: %s | Status: %s | Valor: R$ %.2f\n",
                        registros[i].descricao,
                        registros[i].tipo,
                        registros[i].status,
                        registros[i].valor
                    );

                    if (tamanho < 0 || tamanho >= TAM_RELATORIO - posicao) {
                        break;
                    }

                    posicao += tamanho;

                }

                // Investimentos
                if (posicao < TAM_RELATORIO - 1) {

                    posicao += snprintf(relatorio + posicao,
                                        TAM_RELATORIO - posicao,
                                        "\n--- INVESTIMENTOS ---\n");

                }

                for (int i = 0; i < qtdInvestimentos; i++) {

                    investimentos[i].rendimento =
                        investimentos[i].valorAtual -
                        investimentos[i].valorInicial;

                    int tamanho = snprintf(
                        relatorio + posicao,
                        TAM_RELATORIO - posicao,
                        "Nome: %s | Inicial: R$ %.2f | Atual: R$ %.2f | Rendimento: R$ %.2f\n",
                        investimentos[i].nome,
                        investimentos[i].valorInicial,
                        investimentos[i].valorAtual,
                        investimentos[i].rendimento
                    );

                    if (tamanho < 0 || tamanho >= TAM_RELATORIO - posicao) {
                        break;
                    }

                    posicao += tamanho;

                }

                // Resumo
                totalEntradas = 0;
                totalSaidas = 0;
                for (int i = 0; i < qtdRegistros; i++) {
                    if (strcmp(registros[i].tipo, "entrada") == 0)
                        totalEntradas += registros[i].valor;
                    else if (strcmp(registros[i].tipo, "saida") == 0)
                        totalSaidas += registros[i].valor;
                }
                saldo = totalEntradas - totalSaidas;

                if (posicao < TAM_RELATORIO - 1) {
                    posicao += snprintf(relatorio + posicao,
                                        TAM_RELATORIO - posicao,
                                        "\n--- RESUMO ---\nEntradas: R$ %.2f | Saidas: R$ %.2f | Saldo: R$ %.2f\n",
                                        totalEntradas, totalSaidas, saldo);
                }

                printf("\n%s", relatorio);

                break;


            case 0:

                printf("\nEncerrando o sistema...\n");

                break;


            default:

                printf("\nOpcao invalida!\n");

        }

    } while (opcao != 0);

    return 0;
}