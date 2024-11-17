#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CRIPTOMOEDAS 100
#define MAX_INVESTIDORES 100

// Estrutura das criptomoedas
typedef struct {
    char nome[50];
    float cotacao_inicial;
    float taxa_compra;
    float taxa_venda;
} Criptomoeda;

// Armazenar a quantidade de cada criptomoeda
typedef struct {
    float quantidade;
    char nome[50];
} QuantidadeCriptomoeda;

// Armazenar quantidades de criptomoedas
QuantidadeCriptomoeda saldo_criptomoedas[MAX_INVESTIDORES][MAX_CRIPTOMOEDAS];

Criptomoeda criptomoedas[MAX_CRIPTOMOEDAS];
int total_criptomoedas = 0;

// Estrutura para Investidores
typedef struct {
    char cpf[12];
    char nome[50];
    char senha[50];
    float saldo_reais;
    float saldo_bitcoin;
    float saldo_ethereum;
    float saldo_ripple;
} Investidor;

// Estrutura para armazenar cotações das criptomoedas
typedef struct {
    float bitcoin;
    float ethereum;
    float ripple;
} cotacao ;

Investidor investidores[MAX_INVESTIDORES];
int total_investidores = 0;

// Dados do administrador
const char cpf_gerente[] = "1";
const char senha_gerente[] = "a";

// Declaração das funções (ADMIN)
void cadastrar_investidor();
void excluir_investidor();
void menu_gerente();
void efetuar_login();
void cadastrar_criptomoedas();
void excluir_criptomoeda();

// Função para limpar o buffer
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// SAIR DO MODO ADMINISTRADOR
void sair_administrador() {
    char escolha;

    printf("Você deseja sair do modo administrador? (S/N): ");
    scanf(" %c", &escolha);

    if (escolha == 'S' || escolha == 's') {
        printf("Saindo do modo administrador...\n");
        efetuar_login(); // Chama a função de login após o administrador sair
    } else if (escolha == 'N' || escolha == 'n') {
        printf("Retornando ao modo administrador...\n");
    } else {
        printf("Opção inválida.\n");
    }
}

// CADASTRO DE NOVO INVESTIDOR
void cadastrar_investidor() {
    if (total_investidores >= MAX_INVESTIDORES) {
        printf("Não foi possível cadastrar novos investidores.\n");
        return;
    }

    Investidor novo_investidor;

    printf("Digite o nome do novo investidor: ");
    scanf(" %[^\n]%*c", novo_investidor.nome);
    printf("Digite o CPF do novo investidor: ");
    scanf("%s", novo_investidor.cpf);
    printf("Digite a senha do novo investidor: ");
    scanf("%s", novo_investidor.senha);
    
    novo_investidor.saldo_reais = 0.0;
    novo_investidor.saldo_bitcoin = 0.0;
    novo_investidor.saldo_ethereum = 0.0;
    novo_investidor.saldo_ripple = 0.0;

    investidores[total_investidores] = novo_investidor;
    total_investidores++;
    printf("Investidor cadastrado com sucesso.\n");
}

// EXCLUSÃO DE INVESTIDOR
void excluir_investidor() {
  char confirmacao;
  char cpf_excluir[12];
  printf("Digite o CPF do investidor a ser deletado: ");
  scanf("%s", cpf_excluir);

  int encontrado = -1;
  for (int i = 0; i < total_investidores; i++) {
    if (strcmp(investidores[i].cpf, cpf_excluir) == 0) {
      encontrado = i;
      break;
    }
  }

  if (encontrado == -1) {
    printf(" O Investidor não foi encontrado.\n");
    return;
  }

  printf("\nInvestidor encontrado:\n");
  printf("Nome: %s\n", investidores[encontrado].nome);
  printf("CPF: %s\n", investidores[encontrado].cpf);
  printf("Saldo em Reais: %.2f\n", investidores[encontrado].saldo_reais);
  printf("Saldo em Bitcoin: %.4f\n", investidores[encontrado].saldo_bitcoin);
  printf("Saldo em Ethereum: %.4f\n", investidores[encontrado].saldo_ethereum);
  printf("Saldo em Ripple: %.4f\n", investidores[encontrado].saldo_ripple);
  while (getchar() != '\n')
    ;

  printf("\nDeseja realmente excluir o investidor? (S/N): ");
  scanf("%c", &confirmacao);

  if (confirmacao == 'S' || confirmacao == 's') {
    for (int i = encontrado; i < total_investidores - 1; i++) {
      investidores[i] = investidores[i + 1];
    }
    total_investidores--;
    printf("Investidor excluído com sucesso.\n");
  } else if (confirmacao == 'N' || confirmacao == 'n') {
    printf("Exclusão cancelada.\n");
  } else {
    printf("Opção inválida.\n");
  }
}

// CADASTRO DE CRIPTOMOEDAS
void cadastrar_criptomoedas() {
    if (total_criptomoedas >= MAX_CRIPTOMOEDAS) {
        printf("Limite de cadastro de criptomoedas atingido.\n");
        return;
    }

    Criptomoeda nova;
    printf("Digite o nome da nova criptomoeda: ");
    scanf(" %[^\n]", nova.nome);
    printf("Digite a cotação inicial: ");
    scanf("%f", &nova.cotacao_inicial);
    printf("Digite a taxa de compra: ");
    scanf("%f", &nova.taxa_compra);
    printf("Digite a taxa de venda: ");
    scanf("%f", &nova.taxa_venda);

    criptomoedas[total_criptomoedas] = nova;
    total_criptomoedas++;

    printf("Criptomoeda cadastrada com sucesso!\n");
}

// EXCLUSÃO DE CRIPTOMOEDAS
void excluir_criptomoeda() {
    char nome_excluir[50];
    char confirmacao;

    printf("Digite o nome da criptomoeda a ser excluída: ");
    scanf(" %[^\n]", nome_excluir);

    int encontrado = -1;
    for (int i = 0; i < total_criptomoedas; i++) {
        if (strcmp(criptomoedas[i].nome, nome_excluir) == 0) {
            encontrado = i;
             break;
        }
    }

    if (encontrado == -1) {
        printf("Criptomoeda não encontrada.\n");
        return;
    }
    
    printf("\nCriptomoeda encontrada:\n");
    printf("Nome: %s\n", criptomoedas[encontrado].nome);
    printf("Cotação inicial: %.2f\n", criptomoedas[encontrado].cotacao_inicial);
    printf("Taxa de compra: %.2f\n", criptomoedas[encontrado].taxa_compra);
    printf("Taxa de venda: %.2f\n", criptomoedas[encontrado].taxa_venda);

    // Limpa buffer antes de ler confirmação
    clear_buffer();
    printf("\nDeseja realmente excluir a criptomoeda? (S/N): ");
    scanf("%c", &confirmacao);

    if (confirmacao == 'S' || confirmacao == 's') {
        for (int i = encontrado; i < total_criptomoedas - 1; i++) {
            criptomoedas[i] = criptomoedas[i + 1];
        }
        total_criptomoedas--;
        printf("Criptomoeda excluída com sucesso.\n");
    } else if (confirmacao == 'N' || confirmacao == 'n') {
        printf("Exclusão cancelada.\n");
    } else {
        printf("Opção inválida.\n");
    }
}

// CONSULTA DE SALDO (ADMIN)
void consultar_saldo_gerente() {
    char cpf_consultar[12];
    printf("Digite o CPF do investidor: ");
    scanf("%s", cpf_consultar);

    int encontrado = -1;
    for (int i = 0; i < total_investidores; i++) {
        if (strcmp(investidores[i].cpf, cpf_consultar) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Investidor não encontrado.\n");
        return;
    }

    printf("\nSaldo do Investidor:\n");
    printf("CPF: %s\n", investidores[encontrado].cpf);
    printf("Saldo em Reais: %.2f\n", investidores[encontrado].saldo_reais);
    printf("Saldo em Bitcoin: %.4f\n", investidores[encontrado].saldo_bitcoin);
    printf("Saldo em Ethereum: %.4f\n", investidores[encontrado].saldo_ethereum);
    printf("Saldo em Ripple: %.4f\n", investidores[encontrado].saldo_ripple);
}

// Menu ADM
void menu_gerente() {
    int opcao;
    while (1) {
        printf("==----------------------------==\n");
        printf("Menu: \n");
        printf("1. Cadastrar novo investidor\n");
        printf("2. Excluir investidor\n");
        printf("3. Cadastrar criptomoedas\n");
        printf("4. Excluir criptomoedas\n");
        printf("5. Consultar saldo\n");
        printf("6. Sair\n");
        printf("==----------------------------==\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
    
        switch (opcao) {
            case 1:
                cadastrar_investidor();
                break;
            case 2:
                excluir_investidor();
                break;
            case 3:
                cadastrar_criptomoedas();
                break;
            case 4:
                excluir_criptomoeda();
                break;
            case 5:
                consultar_saldo_gerente();
                break;
            case 6:
                sair_administrador();
                break;
            default:
                printf("Opção inválida.\n");
        }
    }
}

// Declaração das funções (INVESTIDOR)
void consultar_saldo_investidor();
void menu_completo(int cliente);
void efetuar_login();
void extrato(int cliente);
void depositar(int cliente);
void sacar(int cliente);
void encerrar_programa();
void carregar_saldos();
void salvar_saldos();
void salvar_dados(int cliente, char operacao, float valor, const char* qual_moeda, const char* cotacao, float valor_cotacao, const char* taxa, float valor_taxa, float valor_reais, float valor_bitcoin, float valor_ethereum, float valor_ripple);
void ler_dados(int cliente);
void comprar_criptomoedas(int cliente, cotacao* cotacoes);
void vender_criptomoedas(int cliente, cotacao* cotacoes);
void atualizar_cotacao();

void consultar_saldo_investidor(Investidor *cliente) {
    char senha_acesso_saldo[50]; 

    // Laço de validação
    while (1) {
        printf("==----------------------------==\n");
        printf("Digite sua senha: ");
        scanf("%s", senha_acesso_saldo);
        if (strcmp(cliente->senha, senha_acesso_saldo) == 0) {
            printf("==----------------------------==\n");
            printf("Nome: %s\n", cliente->nome);
            printf("CPF: %c%c%c.%c%c%c.%c%c%c-%c%c\n", // Formatação do CPF
                cliente->cpf[0], cliente->cpf[1], cliente->cpf[2],
                cliente->cpf[3], cliente->cpf[4], cliente->cpf[5],
                cliente->cpf[6], cliente->cpf[7], cliente->cpf[8],
                cliente->cpf[9], cliente->cpf[10]);
            printf("\nReais (R$): %.2f\n", cliente->saldo_reais);
            printf("Bitcoin (BTC): %.6f\n", cliente->saldo_bitcoin);
            printf("Ethereum (ETH): %.6f\n", cliente->saldo_ethereum);
            printf("Ripple (XRP): %.6f\n", cliente->saldo_ripple);
            return;
        } else {
            printf("Senha incorreta. Digite novamente.\n");
        }
    }
}

// Função de login
void efetuar_login() {
  char cpf_login[12];
  char senha_login[7];
  printf("Digite seu CPF: ");
  scanf("%s", cpf_login);
  printf("Digite sua senha: ");
  scanf("%s", senha_login);

  // Verifica login administrador
  if (strcmp(cpf_login, cpf_gerente) == 0 &&
      strcmp(senha_login, senha_gerente) == 0) {
    printf("Acesso concedido ao menu de gerenciamento.\n");
    menu_gerente();
    return;
  }

  // Verifica login investidores
  for (int i = 0; i < total_investidores; i++) {
    if (strcmp(investidores[i].cpf, cpf_login) == 0 &&
        strcmp(investidores[i].senha, senha_login) == 0) {
      printf("Bem-vindo, %s!\n", investidores[i].nome);
      menu_completo(i);
      return;
    }
  }

  printf("CPF ou senha inválidos.\n");
}


// Núcleo da lógica de interação (investidor)
void menu_completo(int cliente) {
    int opcao_menu;

    // Laço infinito até que o cliente decida deslogar
    while (1) {
        printf("==----------------------------==\n");
        printf("Menu: \n");
        printf("1. Consultar saldo\n");
        printf("2. Consultar extrato\n");
        printf("3. Depositar\n");
        printf("4. Sacar\n");
        printf("5. Comprar criptomoedas\n");
        printf("6. Vender criptomoedas\n");
        printf("7. Atualizar cotação\n");
        printf("8. Sair\n");
        printf("==----------------------------==\n");
        printf("Escolha sua opção: ");
        scanf("%d", &opcao_menu);

        // Executa a operação escolhida
        switch (opcao_menu) {
            case 1:
                consultar_saldo_investidor(&cliente);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                return; // Sai do laço e retorna à função 'efetuar_login'
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

// Função principal
int main() {
  while (1) {
    efetuar_login();
    int opcao;
    printf("Deseja fazer um novo login? (1. Sim / 2. Não): ");
    scanf("%d", &opcao);
    if (opcao == 2) {
      printf("==----------------------------==\n");
      break;
    }
  }
  return 0;
}
