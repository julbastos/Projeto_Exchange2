#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INVESTIDORES 50
#define MAX_CRIPTOMOEDAS 20

// Declaração das funções (ADMIN)
void cadastrar_investidor();
void excluir_investidor();
void menu_gerente();
void efetuar_login_admin();
void cadastrar_criptomoedas();
void excluir_criptomoeda();
void consultar_extrato();
void atualizar_cotacao();

// Dados do administrador
const char cpf_gerente[] = "1";
const char senha_gerente[] = "a";

// Estrutura para saldo de determinada criptomoeda
typedef struct {
  char nome[50];
  float saldo;
} Saldo_Cripto;

// Estrutura para Investidores
typedef struct {
  char cpf[11];
  char nome[50];
  char senha[50];
  float saldo_reais;
  Saldo_Cripto saldos[50]; // Lista de saldos das criptomoedas
  int total_saldos;        // Número de criptomoedas com saldos
} Investidor_admin;

Investidor_admin investidores[MAX_INVESTIDORES];
int total_investidores_admin = 0;

// Estrutura das criptomoedas
typedef struct {
  char nome[50];
  float cotacao_inicial;
  float taxa_compra;
  float taxa_venda;
} Criptomoeda;

Criptomoeda criptomoedas[MAX_CRIPTOMOEDAS];
int total_criptomoedas = 0;

// Função para limpar o buffer
void clear_buffer_admin() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

// Função para salvar os dados no arquivo após alteração
void salvar_dados_txt() {
  FILE *arquivo = fopen("investidores.txt", "a+");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para salvar os dados.\n");
    return;
  }
  // Salva os investidores
  fprintf(arquivo, "%d\n", total_investidores_admin); // Número de investidores
  for (int i = 0; i < total_investidores_admin; i++) {
    fprintf(arquivo, "%s %s %s %.2f %d\n", investidores[i].cpf,
            investidores[i].nome, investidores[i].senha,
            investidores[i].saldo_reais, investidores[i].total_saldos);

    for (int j = 0; j < investidores[i].total_saldos; j++) {
      fprintf(arquivo, "%s %.2f\n", investidores[i].saldos[j].nome,
              investidores[i].saldos[j].saldo);
    }
  }

  fclose(arquivo);
}

// Função para carregar os dados do arquivo de texto
void carregar_dados_txt() {
  FILE *arquivo = fopen("investidores.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para carregar os dados.\n");
    return;
  }

  fscanf(arquivo, "%d",
         &total_investidores_admin); // Carrega o número de investidores
  for (int i = 0; i < total_investidores_admin; i++) {
    fscanf(arquivo, "%s %s %s %f %d", investidores[i].cpf, investidores[i].nome,
           investidores[i].senha, &investidores[i].saldo_reais,
           &investidores[i].total_saldos);

    for (int j = 0; j < investidores[i].total_saldos; j++) {
      fscanf(arquivo, "%s %f", investidores[i].saldos[j].nome,
             &investidores[i].saldos[j].saldo);
    }
  }

  fclose(arquivo);
}

// CADASTRO DE NOVO INVESTIDOR
void cadastrar_investidor() {
  if (total_investidores_admin >= MAX_INVESTIDORES) {
    printf("Não foi possível cadastrar novos investidores.\n");
    return;
  }

  Investidor_admin novo_investidor;

  printf("Digite o nome do novo investidor: ");
  scanf(" %[^\n]%*c", novo_investidor.nome);
  printf("Digite o CPF do novo investidor: ");
  scanf("%s", novo_investidor.cpf);
  printf("Digite a senha do novo investidor: ");
  scanf("%s", novo_investidor.senha);

  // Inicializar saldos de criptomoedas com 0
  novo_investidor.total_saldos = 0;
  for (int i = 0; i < total_criptomoedas; i++) {
    strcpy(novo_investidor.saldos[i].nome, criptomoedas[i].nome);
    novo_investidor.saldos[i].saldo = 0.0;
    novo_investidor.total_saldos++;
  }

  investidores[total_investidores_admin] = novo_investidor;
  total_investidores_admin++;
  printf("Investidor cadastrado com sucesso.\n");

  // Salvar criação em binário
  salvar_dados_txt();
}

// EXCLUSÃO DE INVESTIDOR
void excluir_investidor() {
  char confirmacao;
  char cpf_excluir[11];
  printf("Digite o CPF do investidor a ser deletado: ");
  scanf("%s", cpf_excluir);
  clear_buffer_admin();

  int encontrado = -1;
  for (int i = 0; i < total_investidores_admin; i++) {
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
  printf("Saldos em Criptomoedas:\n");
  for (int j = 0; j < investidores[encontrado].total_saldos; j++) {
    printf("- %s: %.4f\n", investidores[encontrado].saldos[j].nome,
           investidores[encontrado].saldos[j].saldo);
  }

  printf("\nDeseja realmente excluir o investidor? (S/N): ");
  scanf("%c", &confirmacao);
  clear_buffer_admin();

  if (confirmacao == 'S' || confirmacao == 's') {
    for (int i = encontrado; i < total_investidores_admin - 1; i++) {
      investidores[i] = investidores[i + 1];
    }
    total_investidores_admin--;

    // Atualização do arquivo de texto
    salvar_dados_txt();

    printf("Investidor excluído com sucesso.\n");
  } else if (confirmacao == 'N' || confirmacao == 'n') {
    printf("Exclusão cancelada.\n");
  } else {
    printf("Opção inválida.\n");
  }
}

// Função para adicionar criptomoeda no struct do investidor
void adicionar_criptomoeda_investidores(const char *nome_criptomoeda) {
  for (int i = 0; i < total_investidores_admin; i++) {
    // Verifica se a criptomoeda já existe no saldo do investidor
    int ja_existe = 0;
    for (int j = 0; j < investidores[i].total_saldos; j++) {
      if (strcmp(investidores[i].saldos[j].nome, nome_criptomoeda) == 0) {
        ja_existe = 1;
        break;
      }
    }

    // Se não existe, adiciona a criptomoeda com saldo inicial 0
    if (!ja_existe) {
      strcpy(investidores[i].saldos[investidores[i].total_saldos].nome,
             nome_criptomoeda);
      investidores[i].saldos[investidores[i].total_saldos].saldo = 0.0;
      investidores[i].total_saldos++;
    }
  }
  printf("Criptomoeda '%s' adicionada aos investidores com saldo inicial 0.\n",
         nome_criptomoeda);
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

  // Atualizar saldos dos investidores com a nova criptomoeda
  adicionar_criptomoeda_investidores(nova.nome);

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
  clear_buffer_admin();
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
  char cpf_consultar[11];
  printf("Digite o CPF do investidor: ");
  scanf("%s", cpf_consultar);

  int encontrado = -1;
  for (int i = 0; i < total_investidores_admin; i++) {
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
  printf("Saldos em Criptomoedas:\n");
  for (int j = 0; j < investidores[encontrado].total_saldos; j++) {
    printf("- %s: %.4f\n", investidores[encontrado].saldos[j].nome,
           investidores[encontrado].saldos[j].saldo);
  }

  // FALTA PEGAR DADOS DO BINÁRIO
}

// FUNÇÃO DE CONSULTA DE SALDO DE UM INVESTIDOR
void consultar_extrato() {
  char cpf_consultar[11];
  printf("Digite o CPF do investidor para consultar o extrato: ");
  scanf("%s", cpf_consultar);

  int encontrado = -1;
  for (int i = 0; i < total_investidores_admin; i++) {
    if (strcmp(investidores[i].cpf, cpf_consultar) == 0) {
      encontrado = i;
      break;
    }
  }

  if (encontrado == -1) {
    printf("Investidor não encontrado.\n");
    return;
  }

  // Abre o arquivo de extrato para ler
  char nome_arquivo[30];
  sprintf(nome_arquivo, "extrato_%s.txt", cpf_consultar);
  FILE *arquivo_extrato = fopen(nome_arquivo, "r");
  if (arquivo_extrato == NULL) {
    printf("Não há extrato disponível para este investidor.\n");
    return;
  }

  printf("\nExtrato do investidor %s:\n", investidores[encontrado].nome);
  char linha[200];
  while (fgets(linha, sizeof(linha), arquivo_extrato)) {
    printf("%s", linha);
  }

  fclose(arquivo_extrato);
}

// FUNÇÃO DE ATUALIZAÇÃO DE COTAÇÃO DE CRIPTOMOEDA
void atualizar_cotacao() {
  char verifica;
  printf("Deseja atualizar a cotação das criptomoedas? (Y/N): ");
  scanf(" %c", &verifica);

  if (verifica == 'N' || verifica == 'n') {
    printf("Retornando ao menu...\n");
    return;
  } else if (verifica == 'Y' || verifica == 'y') {
    float percentual_var, fator_var;
    srand(time(NULL));

    printf("\n Atualizando Cotações \n");
    for (int i = 0; i < total_criptomoedas; i++) {
      percentual_var = ((rand() % 101) / 100.0) * 10.0 - 5.0;
      fator_var = 1 + (percentual_var / 100.0);

      // Atualiza a cotação da criptomoeda
      criptomoedas[i].cotacao_inicial *= fator_var;

      printf("%s: Nova cotação = %.2f\n", criptomoedas[i].nome,
             criptomoedas[i].cotacao_inicial);
    }
    printf("-- Cotações Atualizadas com Sucesso --\n");
  } else {
    printf("Por favor, tente novamente.\n");
  }
}

// SAIR DO MODO ADMINISTRADOR
void sair_administrador() {
  char escolha;

  printf("Você deseja sair do modo administrador? (S/N): ");
  scanf(" %c", &escolha);
  clear_buffer_admin();

  if (escolha == 'S' || escolha == 's') {
    printf("Saindo do modo administrador...\n");
  } else if (escolha == 'N' || escolha == 'n') {
    printf("Retornando ao modo administrador...\n");
  } else {
    printf("Opção inválida.\n");
  }
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
    printf("6. Consultar extrato de investidor\n");
    printf("7. Atualizar cotação de criptomoedas\n");
    printf("8. Sair\n");
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
      consultar_extrato();
      break;
    case 7:
      atualizar_cotacao();
      break;
    case 8:
      sair_administrador();
      return;
    default:
      printf("Opção inválida.\n");
    }
  }
}

// FUNÇÃO DE LOGIN
void efetuar_login_admin() {
    char cpf_login[11];
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


  printf("CPF ou senha inválidos.\n");
}

int logout() {
  while (1) {
    int opcao;
    printf("Deseja fazer um novo login? (1. Sim / 2. Não): ");
    scanf("%d", &opcao);
    clear_buffer_admin();
    if (opcao == 1) {
      efetuar_login_admin();
    } else if (opcao == 2) {
      printf("==----------------------------==\n");
      printf("\t\tAté mais!\n");
      printf("==----------------------------==\n");
      break;
    } else {
      printf("Opção inválida. Tente novamente\n");
    }
  }
  return 0;
}
