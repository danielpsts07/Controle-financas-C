#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

int main(){
    setlocale(LC_ALL,"Portuguese_Brazil");

    int navegador;
    float receita,despesa,totDespesa = 0,totReceitas = 0,saldo;
    char resp;

    while (true)
    {
        printf("=============Menu=============\n");
        printf("Escolha uma opção: \n");
        printf("1 - Cadastrar Receita \n");
        printf("2 - Cadastra Despesa \n");
        printf("3 - Calcular Total de Receitas \n");
        printf("4 - Calcular Total de Despesas \n");
        printf("5 - Calcular Saldo \n");
        printf("6 - Sair \n");
        printf("==============================\n");
        scanf("%d",&navegador);

        switch (navegador)
        {
        case 1:
            while (true)
            {
                printf("Digite o valor que deseja cadastrar: \n");
                scanf("%f",&receita);
                totReceitas += receita;

                printf("Deseja continuar? S/N\n");
                scanf(" %c",&resp);
                while (resp != 'S' && resp != 'N')
                {
                    printf("Responda apenas S / N!\n");
                    scanf(" %c",&resp);
                }
                if (resp == 'N')
                {
                    break;
                }

            }
            break; 

        case 2:
            while (true)
            {
                printf("Digite o valor despesa: \n");
                scanf("%f",&despesa);
                totDespesa += despesa;
                printf("Deseja continuar? S/N\n");
                scanf(" %c",&resp);
                while (resp != 'S' && resp != 'N')
                {
                    printf("Responda apenas S / N!\n");
                    scanf(" %c",&resp);
                }
                if (resp == 'N')
                {
                    break;
                }
                
            }
            
            break;

        case 3:
            printf("Seu total de receita é de : R$%.2f\n",totReceitas);
            break;
        
        case 4:
            printf("Seu total de despesa é de : R$%.2f\n",totDespesa);
            break;

        case 5:
            saldo = totReceitas - totDespesa;
            printf("Seu saldo atual é de : R$%.2f\n",saldo);
            break;
        
        }
    if (navegador == 6)
    {
        break;
    }
    
    }
}