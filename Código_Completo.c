#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Caso necessário
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Matrizes de Strings
char cpfs[4][12] = {
    "12345678910",
    "22124085611",
    "52124009312",
    "22124071613"
};

char senhas[4][7] = {
    "123456",
    "234567",
    "345678",
    "456789"
};

char nomes[4][20] = {
    "Analiza Julia",
    "Ana Lima",
    "Elizabeth Kruger",
    "Julia Barreto"
};

// Armazenamento de saldos com valores iniciais zerados para cada cliente
float reais[4] = {0.00, 0.00, 0.00, 0.00};
float bitcoin[4] = {0.00, 0.00, 0.00, 0.00};
float ethereum[4] = {0.00, 0.00, 0.00, 0.00};
float ripple[4] = {0.00, 0.00, 0.00, 0.00};

// Estrutura para armazenar cotações das criptomoedas
typedef struct {
    float bitcoin;
    float ethereum;
    float ripple;
} cotacao ;

// Declaração das funções para reconhecimento
void consultar_saldo(int cliente);
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

// Início das implementações de cada código
void consultar_saldo(int cliente) {
    char senha_acesso_saldo[7]; // 6 dígitos + 1 para o caractere nulo

    // Laço de validação
    while (1) {
        printf("==----------------------------==\n");
        printf("Digite sua senha: ");
        scanf("%s", senha_acesso_saldo);
        if (strcmp(senhas[cliente], senha_acesso_saldo) == 0) {
            printf("==----------------------------==\n");
            printf("Nome: %s\n", nomes[cliente]);
            printf("CPF: %c%c%c.%c%c%c.%c%c%c-%c%c\n", // Formatação do CPF
                cpfs[cliente][0], cpfs[cliente][1], cpfs[cliente][2],
                cpfs[cliente][3], cpfs[cliente][4], cpfs[cliente][5],
                cpfs[cliente][6], cpfs[cliente][7], cpfs[cliente][8],
                cpfs[cliente][9], cpfs[cliente][10]);
            printf("\nReais (R$): %.2f\n", reais[cliente]);
            printf("Bitcoin (BTC): %.6f\n", bitcoin[cliente]);
            printf("Ethereum (ETH): %.6f\n", ethereum[cliente]);
            printf("Ripple (XRP): %.6f\n", ripple[cliente]);
            return;
        } else {
            printf("Senha incorreta. Digite novamente.\n");
        }
    }
}

void salvar_saldos() {
    FILE *arquivo = fopen("saldos.bin", "wb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo para salvar os saldos");
        return;
    }
    fwrite(reais, sizeof(float), 4, arquivo);
    fwrite(bitcoin, sizeof(float), 4, arquivo);
    fwrite(ethereum, sizeof(float), 4, arquivo);
    fwrite(ripple, sizeof(float), 4, arquivo);
    fclose(arquivo);
}

void carregar_saldos() {
    FILE *arquivo = fopen("saldos.bin", "rb");
    if (arquivo == NULL) {
        return;
    }
    fread(reais, sizeof(float), 4, arquivo);
    fread(bitcoin, sizeof(float), 4, arquivo);
    fread(ethereum, sizeof(float), 4, arquivo);
    fread(ripple, sizeof(float), 4, arquivo);
    fclose(arquivo);
}

void salvar_dados(int cliente, char operacao, float valor, const char* qual_moeda, const char* cotacao, float valor_cotacao, const char* taxa, float valor_taxa, float valor_reais, float valor_bitcoin, float valor_ethereum, float valor_ripple) {
    FILE *dados = fopen("dados_arq.bin", "ab");
    if (dados == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char cpf_cliente[12];
    strncpy(cpf_cliente, cpfs[cliente], sizeof(cpf_cliente));

    // Gravar dados
    fwrite(&tm, sizeof(tm), 1, dados);
    fwrite(&operacao, sizeof(char), 1, dados);
    fwrite(&valor, sizeof(float), 1, dados);
    fwrite(cpf_cliente, sizeof(char), sizeof(cpf_cliente), dados); // Grava o CPF
    fwrite(qual_moeda, sizeof(char), 10, dados); // Tamanho fixo
    fwrite(cotacao, sizeof(char), 10, dados); // Tamanho fixo
    fwrite(&valor_cotacao, sizeof(float), 1, dados);
    fwrite(taxa, sizeof(char), 10, dados); // Tamanho fixo
    fwrite(&valor_taxa, sizeof(float), 1, dados);
    fwrite(&valor_reais, sizeof(float), 1, dados);
    fwrite(&valor_bitcoin, sizeof(float), 1, dados);
    fwrite(&valor_ethereum, sizeof(float), 1, dados);
    fwrite(&valor_ripple, sizeof(float), 1, dados);

    fclose(dados);
}

void ler_dados(int cliente) {
    FILE *dados = fopen("dados_arq.bin", "rb");
    if (dados == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    struct tm tm; // Estrutura para armazenar informações de data/hora
    char operacao, cpf_cliente[12];
    float valor, valor_cotacao, valor_taxa, valor_reais, valor_bitcoin, valor_ethereum, valor_ripple;
    char qual_moeda[10], cotacao[10], taxa[10];

    // Lê o arquivo até que não haja mais dados
    while (fread(&tm, sizeof(tm), 1, dados) == 1) {
        fread(&operacao, sizeof(char), 1, dados);
        fread(&valor, sizeof(float), 1, dados);
        fread(cpf_cliente, sizeof(char), sizeof(cpf_cliente), dados); // Lê o CPF
        fread(qual_moeda, sizeof(char), sizeof(qual_moeda), dados);
        fread(cotacao, sizeof(char), sizeof(cotacao), dados);
        fread(&valor_cotacao, sizeof(float), 1, dados);
        fread(taxa, sizeof(char), sizeof(taxa), dados);
        fread(&valor_taxa, sizeof(float), 1, dados);
        fread(&valor_reais, sizeof(float), 1, dados);
        fread(&valor_bitcoin, sizeof(float), 1, dados);
        fread(&valor_ethereum, sizeof(float), 1, dados);
        fread(&valor_ripple, sizeof(float), 1, dados);

        // Verifica se o CPF corresponde ao cliente logado
        if (strcmp(cpf_cliente, cpfs[cliente]) == 0) {
            printf("Data: %02d-%02d-%d %02d:%02d\n", 
                   tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, 
                   tm.tm_hour, tm.tm_min);
            printf("Operação: %c \tValor: %.2f\n", operacao, valor);
            printf("Moeda: %s \tCotação: %.2f\n", qual_moeda, valor_cotacao);
            printf("Taxa: %.2f\n", valor_taxa);
            printf("Reais: %.2f \tBitcoin: %.2f \tEthereum: %.2f \tRipple: %.2f\n", 
                   valor_reais, valor_bitcoin, valor_ethereum, valor_ripple);
            printf("--------------------------------------------------------------\n");
        }
    }

    fclose(dados);
}


void extrato(int cliente) {
    char senha1[7];
    printf("Digite sua senha: ");
    scanf("%s", senha1);

    if (strcmp(senha1, senhas[cliente]) == 0) {
        printf("--------------------------------------------------------------\n");
        printf("Extrato de %s:\n", nomes[cliente]);
        ler_dados(cliente);
    } else {
        printf("Senha incorreta. Digite novamente.\n");
    }
}


void depositar(int cliente) {
    float v_d;

    printf("Valor do depósito (R$): ");
    scanf("%f", &v_d);
    reais[cliente] += v_d; // Atualiza o saldo
    printf("Depósito realizado. Saldo atual: R$ %.2f\n", reais[cliente]);

    // Grava os dados do depósito
    salvar_dados(cliente,'+', v_d, "REAL", "COT", 0.0, "TAXA", 0.0, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);
}

void sacar(int cliente) {
    char senha1[7];
    float v_s;
    printf("Digite sua senha: ");
    scanf("%s", senha1);

    if (strcmp(senha1, senhas[cliente]) == 0) {
        printf("Valor do saque (R$): ");
        scanf("%f", &v_s);

        if (v_s > reais[cliente]) {
            printf("Saldo inferior ao saque solicitado.\n");
        } else {
            reais[cliente] -= v_s; // Atualiza o saldo
            printf("Saque realizado. Saldo atual: R$ %.2f\n", reais[cliente]);
        }
    } else {
        printf("Senha incorreta. Digite novamente.\n");
    }
    salvar_dados(cliente,'-', v_s, "REAL", "COT", 0.0, "TAXA", 0.0, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);

}

// Núcleo da lógica de interação
void menu_completo(int cliente) {
    int opcao_menu;

    // Cotações Iniciais com valores aproximados (final de setembro)
    cotacao cotacoes;
    cotacoes.bitcoin = 357503.89;
    cotacoes.ethereum = 14509.18;
    cotacoes.ripple = 3.56;

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
                consultar_saldo(cliente);
                break;
            case 2:
                extrato(cliente);
                break;
            case 3:
                depositar(cliente);
                break;
            case 4:
                sacar(cliente);
                break;
            case 5:
                comprar_criptomoedas(cliente, &cotacoes);
                break;
            case 6:
                vender_criptomoedas(cliente, &cotacoes);
                break;
            case 7:
                atualizar_cotacao(&cotacoes);
                break;
            case 8:
                return; // Sai do laço e retorna à função 'efetuar_login'
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
}

// Função para encerrar o programa com sucesso (status 0)
void encerrar_programa() { 
    salvar_saldos();
    printf("==--------------------------------------------------==\n");
    printf("Encerrando o Programa. Obrigado por trabalhar conosco!\n");
    printf("==--------------------------------------------------==\n");
    exit(0); 
}


void efetuar_login() {
    char cpf_login[12];  // 11 dígitos + 1 para o caractere nulo
    char senha_login[7]; // 6 dígitos + 1 para o caractere nulo
    int opcao_login;

    // Laço infinito para o processo de login dos clientes
    while (1) {
        printf("==--------------------------------------==\n");
        printf("Seja bem-vindo à Exchange de Criptomoedas!\n");
        printf("1 - Efetuar Login\n");
        printf("2 - Sair\n");
        printf("==--------------------------------------==\n");
        printf("Por favor, selecione a opção desejada: ");
        scanf("%d", &opcao_login);

        if (opcao_login == 2){
            encerrar_programa(); // Saída
        } else if (opcao_login != 1){
            printf("Opção irreconhecível. Tente novamente.\n");
        } else {
            printf("Digite seu CPF (Somente números): ");
            scanf("%s", cpf_login);

            printf("Digite sua senha (Somente números): ");
            scanf("%s", senha_login);

            for (int i = 0; i < 4; i++) {
                if (strcmp(cpfs[i], cpf_login) == 0 && strcmp(senhas[i], senha_login) == 0) {
                    printf("==--------------------------------------==\n");
                    printf("É bom te ter de volta, %s :)\n", nomes[i]);
                    menu_completo(i);
                    return; 
                }
            } 
            printf("CPF ou senha inválidos. Tente novamente.\n");
        }
    }
}

void comprar_criptomoedas(int cliente, cotacao* cotacoes){
    float valor_compra, valor_taxa, valor_total;
    int opcao_moeda;
    char senha_login[7];

    printf("Operação escolhida: Comprar criptomoedas para cliente %s\n", nomes[cliente]);
    printf("Digite sua senha novamente: ");
    scanf("%s", senha_login);

    if (strcmp(senha_login, senhas[cliente]) != 0){
        printf("Senha incorreta. Operação cancelada.\n");
        return;
    }

    printf("==-----------------------------==\n");
    printf("Valores disponíveis na carteira:\n");
    printf("Reais (R$): %.2f\n", reais[cliente]);
    printf("Bitcoin (BTC): %.2f\n", bitcoin[cliente]);
    printf("Ethereum (ETH): %.2f\n", ethereum[cliente]);
    printf("Ripple (XRP): %.2f\n", ripple[cliente]);
    printf("==-----------------------------==\n");

    printf("Tabela de valores:\n");
    printf("1 - BTC (Taxa de 2%%): %.2f\n2 - ETH (Taxa de 1%%): %.2f\n3 - XRP (Taxa de 1%%): %.2f\n", cotacoes->bitcoin, cotacoes->ethereum, cotacoes->ripple);
    printf("Escolha a criptomoeda que deseja comprar: ");
    scanf("%d", &opcao_moeda);

    if(opcao_moeda < 1 || opcao_moeda > 3){
        printf("Opção inválida. Por favor, tente novamente.\n");
        return;
    }

    printf("Digite o valor em reais para compra: ");
    scanf("%f", &valor_compra);

    if (valor_compra <= 0){
        printf("Valor inválido. Por favor, tente novamente.\n");
        return;
    }

    if (opcao_moeda == 1){
        valor_taxa = valor_compra * 0.02; // Bitcoin 
    } else {
        valor_taxa = valor_compra * 0.01; // Ethereum e Ripple
    }

    valor_total = valor_compra + valor_taxa;

    if (valor_total > reais[cliente]) {
        printf("Saldo insuficiente para a compra. Por favor, tente novamente.\n");
        return;
    }

    // Aprovação da compra, atualização dos saldos e salvamento de dados de compra
    if (opcao_moeda == 1){
        bitcoin[cliente] += valor_total/cotacoes->bitcoin; 
        printf("Compra de Bitcoin realizada com sucesso!\n");
        salvar_dados(cliente, '+', valor_compra, "BTC", "TAXA", valor_taxa, "", valor_total, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);
    } else if (opcao_moeda == 2){
        ethereum[cliente] += valor_total/cotacoes->ethereum;
        printf("Compra de Ethereum realizada com sucesso!\n");
        salvar_dados(cliente, '+', valor_compra, "ETH", "TAXA", valor_taxa, "", valor_total, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);
    } else if (opcao_moeda == 3){
        ripple[cliente] += valor_total/cotacoes->ripple;
        printf("Compra de Ripple realizada com sucesso!\n");
        salvar_dados(cliente, '+', valor_compra, "XRP", "TAXA", valor_taxa, "", valor_total, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);
    } 

    // Ajustar valor na carteira após validação da compra
    reais[cliente] -= valor_total;

}

void vender_criptomoedas(int cliente, cotacao* cotacoes){
    float valor_venda, valor_taxa, valor_total;
    int opcao_moeda;
    char senha_login[7];

    printf("Operação escolhida: Vender criptomoedas para cliente %s\n", nomes[cliente]);
    printf("Digite sua senha novamente: ");
    scanf("%s", senha_login);

    if (strcmp(senha_login, senhas[cliente]) != 0){
        printf("Senha incorreta. Operação cancelada.\n");
        return;
    }

    printf("==-----------------------------==\n");
    printf("Valores disponíveis na carteira:\n");
    printf("Reais (R$): %.2f\n", reais[cliente]);
    printf("Bitcoin (BTC): %.2f\n", bitcoin[cliente]);
    printf("Ethereum (ETH): %.2f\n", ethereum[cliente]);
    printf("Ripple (XRP): %.2f\n", ripple[cliente]);
    printf("==-----------------------------==\n");

    printf("Moedas disponíveis:\n");
    printf("1 - BTC (Taxa de 3%%)\n2 - ETH (Taxa de 2%%)\n3 - XRP (Taxa de 1%%)\n");
    printf("Escolha a criptomoeda que deseja vender: ");
    scanf("%d", &opcao_moeda);

    if(opcao_moeda < 1 || opcao_moeda > 3){
        printf("Opção inválida. Por favor, tente novamente.\n");
        return;
    }

    printf("Digite o valor que deseja vender: ");
    scanf("%f", &valor_venda);

    if (valor_venda <= 0){
        printf("Valor inválido. Por favor, tente novamente.\n");
        return;
    } if (opcao_moeda == 1) { // BTC
        valor_taxa = valor_venda * 0.03;
        valor_total = valor_venda - valor_taxa;

        float valor_venda_bitcoin = valor_venda / cotacoes->bitcoin;

        if (valor_venda_bitcoin > bitcoin[cliente]){
            printf("Saldo insuficiente de Bitcoin para realizar a venda. Por favor, tente novamente.\n");
            return;
        }

        bitcoin[cliente] -= valor_venda_bitcoin;
        reais[cliente] += valor_total;
        printf("==------------------------------------==\n");
        printf("Venda de Bitcoin realizada com sucesso!\n");
        salvar_dados(cliente, '-', valor_venda, "BTC", "TAXA", valor_taxa, "", valor_total, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);

    } else if (opcao_moeda == 2){ // ETH
        valor_taxa = valor_venda * 0.02;
        valor_total = valor_venda - valor_taxa;

        float valor_venda_ethereum = valor_venda / cotacoes->ethereum;

        if (valor_venda_ethereum > ethereum[cliente]) {
            printf("Saldo insuficiente de Ethereum para realizar a venda.\n");
            return;
        }

        ethereum[cliente] -= valor_venda_ethereum;
        reais[cliente] += valor_total;
        printf("==-------------------------------------==\n");
        printf("Venda de Ethereum realizada com sucesso!\n");
        salvar_dados(cliente, '-', valor_venda, "ETH", "TAXA", valor_taxa, "", valor_total, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);

    } else if (opcao_moeda == 3){  // XRP
        valor_taxa = valor_venda * 0.01; 
        valor_total = valor_venda - valor_taxa;

        float valor_venda_ripple = valor_venda / cotacoes->ripple;

        if (valor_venda_ripple > ripple[cliente]) {
            printf("Saldo insuficiente de Ripple para realizar a venda.\n");
            return;
        }

        ripple[cliente] -= valor_venda_ripple;
        reais[cliente] += valor_total;
        printf("==------------------------------------==\n");
        printf("Venda de Ripple realizada com sucesso!\n");
        salvar_dados(cliente, '-', valor_venda, "XRP", "TAXA", valor_taxa, "", valor_total, reais[cliente], bitcoin[cliente], ethereum[cliente], ripple[cliente]);
    }
}

void atualizar_cotacao(cotacao *cotacoes){
    char verifica;
    printf("==----------------------==\n");
    printf("Cotações Atuais:\n");
    printf("Bitcoin (BTC): %.2f\n", cotacoes->bitcoin);
    printf("Ethereum (ETH): %.2f\n", cotacoes->ethereum);
    printf("Ripple (XRP): %.2f\n", cotacoes->ripple);
    printf("==----------------------==\n");
    printf("Gostaria de atualizar a cotação? (Y/N) ");
    scanf(" %c", &verifica);

    if (verifica == 'N' || verifica == 'n'){
        printf("Retornando ao menu...\n");
        return;
    } else if (verifica == 'Y' || verifica == 'y'){
       float percentual_var, fator_var;

        srand(time(NULL));
        percentual_var = ((rand() % 101) / 100.0) * 10.0 - 5.0; // Gera variação entre -5.0% e 5.0%
        fator_var = 1 + (percentual_var / 100.0);

        // Atualizar cotação de Bitcoin
        cotacoes->bitcoin *= fator_var;

        // Atualizar cotação de Ethereum
        cotacoes->ethereum *= fator_var;

        // Atualizar cotação de Ripple
        cotacoes->ripple *= fator_var;

        printf("Cotações atualizadas:\n");
        printf("Bitcoin: %.2f\n", cotacoes->bitcoin);
        printf("Ethereum: %.2f\n", cotacoes->ethereum);
        printf("Ripple: %.2f\n", cotacoes->ripple);
    } else {
        printf("Opção irreconhecível. Por favor, tente novamente.\n");
    }
}

int main() {

    // Declarar para evitar erro de segmentação
    cotacao cotacoes =  {357503.89, 14509.18, 3.56};

    carregar_saldos();
    while (1) {
        efetuar_login();
    }

    return 0;
}