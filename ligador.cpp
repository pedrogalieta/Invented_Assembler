#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

void add_vector_relativo(string linha,vector <int> &tamanho,vector <pair<int,int> > &relativos, int indice_modulo){

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
  return tamanho_efetivo;
}

void add_tab_uso(map <int,string>  &uso, string linha, int indice_modulo, vector<int> tamanho){

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

void add_tab_uso_geral(map <string,int>  &uso_geral, string linha, int indice_modulo, vector<int> tamanho){

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
          add_tab_uso(uso, linha, indice_modulo, tamanho);
        }
      }
    }

    // Cria a tabela de uso geral
    if(linha == "TABLE DEFINITION"){
      while(linha != "RELATIVE"){
        linha.clear();
        getline(modulo_atual,linha);
        if(linha != "RELATIVE"){
            add_tab_uso_geral(uso_geral, linha, indice_modulo, tamanho);
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
    // Trata cada módulo separadamente, gerando todas tabelas e vectors de interesse
    modulo += modulos[i];
    modulo += ".obj";
    mod.open(&modulo[0]);
    // Se o código só tem um módulo, o código não precisa ser ligado
    if(modulos.size() == 1){
      modulo.clear();
      getline(mod, modulo);
      arq_exe << modulo;
    }
    trata_modulos(mod,tamanho,uso_geral, uso, i, relativos, codigo);
    mod.close();
    i++;
  }

  i = 0;
  //Corrige os endereços
  while(i<codigo.size()){
    somador = codigo[i];
    j = 0;
    flag_relativo = 0;

    while(j<relativos.size()){
      if(relativos[j].first == i){
        flag_relativo = 1;
        j++;
        break;
      }
      j++;
    }

    if(flag_relativo == 1){
      if(uso.find(i) != uso.end()){
        somador += uso_geral.find(uso.find(i)->second)->second;
      }else {
        int k = 0;
        while(k<relativos[j].second){
          somador += tamanho[k];
          k++;
        }
      }
    }
    ss << somador;
    referencia = ss.str();
    referencia += ' ';
    arq_exe << referencia;
    referencia.clear();
    ss.str("");
    i++;
  }

  arq_exe.close();
  return 0;
}
