// Programadores: Pedro Saman D N Cesarino   15/0144890
//                Pedro Victor Galieta Tomaz 15/0144938
// Professor: Bruno Luiggi Macchiavello Espinoza
// Universidade de Brasília - Departamento de Ciência da Computação
// Objetivo: Fazer um programa que é capaz de fazer a ligação (linking) de diferentes módulos de um mesmo código no formato
//           objeto que foi montado por outro arquivo. O assembly que gera esse objeto é o assembly inventado utilizado na
//           disciplina de Software Básico

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

void add_vector_relativo(string linha,vector <int> &tamanho,vector <pair<int,int> > &relativos, int indice_modulo){

  // Entradas: linha = é a string q contém cada linha da seção "RELATIVE"
  //           tamanho = é um vector usado para corrigirmos os valores das posições indicadas
  //           relativos = é justamente o vector que nos diz quais posições são relativas e qual seu módulo originário
  //           indice_modulo = é um inteiro que indica qual o número do atual módulo sendo processado
  // Saídas: Não há saídas já que relativos é passado por referência
  // Comentários: Essa função cria um vector com todas as posições, já corrigidas, que são relativas no código final
  //              e qual o módulo original desse valor relativo


  int i, j = 0, valor_int;
  string valor_str;
  linha += ' ';
  for(i = 0; i<linha.length(); i++){
    if(linha[i] != ' '){
      valor_str += linha[i];
    }else{
      j = 0;
      valor_int = atoi(valor_str.c_str());
      while(j<indice_modulo){
        valor_int += tamanho[j];
        j++;
      }
      relativos.push_back(pair<int,int> (valor_int,indice_modulo));
      valor_str.clear();
    }
  }
}

int verifica_tamanho(string linha){

  // Entradas: linha = é a string q contém toda a seção do código objeto que contém o código máquina
  // Saídas: tamanho_efetivo = tamanho em inteiro do tamanho da seção de código para tal módulo
  // Comentários: Essa função é necessária para corrigirmos os valores relativos das partes de código que vem após o 1º módulo

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
  return tamanho_efetivo;
}

void add_tab_uso(string linha ,map <int,string>  &uso, int indice_modulo, vector<int> tamanho){

  // Entradas: linha = é a string q contém cada linha da seção "TABLE USE"
  //           uso = é o map que indexa uma posição com um label que foi utilizado com referência cruzada
  //           indice_modulo = é um inteiro que indica qual o número do atual módulo sendo processado
  //           tamanho = é o vector que guarda o tamanho em inteiro dos módulos
  // Saídas: Não há saídas já que uso é passado por referência
  // Comentários: Essa função adiciona em um map todas as posições, já corrigidas, em que uma label foi utilizada com
  //              referência cruzada

  int i, fator_corrigido;
  string nome;
  string valor_str;

  for(i=0;i<linha.length();i++){
    if(linha[i] != ' '){
      nome += linha[i];
    }else {
      i++;
      break;
    }
  }
  for(i;i<linha.length();i++){
      valor_str += linha[i];
  }

  i = 0;
  while(i<indice_modulo){
    fator_corrigido += tamanho[i];
    i++;
  }

  fator_corrigido += atoi(valor_str.c_str());
  uso.insert(make_pair(fator_corrigido, nome));

}

void add_tab_uso_geral(string linha, map <string,int>  &uso_geral, int indice_modulo, vector<int> tamanho){

  // Entradas: linha = é a string q contém cada linha da seção "TABLE DEFINITION"
  //           uso = é o map que indexa os labels que foram utilizados com referência cruzada e sua posição já corrigida
  //           indice_modulo = é um inteiro que indica qual o número do atual módulo sendo processado
  //           tamanho = é o vector que guarda o tamanho em inteiro dos módulos
  // Saídas: Não há saídas já que uso_geral é passado por referência
  // Comentários: Essa função cria um map com todos os labels utilizados com referência cruzada e o valor da memória
  //              já corrigido

  int i, fator_corrigido;
  string nome;
  string valor_str;

  for(i=0;i<linha.length();i++){
    if(linha[i] != ' '){
      nome += linha[i];
    }else {
      i++;
      break;
    }
  }
  for(i;i<linha.length();i++){
      valor_str += linha[i];
  }

  i = 0;
  while(i<indice_modulo){
    fator_corrigido += tamanho[i];
    i++;
  }

  fator_corrigido += atoi(valor_str.c_str());

  uso_geral.insert(make_pair(nome, fator_corrigido));
}

void add_vector_codigo(string linha,vector<int> &codigo){

  // Entradas: linha = é a string q contém cada linha da seção "CODE"
  //           codigo = é um vector de inteiros que concatena os códigos objeto de todos os módulos juntos
  // Saídas: Não há saídas já que o vector codigo é passado por referência
  // Comentários: Nessa função lemos cada valor da linha "CODE" convertemos para inteiro e guardamos no vector

  int i;
  string valor_str;

  for(i=0;i<linha.length();i++){
    if(linha[i] != ' '){
      valor_str += linha[i];
    }else{
      codigo.push_back(atoi(valor_str.c_str()));
      valor_str.clear();
    }
  }
}

void trata_modulos(ifstream &modulo_atual, vector<int> &tamanho, map <string,int>  &uso_geral, map <int,string> &uso, int indice_modulo, vector<pair<int,int> > &relativos, vector<int> &codigo){

  // Entradas: modulo_atual = é o arquivo do módulo sendo processado
  //           tamanho = é um vector que guarda dinamicamente o tamanho do código efetivo do módulo
  //           uso_geral = é um map que indexa todas referências cruzadas e suas posições corrigidas
  //           uso = é um map que indexa a posição no código que uma referência cruzada tal foi usada
  //           indice_modulo = é um inteiro que nos diz qual a posição desse módulo entre os outros
  //           relativos = é um vector que guarda quais posições do código são relativas e qual seu respectivo módulo
  //           codigo = é um vector que guarda os valores do código objeto de todos os módulos em ordem de aparecimento
  // Saídas:   Não há saídas, pois todas as estruturas de dados são passadas por referência
  // Comentários: Essa função faz a gestão do que é lido nas várias linhas de texto de cada arquivo objeto, ou seja,
  //              ela meramente lê uma linha e, se ela for igual a alguma frase específica, chama uma função para tra-
  //              tar aquela seção do objeto


  int i;
  string linha;

  // Itera sobre o módulo todo linha por linha
  while(getline(modulo_atual,linha)){

    // Cria a tabela de uso para cada módulo
    if(linha == "TABLE USE"){
      while(linha != "TABLE DEFINITION"){
        linha.clear();
        getline(modulo_atual,linha);
        if(linha != "TABLE DEFINITION"){
          add_tab_uso(linha, uso, indice_modulo, tamanho);
        }
      }
    }

    // Cria a tabela de uso geral
    if(linha == "TABLE DEFINITION"){
      while(linha != "RELATIVE"){
        linha.clear();
        getline(modulo_atual,linha);
        if(linha != "RELATIVE"){
            add_tab_uso_geral(linha, uso_geral, indice_modulo, tamanho);
        }
      }
    }

    // Indica quais endereços são relatrivos
    if(linha == "RELATIVE"){
      linha.clear();
      getline(modulo_atual,linha);
      add_vector_relativo(linha,tamanho,relativos, indice_modulo);
    }

    // Concatena os códigos objeto de cada módulo no arquivo de saída e calcula o tamanho de memória do módulo
    if(linha == "CODE"){
      linha.clear();
      getline(modulo_atual,linha);
      linha += ' ';
      add_vector_codigo(linha,codigo);
      tamanho.push_back(verifica_tamanho(linha));
    }
    linha.clear();
  }
}

int main(int argc, char const *argv[]) {

  stringstream ss;
  int somador;
  int flag_relativo;
  string referencia;
  vector<string> modulos;
  vector<int> tamanho;
  vector<pair<int, int> > relativos;
  vector<int> codigo;
  map <string,int> uso_geral;
  map <int,string> uso;

  ifstream mod;
  fstream arq_exe;
  string modulo;

  int j;
  int i = 1; // Ignora "./ligador"
  while(i<argc){
    modulos.push_back(argv[i]);
    i++;
  }

  i = 0;
  // Chama a função de tratameto para cada módulo lido do terminal
  while(i<modulos.size()){

    // Cria o arquivo de saída ".e" com o mesmo nome do primeiro módulo
    if(i == 0){
      modulo += modulos[i];
      modulo += ".e";
      arq_exe.open(&modulo[0],ios::out);
    }

    modulo.clear();
    // Pega o nome do módulo passado e o abre
    modulo += modulos[i];
    modulo += ".obj";
    mod.open(&modulo[0]);

    // Se o código só tem um módulo, o código não precisa ser ligado
    if(modulos.size() == 1){
      modulo.clear();
      getline(mod, modulo);
      arq_exe << modulo;
      return 0;
    }

    // Preenche todas as estruturas necessárias para a ligação
    trata_modulos(mod,tamanho,uso_geral, uso, i, relativos, codigo);

    // Fecha o módulo que acabamos de processar
    mod.close();

    // Passa para o próximo módulo
    i++;
  }

  i = 0;
  //Após criar todas as estruturas necessárias, corrige os endereços relativos
  while(i<codigo.size()){ // Enquanto não chega no final do código
    somador = codigo[i];
    j = 0;
    flag_relativo = 0;

    // Checa se o endereço é relativo
    while(j<relativos.size()){
      if(relativos[j].first == i){
        flag_relativo = 1;
        j++;
        break;
      }
      j++;
    }

    if(flag_relativo == 1){
      // Se for relativo, checa se ele está na tabela de uso de algum módulo
      if(uso.find(i) != uso.end()){
        somador += uso_geral.find(uso.find(i)->second)->second;
      }else {
        // Se não, apenas conserta a o enderço relativo ao primeiro módulo
        int k = 0;
        while(k<relativos[j].second){
          somador += tamanho[k];
          k++;
        }
      }
    }
    i++;
    // essas linhas são uma gambiarra para subistituir o to_string que não funciona no meu compilador de c++
    ss << somador;
    referencia = ss.str();
    referencia += ' ';
    arq_exe << referencia;
    referencia.clear();
    ss.str("");
  }

  // Fecha o arquivo de saída
  arq_exe.close();

  return 0;
}
