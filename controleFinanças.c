
#include <stdio.h>
#include <string.h>

#define MAX_REGISTROS 100
#define MAX_INVESTIMENTOS 50
#define TAM_TEXTO 100
#define TAM_RELATORIO 15000

typedef struct {
    char descricao[TAM_TEXTO];
    char tipo[20];
    char status[30];
    float valor;
} RegistroRelatorio;

typedef struct {
    char nome[TAM_TEXTO];
    float valorInicial;
    float valorAtual;
    float rendimento;
} Investimento;

int main() {

    RegistroRelatorio registros[MAX_REGISTROS];
    Investimento investimentos[MAX_INVESTIMENTOS];

    int qtdRegistros = 0;
    int qtdInvestimentos = 0;

    int opcao;

    float totalEntradas = 0;
    float totalSaidas = 0;
    float saldo = 0;

    char relatorio[TAM_RELATORIO] = "";
    int posicao;

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
        scanf("%d", &opcao);
        getchar();

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

                    printf("Status (Pago, Pendente etc.): ");
                    fgets(registros[qtdRegistros].status, 30, stdin);

                    registros[qtdRegistros].status[
                        strcspn(registros[qtdRegistros].status, "\n")
                    ] = '\0';

                    printf("Valor: R$ ");
                    scanf("%f", &registros[qtdRegistros].valor);
                    getchar();

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
                    scanf("%f",
                          &investimentos[qtdInvestimentos].valorInicial);

                    printf("Valor atual: R$ ");
                    scanf("%f",
                          &investimentos[qtdInvestimentos].valorAtual);

                    getchar();

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
                        printf("Descricao: %s\n",
                               registros[i].descricao);

                        printf("Tipo: %s\n",
                               registros[i].tipo);

                        printf("Status: %s\n",
                               registros[i].status);

                        printf("Valor: R$ %.2f\n",
                               registros[i].valor);
                    }
                }

                break;


            case 4:

                totalEntradas = 0;
                totalSaidas = 0;

                for (int i = 0; i < qtdRegistros; i++) {

                    if (strcmp(registros[i].tipo, "Entrada") == 0 ||
                        strcmp(registros[i].tipo, "entrada") == 0) {

                        totalEntradas += registros[i].valor;

                    } else if (
                        strcmp(registros[i].tipo, "Saida") == 0 ||
                        strcmp(registros[i].tipo, "saida") == 0) {

                        totalSaidas += registros[i].valor;

                    }

                }

                saldo = totalEntradas - totalSaidas;

                printf("\n===== RESUMO BANCARIO =====\n");

                printf("Total de entradas: R$ %.2f\n",
                       totalEntradas);

                printf("Total de saidas: R$ %.2f\n",
                       totalSaidas);

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

                        printf("\nInvestimento: %s\n",
                               investimentos[i].nome);

                        printf("Valor inicial: R$ %.2f\n",
                               investimentos[i].valorInicial);

                        printf("Valor atual: R$ %.2f\n",
                               investimentos[i].valorAtual);

                        printf("Rendimento: R$ %.2f\n",
                               investimentos[i].rendimento);

                    }

                }

                break;


            case 6:

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
