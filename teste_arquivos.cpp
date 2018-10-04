#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctype.h>

using namespace std;

void insere_diretivas_no_mapa(map<string,int> &tabela_de_diretivas){ //Função que insere as chaves no map de instruções

     tabela_de_diretivas.insert(make_pair("SECTION",0));
     tabela_de_diretivas.insert(make_pair("SPACE",1));
     tabela_de_diretivas.insert(make_pair("CONST",2));
     tabela_de_diretivas.insert(make_pair("PUBLIC",3));
     tabela_de_diretivas.insert(make_pair("EQU",4));
     tabela_de_diretivas.insert(make_pair("IF",5));
     tabela_de_diretivas.insert(make_pair("EXTERN",6));
     tabela_de_diretivas.insert(make_pair("BEGIN",7));
     tabela_de_diretivas.insert(make_pair("END",8));

}

void insere_instrucoes_no_mapa(map<string,int> &tabela_de_instrucoes){ //Função que insere as chaves no map de instruções

     tabela_de_instrucoes.insert(make_pair("ADD",1));
     tabela_de_instrucoes.insert(make_pair("SUB",2));
     tabela_de_instrucoes.insert(make_pair("MULT",3));
     tabela_de_instrucoes.insert(make_pair("DIV",4));
     tabela_de_instrucoes.insert(make_pair("JMP",5));
     tabela_de_instrucoes.insert(make_pair("JMPN",6));
     tabela_de_instrucoes.insert(make_pair("JMPP",7));
     tabela_de_instrucoes.insert(make_pair("JMPZ",8));
     tabela_de_instrucoes.insert(make_pair("COPY",9));
     tabela_de_instrucoes.insert(make_pair("LOAD",10));
     tabela_de_instrucoes.insert(make_pair("STORE",11));
     tabela_de_instrucoes.insert(make_pair("INPUT",12));
     tabela_de_instrucoes.insert(make_pair("OUTPUT",13));
     tabela_de_instrucoes.insert(make_pair("STOP",14));

}

int checa_abertura_dos_arquivos(ifstream &programa, fstream &simbolos){ //Checa se os arquivo foram abertos corretamente

     if (!(programa.is_open() && programa.good())){
       cout << "Arquivo Fonte não foi achado \nFinalizando programa";
       return -1;
     }

    if (!simbolos.is_open()){
      cout << "Tabela de simbolos não aberta corretamente \nFinalizando programa";
      return -1;
    }
    return 1;
}

int main(){

     int i,j,k,tamanho_da_linha,x;
     string aux1;
     string aux2;
     string linha_codigo_fonte;
     string palavra_codigo_fonte;
     string palavra_da_tabela;
     string inicio_comentario = ";";
     map<string,int> tabela_de_instrucoes;
     map<string,int> tabela_de_diretivas;

     ifstream arquivo_fonte;		        //Cria os objetos para o arquivo fonte
     fstream tabela_de_simbolos;           //Cria o objeto para a tabela de simbolos para leitura e escrita
     arquivo_fonte.open("programa.asm");   //Abre os arquivos para o codigo fonte e a tabela de simbolos
     tabela_de_simbolos.open("tabela_de_simbolos.txt", fstream::app | fstream::in);

     if (!(checa_abertura_dos_arquivos(arquivo_fonte,tabela_de_simbolos))){ //Mata o programa caso não consiga abrir os arquivos
            return -1;
     }

    insere_instrucoes_no_mapa(tabela_de_instrucoes); //Insere as instruções no mapa
    insere_diretivas_no_mapa(tabela_de_diretivas); //Insere as diretivas no mapa

    // Fazer uma Função que faz isso. Esse while pega todas as linhas do arquivo asm e verifica se cada palavra é ou não uma instrução e devolve o opcode
    while(!arquivo_fonte.fail()){ //Itera sobre o arquivo assembly
          getline(arquivo_fonte,linha_codigo_fonte); //Pega linha por linha do arquivo assembly
          i=0;
          k=0;
          tamanho_da_linha = linha_codigo_fonte.length();

          //Esse while itera pela linha lida pelo getline
          while(i<tamanho_da_linha){
               for(j=0;linha_codigo_fonte[i]!=' ' ;j++,i++){
                    palavra_codigo_fonte += linha_codigo_fonte[i];
                    if(j+k >= tamanho_da_linha){
                         palavra_codigo_fonte.erase(j,palavra_codigo_fonte.length());
                         break;
                    }
               }

               // Esse if interrompe a leitura do resto da linha se lermos um ;
               if (palavra_codigo_fonte == inicio_comentario){
                    palavra_codigo_fonte.erase(0,palavra_codigo_fonte.length());
                    break;
               }

               // Esse if else escreve na tela se a palavra lida da linha é ou não uma instrução
               if(tabela_de_instrucoes.find(palavra_codigo_fonte) != tabela_de_instrucoes.end()){
                         cout << "opcode de " << palavra_codigo_fonte << " é: " << tabela_de_instrucoes.find(palavra_codigo_fonte)->second << endl;
               }else if(palavra_codigo_fonte.length() != 0){ //Não precisa checar caso a string seja NULL
                     cout<< "Não achamos:" << palavra_codigo_fonte << "no map " << endl;
               }

               palavra_codigo_fonte.erase(0,palavra_codigo_fonte.length()); //Apaga a string palavra_codigo_fonte

               //Variáveis de controle para esse código iterar corretamente
               k+=j;
               k++;
               i++;
          }

     }

    arquivo_fonte.close(); // fecha o arquivo após usa-lo

  return 0;
}
