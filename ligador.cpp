#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;

int verifica_tamanho(string linha){

  // Com espaços o que não é o que queremos
  int tamanho_linha = linha.length();
  int i, tamanho_efetivo = 0;

  // Só incrementa tamanho_efetivo se o caractere não for espaço e então procura o próximo espaço
  for(i = 0; i<tamanho_linha; i++){
    if(linha[i] != ' '){
      tamanho_efetivo++;
      while(linha[i] != ' '){
        i++;
      }
    }
  }
  cout<<tamanho_efetivo<<endl;
  return tamanho_efetivo;
}

int alinha_modulos(fstream &arq_exe ,ifstream &modulo_atual){

  string linha;

  while(getline(modulo_atual,linha)){

    if(linha == "CODE"){
      linha.clear();
      getline(modulo_atual,linha);
      linha += ' ';
      arq_exe << linha;
      return verifica_tamanho(linha);
    }
    linha.clear();

  }
  // Arquivo obj não tem código
  return -1;



}

int main(int argc, char const *argv[]) {

  int i = 0, contador_modulos = 0;
  int tamanho_0, tamanho_1, tamanho_2, tamanho_3;

  ifstream mod_0,mod_1,mod_2,mod_3;
  fstream arq_exe;
  string modulo_0, modulo_1, modulo_2, modulo_3;
  string aux;

  modulo_0 += argv[1];
  aux += modulo_0;
  aux += ".e";
  modulo_0 += ".obj";
  modulo_1 += argv[2];
  modulo_1 += +".obj";
  contador_modulos = 2;
  // while(i<=argc){
  //   if(i == 1){
  //     modulo_0 += argv[i];
  //     contador_modulos++;
  //   }else if(i == 2){
  //     modulo_1 += argv[i];
  //     contador_modulos++;
  //   }else if(i == 3){
  //     modulo_2 += argv[i];
  //     contador_modulos++;
  //   }else if(i == 4){
  //     modulo_3 += argv[i];
  //         contador_modulos++;
  //   }
  //   i++;
  // }

  mod_0.open(&modulo_0[0]);
  mod_1.open(&modulo_1[0]);
  arq_exe.open(&aux[0],ios::out);

  tamanho_0 = alinha_modulos(arq_exe,mod_0);
  tamanho_1 = alinha_modulos(arq_exe,mod_1);

  mod_0.close();
  mod_1.close();
  arq_exe.close();

}
