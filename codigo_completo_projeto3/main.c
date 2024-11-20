#include "admin.h"      // Incluindo o cabeçalho do admin
#include "investidor.h" // Incluindo o cabeçalho do investidor
#include <stdio.h>

int main() {
  while (1){
    int escolha;
    printf("Escolha uma opção:\n");
    printf("1. Acessar como Administrador\n");
    printf("2. Acessar como Investidor\n");
    printf("Escolha: ");
    scanf("%d", &escolha);
  
    if (escolha == 1) {
      efetuar_login_admin(); // Função de login do administrador
    } else if (escolha == 2) {
      main_investidor(); // Função principal do investidor
    } else {
      printf("Opção inválida.\n");
    }
  }

  return 0;
}
