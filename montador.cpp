#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;

void pre_processamento(ifstream &arq_fonte ,ofstream &arq_pre_processado){

  // Entradas: arq_fonte = Arquivo assembly que vai ser pre processado
  // Saídas: arq_pre_processado = Arquivo assembly já pre processado
  // Comentários: Essa função retira comentários do código fonte e trata as diretivas EQU e IF

  int i, length_linha, posicao, acumulador_erro, flag_rotulo, length_palavra, flag_IF, pular_proxima_linha = 0;
  string linha, nova_linha, palavra, rotulo, valor_EQU;
  map<string, string> EQU_map; //mapa de EQU's definidos

  // Varre o arquivo fonte
  while(getline(arq_fonte,linha)){

    acumulador_erro = 0;
    flag_IF = 0;
    flag_rotulo = 0; // Serve para saber se a linha lida tinha algum rótulo definido
    length_linha = linha.length();

    // Esse if faz com que caso a linha comece por comentário não fique um \n no arquivo pre processado
    if(linha[0] == ';'){
      continue;
    }

    // Esse if faz com que o pre processador ignore a próxima linha caso tenha um IF 0 antes dela
    if(pular_proxima_linha == 1){
      pular_proxima_linha = 0;
      continue;
    }

    // Varre a linha
    for(i=0; i<length_linha; i++){
      nova_linha += linha[i];
      palavra += linha[i];

      // Se for ';' deve parar de ler a linha já que é comentário
      if(linha[i] == ';'){
           // Apaga o ';'
           nova_linha.erase(nova_linha.end()-1);
           // Esse if apaga também o espaço q vem antes do ';' se tiver
           if(linha[i-1] == ' '){
             nova_linha.erase(nova_linha.end()-1);
           }
           break;
      }

      // Pega os rótulos presentes no código
      if(linha[i] == ':'){
        flag_rotulo = 1;
        posicao = i;
        rotulo += nova_linha;
        // Retira o ':' do rótulo
        rotulo.erase(rotulo.end()-1);
      }

      // Esse bloco identifica se foi lido um IF e seta a flag para tratar essa informação para 1
      if(linha[i] == ' ' && palavra.length()==3){
        if(linha[i-2] == 'I' && linha[i-1] == 'F'){
          flag_IF = 1;
        }
      }

      // Essa parte do código checa se existe algum EQU para ser substituído, esse if checa a palavra antes de um espaço em branco
      if(linha[i] == ' '){
        if(palavra.length() > 1){
          //remove espaço
          palavra.erase(palavra.end()-1);
          // Checa se a palavra lida é um EQU já definido, se for apagamos a palavra e escrevemos o seu valor no map de EQU's
          if (EQU_map.find(palavra) != EQU_map.end()){
            length_palavra = palavra.length();
            nova_linha.erase(i-length_palavra-acumulador_erro,length_palavra+1);
            nova_linha += EQU_map.find(palavra)->second;
            nova_linha += ' ';
            // Necessário para escrever a palavra certa no código de saída
            acumulador_erro += length_palavra-1;
          }
        }else{
          // Essa linha de código apaga espaços extras que aparecerem no código fonte
          nova_linha.erase(nova_linha.end()-1);
          acumulador_erro +=1;
        }
        palavra.clear();
      // Complemento da checagem de EQU's a serem substituídos, esse if checa uma palavra no final da linha
      }else if(i == (length_linha - 1)){
        // Checa se a palavra lida é um EQU já definido, se for apagamos a palavra e escrevemos o seu valor no map de EQU's
        if (EQU_map.find(palavra) != EQU_map.end()){
          length_palavra = palavra.length();
          nova_linha.erase(i-length_palavra+1-acumulador_erro,length_palavra);
          nova_linha += EQU_map.find(palavra)->second;
        }
        palavra.clear();
      }
    }

    // Checa se o rótulo era para um EQU ou outra coisa
    if(flag_rotulo == 1){
      // Verificar se existe um jeito melhor para isso \/
      if(nova_linha[posicao+2] == 'E' && nova_linha[posicao+3] == 'Q' && nova_linha[posicao+4] == 'U' && nova_linha[posicao+5] == ' ' ){
        // Se for EQU adiciona no map de EQU's
        for(i=posicao+6;i<length_linha;i++){
          valor_EQU+=nova_linha[i];
        }
        // Depois de tratar o EQU essa linha pode ser descartada
        EQU_map.insert(make_pair(rotulo,valor_EQU));
        valor_EQU.clear();
        rotulo.clear();
        nova_linha.clear();
        continue;
      }
    }

    // Caso essa linha tenha um IF checamos para ver se ele é falso ou verdadeiro
    if(flag_IF == 1){
      for(i=3;i<nova_linha.length();i++){
        palavra+=nova_linha[i];
      }
      // Se for falso, devemos ignorar a próxima linha se verdadeiro seguir na leitura normalmente
      if(atoi(palavra.c_str()) == 0){
        pular_proxima_linha = 1;
      }
      // Depois de tratar o IF essa linha pode ser descartada
      valor_EQU.clear();
      rotulo.clear();
      nova_linha.clear();
      continue;
    }

    // Escreve no arquivo programa.pre a linha sem os comentarios, com EQU substituído
    arq_pre_processado << nova_linha << endl;

    // Limpa as strings utilizadas na função
    valor_EQU.clear();
    rotulo.clear();
    nova_linha.clear();
  }
}

int main () {

  string linha;
  string palavra;
  int i;
  int length_linha;
  ifstream arq_fonte;
  ofstream arq_pre_processado;
  map<string, pair<int,int> > i_map; //mapa de instruções


  i_map.insert(make_pair("ADD", make_pair(1, 3)));
  i_map.insert(make_pair("SUB", make_pair(2, 3)));
  i_map.insert(make_pair("MULT", make_pair(3, 3)));
  i_map.insert(make_pair("DIV", make_pair(4, 3)));
  i_map.insert(make_pair("JMP", make_pair(5, 3)));
  i_map.insert(make_pair("JMPN", make_pair(6, 3)));
  i_map.insert(make_pair("JMPP", make_pair(7, 3)));
  i_map.insert(make_pair("JMPZ", make_pair(8, 3)));
  i_map.insert(make_pair("COPY", make_pair(9, 3)));
  i_map.insert(make_pair("LOAD", make_pair(10, 3)));
  i_map.insert(make_pair("STORE", make_pair(11, 3)));
  i_map.insert(make_pair("INPUT", make_pair(12, 3)));
  i_map.insert(make_pair("OUTPUT", make_pair(13, 3)));
  i_map.insert(make_pair("STOP", make_pair(14, 3)));

  arq_fonte.open("programa.asm");
  arq_pre_processado.open("programa.pre",ofstream::out);

  if(!arq_fonte.is_open() || !arq_pre_processado.is_open()){

    cout << "Impossível abrir arquivo solicitado!";
    return -1;
  }

  // Passagem de pre processamento
  pre_processamento(arq_fonte, arq_pre_processado);

  while(getline(arq_fonte, linha)){

    length_linha = linha.length();

    //varre a linha, separando cada palavra
    for(i=0; i<length_linha; i++){

      palavra += linha[i];

      //fim de palavra
      if(linha[i] == ' '){

        if(palavra.length() > 1){

          //remove espaço
          palavra.erase(palavra.end()-1);

          if (i_map.find(palavra) != i_map.end()){

            //cout << palavra << ' ' << i_map.find(palavra)->second.first << endl;
          }
          else{

            //cout << palavra << ' ' << "não existente!" << endl;
          }
        }

        palavra.clear();
      }

      //fim de linha (e de uma palavra)
      else if(i == (length_linha - 1)){

        if (i_map.find(palavra) != i_map.end()){

          //cout << palavra << ' ' << i_map.find(palavra)->second.first << endl;
        }
        else{

          //cout << palavra << ' ' << "não existente!" << endl;
        }

        palavra.clear();
      }
    }
  }

  arq_fonte.close();

  return 0;
}
