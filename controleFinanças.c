/*
 * SISTEMA DE GESTAO FINANCEIRA - VERSAO INTEGRADA
 * Programacao Estruturada - UNEB Campus II
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USUARIOS 10
#define MAX_REGISTROS 50
#define MAX_INVESTIMENTOS 30
#define MAX_HISTORICOS 50
#define TAM_TEXTO 100
#define TAM_RELATORIO 15000

/* ================================================================== */
/* ESTRUTURAS DO GRUPO (iguais ao codigo original)                     */
/* ================================================================== */
typedef struct {
    char descricao[TAM_TEXTO];
    char tipo[20];
    char status[30];
    double valor;
} RegistroRelatorio;

typedef struct {
    char nome[TAM_TEXTO];
    char tipo[30];
    char status[30];
    double valorInicial;
    double valorAtual;
    double rendimento;
} Investimento;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    char nomeInvestimento[TAM_TEXTO];
    Data periodo;
    double valorAnterior;
    double valorAtualizado;
    double evolucao;
} HistoricoInvestimento;

typedef struct {
    char tipo[30];
    int numeroConta;
    int agencia;
    double dinheiroInicial;
} ContaBancaria;

typedef struct {
    char nome[TAM_TEXTO];
    char senha[30];
    ContaBancaria conta;
    RegistroRelatorio registros[MAX_REGISTROS];
    int qtdRegistros;
    Investimento investimentos[MAX_INVESTIMENTOS];
    int qtdInvestimentos;
    HistoricoInvestimento historicos[MAX_HISTORICOS];
    int qtdHistoricos;
} Usuario;

/* Dados do sistema (globais, para as funcoes e menus compartilharem) */
Usuario usuarios[MAX_USUARIOS];
int qtdUsuarios = 0;
char relatorio[TAM_RELATORIO] = "";

/* ================================================================== */
/* MENU: estrutura e funcoes de apoio                                  */
/* ================================================================== */
struct MenuOpcao {
    int codigo;      /* numero que o usuario digita */
    char nome[60];   /* texto mostrado na tela */
};

int lerInt(const char *msg)
{
    char buf[64];
    int v;
    for (;;) {
        printf("%s", msg);
        if (fgets(buf, sizeof buf, stdin) == NULL) return 0;
        if (sscanf(buf, "%d", &v) == 1) return v;
        printf("Entrada invalida. Digite apenas numeros.\n");
    }
}

void limparTela(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausa(void)
{
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void cabecalho(const char *titulo, const char *subtitulo)
{
    printf("==========================================\n");
    printf("   SISTEMA DE GESTAO FINANCEIRA\n");
    printf("==========================================\n");
    printf("  %s\n", titulo);
    if (subtitulo[0] != '\0')
        printf("  %s\n", subtitulo);
    printf("------------------------------------------\n");
}

/* Confere se o codigo digitado existe no vetor de opcoes */
int opcaoExiste(struct MenuOpcao opcoes[], int n, int escolha)
{
    int i;
    for (i = 0; i < n; i++)
        if (opcoes[i].codigo == escolha)
            return 1;
    return 0;
}

/* Mostra as opcoes com laco e repete a leitura ate a escolha ser valida */
int escolherOpcao(const char *titulo, const char *subtitulo,
                  struct MenuOpcao opcoes[], int n)
{
    int i, escolha;

    for (;;) {
        limparTela();
        cabecalho(titulo, subtitulo);
        for (i = 0; i < n; i++)
            printf("  [%d] %s\n", opcoes[i].codigo, opcoes[i].nome);
        printf("------------------------------------------\n");

        escolha = lerInt("Escolha uma opcao: ");
        if (opcaoExiste(opcoes, n, escolha))
            return escolha;

        printf("Opcao invalida! Tente novamente.\n");
        pausa();
    }
}

/* ================================================================== */
/* USUARIOS (cadastro usuario)                                         */
/* ================================================================== */

/* Cadastra um usuario. Serve tanto para o primeiro usuario quanto para os
   seguintes (as checagens de nome/conta repetidos so atuam se ja houver
   usuarios cadastrados). */
void cadastrarUsuario(void)
{
    int i, k;
    char entrada[64];
    int nomeRepetido;
    int contaRepetida;
    int senhaValida;
    int opcaoTipo;
    int primeiro = (qtdUsuarios == 0);

    if (qtdUsuarios >= MAX_USUARIOS) {
        printf("\nLimite maximo de usuarios atingido!\n");
        return;
    }

    do {
        nomeRepetido = 0;
        printf("\nNome de usuario: ");
        fgets(usuarios[qtdUsuarios].nome, TAM_TEXTO, stdin);
        usuarios[qtdUsuarios].nome[
            strcspn(usuarios[qtdUsuarios].nome, "\n")
        ] = '\0';

        if (strlen(usuarios[qtdUsuarios].nome) == 0) {
            printf("Erro: O nome nao pode ser vazio!\n");
            nomeRepetido = 1;
        } else {
            for (i = 0; i < qtdUsuarios; i++) {
                if (strcmp(usuarios[i].nome,
                           usuarios[qtdUsuarios].nome) == 0) {
                    printf("Erro: Nome de usuario ja existe!\n");
                    nomeRepetido = 1;
                    break;
                }
            }
        }
    } while (nomeRepetido == 1);

    printf("Quantidade de dinheiro total inicial (ex: 1000, 0 ou -50): R$ ");
    fgets(entrada, sizeof(entrada), stdin);
    for (k = 0; entrada[k] != '\0'; k++) {
        if (entrada[k] == ',') entrada[k] = '.';
    }
    usuarios[qtdUsuarios].conta.dinheiroInicial = atof(entrada);

    do {
        printf("Numero da agencia: ");
        fgets(entrada, sizeof(entrada), stdin);
        usuarios[qtdUsuarios].conta.agencia = atoi(entrada);
        if (usuarios[qtdUsuarios].conta.agencia <= 0) {
            printf("Erro: Informe uma agencia maior que zero!\n");
        }
    } while (usuarios[qtdUsuarios].conta.agencia <= 0);

    do {
        contaRepetida = 0;
        printf("Numero da conta bancaria: ");
        fgets(entrada, sizeof(entrada), stdin);
        usuarios[qtdUsuarios].conta.numeroConta = atoi(entrada);

        if (usuarios[qtdUsuarios].conta.numeroConta <= 0) {
            printf("Erro: O numero da conta deve ser maior que zero!\n");
            contaRepetida = 1;
        } else {
            for (i = 0; i < qtdUsuarios; i++) {
                if (usuarios[i].conta.numeroConta ==
                    usuarios[qtdUsuarios].conta.numeroConta) {
                    printf("Erro: Numero de conta ja existe!\n");
                    contaRepetida = 1;
                    break;
                }
            }
        }
    } while (contaRepetida == 1);

    do {
        printf("\nTipo de Conta Bancaria:\n");
        printf("1 - Conta Corrente\n");
        printf("2 - Conta Poupanca\n");
        printf("3 - Conta Salario\n");
        printf("Opcao: ");
        fgets(entrada, sizeof(entrada), stdin);
        opcaoTipo = atoi(entrada);

        if (opcaoTipo == 1) {
            strcpy(usuarios[qtdUsuarios].conta.tipo, "Corrente");
        } else if (opcaoTipo == 2) {
            strcpy(usuarios[qtdUsuarios].conta.tipo, "Poupanca");
        } else if (opcaoTipo == 3) {
            strcpy(usuarios[qtdUsuarios].conta.tipo, "Salario");
        } else {
            printf("Erro: Opcao invalida! Escolha 1, 2 ou 3.\n");
        }
    } while (opcaoTipo < 1 || opcaoTipo > 3);

    do {
        int temEspaco = 0;
        int qtdCaracteres = 0;
        senhaValida = 1;

        printf("Crie uma senha (minimo 4 caracteres, sem espacos): ");
        fgets(usuarios[qtdUsuarios].senha, 30, stdin);
        usuarios[qtdUsuarios].senha[
            strcspn(usuarios[qtdUsuarios].senha, "\n")
        ] = '\0';

        for (i = 0; usuarios[qtdUsuarios].senha[i] != '\0'; i++) {
            if (usuarios[qtdUsuarios].senha[i] == ' ') {
                temEspaco = 1;
            }
            qtdCaracteres++;
        }

        if (qtdCaracteres < 4) {
            printf("Erro: A senha deve ter pelo menos 4 caracteres!\n");
            senhaValida = 0;
        } else if (temEspaco == 1) {
            printf("Erro: A senha nao pode conter espacos!\n");
            senhaValida = 0;
        }
    } while (senhaValida == 0);

    usuarios[qtdUsuarios].qtdRegistros = 0;
    usuarios[qtdUsuarios].qtdInvestimentos = 0;
    usuarios[qtdUsuarios].qtdHistoricos = 0;
    qtdUsuarios++;

    if (primeiro)
        printf("\nPrimeiro usuario criado com sucesso!\n");
    else
        printf("\nUsuario cadastrado com sucesso!\n");
}

void removerUsuario(void)
{
    int i;
    char entrada[64];
    int idxRemover;
    char senhaConfirmacao[30];

    if (qtdUsuarios == 0) {
        printf("\nNenhum usuario para remover.\n");
        return;
    }

    printf("\n--- REMOVER USUARIO ---\n");
    for (i = 0; i < qtdUsuarios; i++) {
        printf("%d - %s\n", i + 1, usuarios[i].nome);
    }
    printf("0 - Cancelar\n");
    printf("\nEscolha o usuario que deseja remover: ");
    fgets(entrada, sizeof(entrada), stdin);
    idxRemover = atoi(entrada) - 1;

    if (idxRemover >= 0 && idxRemover < qtdUsuarios) {
        printf("Digite a senha do usuario '%s' para confirmar a remocao: ",
               usuarios[idxRemover].nome);
        fgets(senhaConfirmacao, 30, stdin);
        senhaConfirmacao[strcspn(senhaConfirmacao, "\n")] = '\0';

        if (strcmp(usuarios[idxRemover].senha, senhaConfirmacao) == 0) {
            for (i = idxRemover; i < qtdUsuarios - 1; i++) {
                usuarios[i] = usuarios[i + 1];
            }
            qtdUsuarios--;
            printf("\nUsuario removido com sucesso!\n");
        } else {
            printf("\nErro: Senha incorreta! O usuario nao foi removido.\n");
        }
    } else if (idxRemover != -1) {
        printf("\nUsuario invalido!\n");
    }
}

/* Devolve o indice do usuario logado, ou -1 se o login nao ocorrer */
int fazerLogin(void)
{
    int i;
    char entrada[64];
    int idxLogin;
    char senhaTentativa[30];

    printf("\n--- ESCOLHA O USUARIO PARA LOGIN ---\n");
    for (i = 0; i < qtdUsuarios; i++) {
        printf("%d - %s (Conta %s: %d)\n",
               i + 1,
               usuarios[i].nome,
               usuarios[i].conta.tipo,
               usuarios[i].conta.numeroConta);
    }
    printf("0 - Cancelar\n");
    printf("\nUsuario: ");
    fgets(entrada, sizeof(entrada), stdin);
    idxLogin = atoi(entrada) - 1;

    if (idxLogin >= 0 && idxLogin < qtdUsuarios) {
        printf("Digite a senha de '%s': ", usuarios[idxLogin].nome);
        fgets(senhaTentativa, 30, stdin);
        senhaTentativa[strcspn(senhaTentativa, "\n")] = '\0';

        if (strcmp(usuarios[idxLogin].senha, senhaTentativa) == 0) {
            printf("\nLogin realizado com sucesso! Bem-vindo(a), %s.\n",
                   usuarios[idxLogin].nome);
            return idxLogin;
        } else {
            printf("\nErro: Senha incorreta!\n");
        }
    } else if (idxLogin != -1) {
        printf("\nUsuario invalido!\n");
    }
    return -1;
}

/* ================================================================== */
/* MOVIMENTACOES / DESPESAS                                            */
/* ================================================================== */
void cadastrarMovimentacao(Usuario *u)
{
    int k, t;
    char entrada[64];

    if (u->qtdRegistros < MAX_REGISTROS) {

        printf("\nDescricao: ");
        fgets(u->registros[u->qtdRegistros].descricao,
              TAM_TEXTO, stdin);
        u->registros[u->qtdRegistros].descricao[
            strcspn(u->registros[u->qtdRegistros].descricao, "\n")
        ] = '\0';

        printf("Tipo (Entrada ou Saida): ");
        fgets(u->registros[u->qtdRegistros].tipo, 20, stdin);
        u->registros[u->qtdRegistros].tipo[
            strcspn(u->registros[u->qtdRegistros].tipo, "\n")
        ] = '\0';

        t = (int)strlen(u->registros[u->qtdRegistros].tipo);
        while (t > 0 && u->registros[u->qtdRegistros].tipo[t - 1] == ' ') {
            u->registros[u->qtdRegistros].tipo[t - 1] = '\0';
            t--;
        }

        for (k = 0; u->registros[u->qtdRegistros].tipo[k] != '\0'; k++) {
            if (u->registros[u->qtdRegistros].tipo[k] >= 'A' &&
                u->registros[u->qtdRegistros].tipo[k] <= 'Z') {
                u->registros[u->qtdRegistros].tipo[k] += 32;
            }
        }

        printf("Status (Pago, Pendente etc.): ");
        fgets(u->registros[u->qtdRegistros].status, 30, stdin);
        u->registros[u->qtdRegistros].status[
            strcspn(u->registros[u->qtdRegistros].status, "\n")
        ] = '\0';

        printf("Valor: R$ ");
        fgets(entrada, sizeof(entrada), stdin);
        for (k = 0; entrada[k] != '\0'; k++) {
            if (entrada[k] == ',') entrada[k] = '.';
        }
        u->registros[u->qtdRegistros].valor = atof(entrada);

        u->qtdRegistros++;

        printf("\nMovimentacao cadastrada!\n");

    } else {
        printf("\nLimite de registros atingido!\n");
    }
}

void exibirMovimentacoes(Usuario *u)
{
    int i;
    if (u->qtdRegistros == 0) {
        printf("\nNenhuma movimentacao cadastrada.\n");
    } else {
        printf("\n===== MOVIMENTACOES DE %s =====\n", u->nome);

        for (i = 0; i < u->qtdRegistros; i++) {
            printf("\nRegistro %d\n", i + 1);
            printf("Descricao: %s\n", u->registros[i].descricao);
            printf("Tipo: %s\n", u->registros[i].tipo);
            printf("Status: %s\n", u->registros[i].status);
            printf("Valor: R$ %.2f\n", u->registros[i].valor);
        }
    }
}

/* ATENCAO: o codigo enviado pelo grupo NAO tem "pagar despesa", "prazo de
   pagamento" nem "data de pagamento" (cartoes da Givercy). Esta funcao e
   so um ponto de ligacao: quando essa parte ficar pronta, e so colocar o
   codigo aqui dentro. */
void pagarDespesa(Usuario *u)
{
    (void)u;
    printf("\n[Pagamento de despesa - ainda nao implementado pelo grupo]\n");
}

/* ================================================================== */
/* INVESTIMENTOS                                                       */
/* ================================================================== */
void cadastrarInvestimento(Usuario *u)
{
    int i, k;
    char entrada[64];

    if (u->qtdInvestimentos < MAX_INVESTIMENTOS) {
        int invalido;

        do {
            invalido = 0;
            printf("\nNome do investimento: ");
            fgets(u->investimentos[u->qtdInvestimentos].nome,
                  TAM_TEXTO, stdin);
            u->investimentos[u->qtdInvestimentos].nome[
                strcspn(u->investimentos[u->qtdInvestimentos].nome, "\n")
            ] = '\0';

            if (strlen(u->investimentos[u->qtdInvestimentos].nome) == 0) {
                printf("Erro: O nome nao pode ser vazio!\n");
                invalido = 1;
            } else {
                for (i = 0; i < u->qtdInvestimentos; i++) {
                    if (strcmp(u->investimentos[i].nome,
                               u->investimentos[u->qtdInvestimentos].nome) == 0) {
                        printf("Erro: Investimento ja cadastrado!\n");
                        invalido = 1;
                        break;
                    }
                }
            }
        } while (invalido == 1);

        do {
            printf("Tipo do investimento (ex: CDB, Tesouro, Acoes): ");
            fgets(u->investimentos[u->qtdInvestimentos].tipo, 30, stdin);
            u->investimentos[u->qtdInvestimentos].tipo[
                strcspn(u->investimentos[u->qtdInvestimentos].tipo, "\n")
            ] = '\0';

            if (strlen(u->investimentos[u->qtdInvestimentos].tipo) == 0) {
                printf("Erro: Informe o tipo do investimento!\n");
            }
        } while (strlen(u->investimentos[u->qtdInvestimentos].tipo) == 0);

        do {
            printf("Valor inicial: R$ ");
            fgets(entrada, sizeof(entrada), stdin);
            for (k = 0; entrada[k] != '\0'; k++) {
                if (entrada[k] == ',') entrada[k] = '.';
            }
            u->investimentos[u->qtdInvestimentos].valorInicial = atof(entrada);

            if (u->investimentos[u->qtdInvestimentos].valorInicial <= 0) {
                printf("Erro: O valor inicial deve ser maior que zero!\n");
            }
        } while (u->investimentos[u->qtdInvestimentos].valorInicial <= 0);

        do {
            printf("Valor atual: R$ ");
            fgets(entrada, sizeof(entrada), stdin);
            for (k = 0; entrada[k] != '\0'; k++) {
                if (entrada[k] == ',') entrada[k] = '.';
            }
            u->investimentos[u->qtdInvestimentos].valorAtual = atof(entrada);

            if (u->investimentos[u->qtdInvestimentos].valorAtual < 0) {
                printf("Erro: O valor atual nao pode ser negativo!\n");
            }
        } while (u->investimentos[u->qtdInvestimentos].valorAtual < 0);

        u->investimentos[u->qtdInvestimentos].rendimento = 0;
        strcpy(u->investimentos[u->qtdInvestimentos].status, "Ativo");

        u->qtdInvestimentos++;

        printf("\nInvestimento cadastrado!\n");

    } else {
        printf("\nLimite de investimentos atingido!\n");
    }
}

void calcularRendimentos(Usuario *u)
{
    int i;
    if (u->qtdInvestimentos == 0) {
        printf("\nNenhum investimento cadastrado.\n");
    } else {
        printf("\n===== RENDIMENTOS (%s) =====\n", u->nome);

        for (i = 0; i < u->qtdInvestimentos; i++) {
            u->investimentos[i].rendimento =
                u->investimentos[i].valorAtual -
                u->investimentos[i].valorInicial;

            printf("\nInvestimento: %s\n", u->investimentos[i].nome);
            printf("Valor inicial: R$ %.2f\n", u->investimentos[i].valorInicial);
            printf("Valor atual: R$ %.2f\n", u->investimentos[i].valorAtual);
            printf("Rendimento: R$ %.2f\n", u->investimentos[i].rendimento);
        }
    }
}

void exibirInvestimentos(Usuario *u)
{
    int i;
    if (u->qtdInvestimentos == 0) {
        printf("\nNenhum investimento cadastrado.\n");
    } else {
        printf("\n===== EXIBIR INVESTIMENTOS (%s) =====\n", u->nome);
        for (i = 0; i < u->qtdInvestimentos; i++) {
            u->investimentos[i].rendimento =
                u->investimentos[i].valorAtual -
                u->investimentos[i].valorInicial;

            printf("\nInvestimento %d\n", i + 1);
            printf("Nome: %s\n", u->investimentos[i].nome);
            printf("Tipo: %s\n", u->investimentos[i].tipo);
            printf("Status: %s\n", u->investimentos[i].status);
            printf("Valor Inicial: R$ %.2f\n", u->investimentos[i].valorInicial);
            printf("Valor Atual: R$ %.2f\n", u->investimentos[i].valorAtual);
            printf("Rendimento: R$ %.2f\n", u->investimentos[i].rendimento);
        }
    }
}

void evolucaoInvestimento(Usuario *u)
{
    int i, k;
    double somaEvolucao;
    char entrada[64];

    if (u->qtdInvestimentos == 0) {
        printf("\nNenhum investimento cadastrado.\n");
    } else if (u->qtdHistoricos >= MAX_HISTORICOS) {
        printf("\nLimite de registros de historico atingido!\n");
    } else {
        int idx;
        double taxa;

        printf("\n===== ACAO DO INVESTIMENTO NO TEMPO =====\n");
        for (i = 0; i < u->qtdInvestimentos; i++) {
            printf("%d - %s | Tipo: %s | Valor Atual: R$ %.2f | Status: %s\n",
                   i + 1,
                   u->investimentos[i].nome,
                   u->investimentos[i].tipo,
                   u->investimentos[i].valorAtual,
                   u->investimentos[i].status);
        }

        printf("\nEscolha o investimento (1 a %d): ", u->qtdInvestimentos);
        fgets(entrada, sizeof(entrada), stdin);
        idx = atoi(entrada) - 1;

        if (idx >= 0 && idx < u->qtdInvestimentos) {
            do {
                printf("Dia da atualizacao (1 a 31): ");
                fgets(entrada, sizeof(entrada), stdin);
                u->historicos[u->qtdHistoricos].periodo.dia = atoi(entrada);
            } while (u->historicos[u->qtdHistoricos].periodo.dia < 1 ||
                     u->historicos[u->qtdHistoricos].periodo.dia > 31);

            do {
                printf("Mes da atualizacao (1 a 12): ");
                fgets(entrada, sizeof(entrada), stdin);
                u->historicos[u->qtdHistoricos].periodo.mes = atoi(entrada);
            } while (u->historicos[u->qtdHistoricos].periodo.mes < 1 ||
                     u->historicos[u->qtdHistoricos].periodo.mes > 12);

            do {
                printf("Ano da atualizacao (ex: 2026): ");
                fgets(entrada, sizeof(entrada), stdin);
                u->historicos[u->qtdHistoricos].periodo.ano = atoi(entrada);
            } while (u->historicos[u->qtdHistoricos].periodo.ano < 1900);

            printf("Taxa de variacao no periodo (em %%): ");
            fgets(entrada, sizeof(entrada), stdin);
            for (k = 0; entrada[k] != '\0'; k++) {
                if (entrada[k] == ',') entrada[k] = '.';
            }
            taxa = atof(entrada);

            strcpy(u->historicos[u->qtdHistoricos].nomeInvestimento,
                   u->investimentos[idx].nome);
            u->historicos[u->qtdHistoricos].valorAnterior =
                u->investimentos[idx].valorAtual;
            u->historicos[u->qtdHistoricos].evolucao =
                u->investimentos[idx].valorAtual * (taxa / 100.0);
            u->historicos[u->qtdHistoricos].valorAtualizado =
                u->historicos[u->qtdHistoricos].valorAnterior +
                u->historicos[u->qtdHistoricos].evolucao;

            u->investimentos[idx].valorAtual =
                u->historicos[u->qtdHistoricos].valorAtualizado;
            u->investimentos[idx].rendimento =
                u->investimentos[idx].valorAtual -
                u->investimentos[idx].valorInicial;

            u->qtdHistoricos++;

            printf("\n--- HISTORICO DE EVOLUCAO DOS INVESTIMENTOS ---\n");
            somaEvolucao = 0;
            for (i = 0; i < u->qtdHistoricos; i++) {
                printf("Data: %02d/%02d/%04d | Investimento: %s | Antes: R$ %.2f | Depois: R$ %.2f | Evolucao: R$ %.2f\n",
                       u->historicos[i].periodo.dia,
                       u->historicos[i].periodo.mes,
                       u->historicos[i].periodo.ano,
                       u->historicos[i].nomeInvestimento,
                       u->historicos[i].valorAnterior,
                       u->historicos[i].valorAtualizado,
                       u->historicos[i].evolucao);
                somaEvolucao = somaEvolucao + u->historicos[i].evolucao;
            }
            printf("Evolucao total acumulada: R$ %.2f\n", somaEvolucao);

        } else {
            printf("\nInvestimento invalido!\n");
        }
    }
}

void sacarInvestimento(Usuario *u)
{
    int i, k;
    char entrada[64];

    if (u->qtdInvestimentos == 0) {
        printf("\nNenhum investimento cadastrado.\n");
    } else {
        int idx;
        double valorSaque;

        printf("\n===== REMOVER / SACAR INVESTIMENTO =====\n");
        for (i = 0; i < u->qtdInvestimentos; i++) {
            printf("%d - %s | Tipo: %s | Valor Atual: R$ %.2f | Status: %s\n",
                   i + 1,
                   u->investimentos[i].nome,
                   u->investimentos[i].tipo,
                   u->investimentos[i].valorAtual,
                   u->investimentos[i].status);
        }

        printf("\nEscolha o investimento para sacar (1 a %d): ", u->qtdInvestimentos);
        fgets(entrada, sizeof(entrada), stdin);
        idx = atoi(entrada) - 1;

        if (idx >= 0 && idx < u->qtdInvestimentos) {
            if (strcmp(u->investimentos[idx].status, "Resgatado") == 0 ||
                u->investimentos[idx].valorAtual <= 0) {
                printf("\nErro: Este investimento ja foi totalmente resgatado/removido!\n");
            } else {
                printf("Informe o valor que deseja sacar (Maximo R$ %.2f): R$ ",
                       u->investimentos[idx].valorAtual);
                fgets(entrada, sizeof(entrada), stdin);
                for (k = 0; entrada[k] != '\0'; k++) {
                    if (entrada[k] == ',') entrada[k] = '.';
                }
                valorSaque = atof(entrada);

                if (valorSaque <= 0) {
                    printf("\nErro: O valor do saque deve ser maior que zero!\n");
                } else if (valorSaque > u->investimentos[idx].valorAtual) {
                    printf("\nErro: Saldo insuficiente no investimento para esse saque!\n");
                } else {
                    u->investimentos[idx].valorAtual =
                        u->investimentos[idx].valorAtual - valorSaque;

                    if (u->investimentos[idx].valorAtual == 0) {
                        strcpy(u->investimentos[idx].status, "Resgatado");
                    } else {
                        strcpy(u->investimentos[idx].status, "Saque Parcial");
                    }

                    if (u->qtdRegistros < MAX_REGISTROS) {
                        strcpy(u->registros[u->qtdRegistros].descricao, "Saque Investimento: ");
                        strcat(u->registros[u->qtdRegistros].descricao, u->investimentos[idx].nome);
                        strcpy(u->registros[u->qtdRegistros].tipo, "entrada");
                        strcpy(u->registros[u->qtdRegistros].status, "Pago");
                        u->registros[u->qtdRegistros].valor = valorSaque;
                        u->qtdRegistros++;
                    }

                    printf("\nSaque de R$ %.2f realizado com sucesso!\n", valorSaque);
                    printf("Investimento: %s | Novo Valor Atual: R$ %.2f | Novo Status: %s\n",
                           u->investimentos[idx].nome,
                           u->investimentos[idx].valorAtual,
                           u->investimentos[idx].status);
                }
            }
        } else {
            printf("\nOpcao de investimento invalida!\n");
        }
    }
}

/* ================================================================== */
/* RELATORIO BANCARIO                                                  */
/* ================================================================== */
void resumoBancario(Usuario *u)
{
    int i;
    double totalEntradas = 0;
    double totalSaidas = 0;
    double saldo;

    for (i = 0; i < u->qtdRegistros; i++) {
        if (strcmp(u->registros[i].tipo, "entrada") == 0) {
            totalEntradas += u->registros[i].valor;
        } else if (strcmp(u->registros[i].tipo, "saida") == 0) {
            totalSaidas += u->registros[i].valor;
        }
    }

    saldo = u->conta.dinheiroInicial + totalEntradas - totalSaidas;

    printf("\n===== RESUMO BANCARIO (%s) =====\n", u->nome);
    printf("Dinheiro inicial da conta: R$ %.2f\n", u->conta.dinheiroInicial);
    printf("Total de entradas: R$ %.2f\n", totalEntradas);
    printf("Total de saidas: R$ %.2f\n", totalSaidas);
    printf("Saldo atual: R$ %.2f\n", saldo);
}

void gerarRelatorioTexto(Usuario *u)
{
    int i, tamanho;
    int posicao;
    double totalEntradas;
    double totalSaidas;
    double saldo;

    relatorio[0] = '\0';
    posicao = 0;

    posicao += snprintf(relatorio + posicao,
                        TAM_RELATORIO - posicao,
                        "===== RELATORIO BANCARIO DO USUARIO: %s =====\n"
                        "Conta %s: %d | Agencia: %d | Dinheiro Inicial: R$ %.2f\n\n",
                        u->nome, u->conta.tipo, u->conta.numeroConta,
                        u->conta.agencia, u->conta.dinheiroInicial);

    posicao += snprintf(relatorio + posicao,
                        TAM_RELATORIO - posicao,
                        "--- MOVIMENTACOES ---\n");

    for (i = 0; i < u->qtdRegistros; i++) {
        tamanho = snprintf(
            relatorio + posicao,
            TAM_RELATORIO - posicao,
            "Descricao: %s | Tipo: %s | Status: %s | Valor: R$ %.2f\n",
            u->registros[i].descricao,
            u->registros[i].tipo,
            u->registros[i].status,
            u->registros[i].valor
        );

        if (tamanho < 0 || tamanho >= TAM_RELATORIO - posicao) {
            break;
        }
        posicao += tamanho;
    }

    if (posicao < TAM_RELATORIO - 1) {
        posicao += snprintf(relatorio + posicao,
                            TAM_RELATORIO - posicao,
                            "\n--- INVESTIMENTOS ---\n");
    }

    for (i = 0; i < u->qtdInvestimentos; i++) {
        u->investimentos[i].rendimento =
            u->investimentos[i].valorAtual -
            u->investimentos[i].valorInicial;

        tamanho = snprintf(
            relatorio + posicao,
            TAM_RELATORIO - posicao,
            "Nome: %s | Tipo: %s | Status: %s | Inicial: R$ %.2f | Atual: R$ %.2f | Rendimento: R$ %.2f\n",
            u->investimentos[i].nome,
            u->investimentos[i].tipo,
            u->investimentos[i].status,
            u->investimentos[i].valorInicial,
            u->investimentos[i].valorAtual,
            u->investimentos[i].rendimento
        );

        if (tamanho < 0 || tamanho >= TAM_RELATORIO - posicao) {
            break;
        }
        posicao += tamanho;
    }

    totalEntradas = 0;
    totalSaidas = 0;
    for (i = 0; i < u->qtdRegistros; i++) {
        if (strcmp(u->registros[i].tipo, "entrada") == 0)
            totalEntradas += u->registros[i].valor;
        else if (strcmp(u->registros[i].tipo, "saida") == 0)
            totalSaidas += u->registros[i].valor;
    }
    saldo = u->conta.dinheiroInicial + totalEntradas - totalSaidas;

    if (posicao < TAM_RELATORIO - 1) {
        posicao += snprintf(relatorio + posicao,
                            TAM_RELATORIO - posicao,
                            "\n--- RESUMO ---\nDinheiro Inicial: R$ %.2f | Entradas: R$ %.2f | Saidas: R$ %.2f | Saldo Final: R$ %.2f\n",
                            u->conta.dinheiroInicial, totalEntradas, totalSaidas, saldo);
    }

    printf("\n%s", relatorio);
}

/* Relatorio geral oferecido ao sair do programa (igual ao original) */
void relatorioGeralEncerramento(void)
{
    int uIdx, i;
    char entrada[64];
    int gerarAntesDeSair;
    int posicao;
    double totalEntradas;
    double totalSaidas;
    double saldo;

    if (qtdUsuarios <= 0)
        return;

    printf("\nDeseja gerar um relatorio geral antes de fechar? (1 - Sim / 2 - Nao): ");
    fgets(entrada, sizeof(entrada), stdin);
    gerarAntesDeSair = atoi(entrada);

    if (gerarAntesDeSair == 1) {
        relatorio[0] = '\0';
        posicao = 0;

        posicao += snprintf(relatorio + posicao,
                            TAM_RELATORIO - posicao,
                            "\n========== RELATORIO GERAL DE ENCERRAMENTO ==========\n");

        for (uIdx = 0; uIdx < qtdUsuarios; uIdx++) {
            Usuario *u = &usuarios[uIdx];
            totalEntradas = 0;
            totalSaidas = 0;

            for (i = 0; i < u->qtdRegistros; i++) {
                if (strcmp(u->registros[i].tipo, "entrada") == 0)
                    totalEntradas += u->registros[i].valor;
                else if (strcmp(u->registros[i].tipo, "saida") == 0)
                    totalSaidas += u->registros[i].valor;
            }
            saldo = u->conta.dinheiroInicial + totalEntradas - totalSaidas;

            posicao += snprintf(relatorio + posicao,
                                TAM_RELATORIO - posicao,
                                "\nUSUARIO %d: %s | Conta %s: %d (Ag: %d)\n"
                                "Dinheiro Inicial: R$ %.2f | Entradas: R$ %.2f | Saidas: R$ %.2f | Saldo Final: R$ %.2f\n",
                                uIdx + 1, u->nome, u->conta.tipo, u->conta.numeroConta,
                                u->conta.agencia, u->conta.dinheiroInicial,
                                totalEntradas, totalSaidas, saldo);

            for (i = 0; i < u->qtdInvestimentos; i++) {
                u->investimentos[i].rendimento =
                    u->investimentos[i].valorAtual -
                    u->investimentos[i].valorInicial;

                posicao += snprintf(relatorio + posicao,
                                    TAM_RELATORIO - posicao,
                                    "   [Investimento] %s (%s) | Status: %s | Atual: R$ %.2f | Rendimento: R$ %.2f\n",
                                    u->investimentos[i].nome,
                                    u->investimentos[i].tipo,
                                    u->investimentos[i].status,
                                    u->investimentos[i].valorAtual,
                                    u->investimentos[i].rendimento);
            }
        }

        printf("%s\n", relatorio);
    }
}

/* ================================================================== */
/* MENUS                                                               */
/* ================================================================== */
void menuInvestimentos(Usuario *u)
{
    struct MenuOpcao opcoes[] = {
        {1, "Cadastrar investimento"},
        {2, "Exibir investimentos detalhados"},
        {3, "Calcular rendimentos"},
        {4, "Evolucao do investimento no tempo"},
        {5, "Remover / sacar investimento"},
        {0, "Voltar"}
    };
    int n = sizeof opcoes / sizeof opcoes[0];
    int escolha;

    do {
        escolha = escolherOpcao("GERENCIAMENTO DE INVESTIMENTOS", "", opcoes, n);
        if (escolha == 1) {
            cadastrarInvestimento(u);
            pausa();
        } else if (escolha == 2) {
            exibirInvestimentos(u);
            pausa();
        } else if (escolha == 3) {
            calcularRendimentos(u);
            pausa();
        } else if (escolha == 4) {
            evolucaoInvestimento(u);
            pausa();
        } else if (escolha == 5) {
            sacarInvestimento(u);
            pausa();
        }
    } while (escolha != 0);
}

void menuDespesas(Usuario *u)
{
    struct MenuOpcao opcoes[] = {
        {1, "Cadastrar movimentacao / despesa"},
        {2, "Exibir movimentacoes"},
        {3, "Pagar despesa"},
        {0, "Voltar"}
    };
    int n = sizeof opcoes / sizeof opcoes[0];
    int escolha;

    do {
        escolha = escolherOpcao("GERENCIAMENTO DE DESPESAS", "", opcoes, n);
        if (escolha == 1) {
            cadastrarMovimentacao(u);
            pausa();
        } else if (escolha == 2) {
            exibirMovimentacoes(u);
            pausa();
        } else if (escolha == 3) {
            pagarDespesa(u);
            pausa();
        }
    } while (escolha != 0);
}

void menuRelatorio(Usuario *u)
{
    struct MenuOpcao opcoes[] = {
        {1, "Resumo bancario"},
        {2, "Relatorio completo em texto"},
        {0, "Voltar"}
    };
    int n = sizeof opcoes / sizeof opcoes[0];
    int escolha;

    do {
        escolha = escolherOpcao("RELATORIO BANCARIO", "", opcoes, n);
        if (escolha == 1) {
            resumoBancario(u);
            pausa();
        } else if (escolha == 2) {
            gerarRelatorioTexto(u);
            pausa();
        }
    } while (escolha != 0);
}

/* Area do usuario: so e aberta depois de um login correto */
void areaUsuario(int idx)
{
    Usuario *u = &usuarios[idx];
    struct MenuOpcao opcoes[] = {
        {1, "Gerenciar investimentos"},
        {2, "Gerenciar despesas"},
        {3, "Relatorio bancario"},
        {0, "Sair da conta (deslogar)"}
    };
    int n = sizeof opcoes / sizeof opcoes[0];
    int escolha;
    char titulo[TAM_TEXTO + 30];
    char info[120];

    sprintf(titulo, "AREA DO USUARIO - %s", u->nome);
    sprintf(info, "Conta %s: %d | Ag: %d", u->conta.tipo,
            u->conta.numeroConta, u->conta.agencia);

    do {
        escolha = escolherOpcao(titulo, info, opcoes, n);
        if (escolha == 1) {
            menuInvestimentos(u);
        } else if (escolha == 2) {
            menuDespesas(u);
        } else if (escolha == 3) {
            menuRelatorio(u);
        }
    } while (escolha != 0);

    printf("\nSaindo da conta de %s...\n", u->nome);
    pausa();
}

/* Configuracoes de usuarios (cadastrar / remover) */
void menuConfigUsuarios(void)
{
    struct MenuOpcao opcoes[] = {
        {1, "Cadastrar usuario"},
        {2, "Remover usuario"},
        {0, "Voltar"}
    };
    int n = sizeof opcoes / sizeof opcoes[0];
    int escolha;

    do {
        escolha = escolherOpcao("CONFIGURACOES DE USUARIOS", "", opcoes, n);
        if (escolha == 1) {
            cadastrarUsuario();
            pausa();
        } else if (escolha == 2) {
            removerUsuario();
            pausa();
            if (qtdUsuarios == 0)
                escolha = 0; /* sem usuarios: volta para a tela inicial */
        }
    } while (escolha != 0);
}

/* ================================================================== */
/* PROGRAMA PRINCIPAL: tela inicial (login ou cadastro)                */
/* ================================================================== */
int main(void)
{
    struct MenuOpcao opcoesPrimeiroAcesso[] = {
        {1, "Criar primeiro usuario"},
        {0, "Sair do programa"}
    };
    struct MenuOpcao opcoesInicial[] = {
        {1, "Login (escolher usuario)"},
        {2, "Configuracoes de usuarios (cadastrar / remover)"},
        {0, "Sair do programa"}
    };
    int nPrimeiro = sizeof opcoesPrimeiroAcesso / sizeof opcoesPrimeiroAcesso[0];
    int nInicial = sizeof opcoesInicial / sizeof opcoesInicial[0];
    int opcaoInicial;
    int idx;

    do {
        if (qtdUsuarios == 0) {
            opcaoInicial = escolherOpcao("BEM-VINDO",
                                         "Nenhum usuario encontrado no sistema!",
                                         opcoesPrimeiroAcesso, nPrimeiro);
            if (opcaoInicial == 1) {
                cadastrarUsuario();
                pausa();
            }
        } else {
            opcaoInicial = escolherOpcao("ABA DE USUARIOS", "",
                                         opcoesInicial, nInicial);
            if (opcaoInicial == 1) {
                idx = fazerLogin();
                if (idx != -1)
                    areaUsuario(idx);   /* area do usuario separada do login */
                else
                    pausa();
            } else if (opcaoInicial == 2) {
                menuConfigUsuarios();
            }
        }
    } while (opcaoInicial != 0);

    relatorioGeralEncerramento();
    printf("\nEncerrando o sistema...\n");
    return 0;
}
