#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// int checa_abertura_dos_arquivos(){
//
//      if (arquivo_fonte.is_open() && arquivo_fonte.good()){ //Checa se o arquivo de interesse foi aberto corretamente
//
//         cout << "Arquivo Fonte aberto com sucesso \n";
//
//     }else{
//       cout << "Arquivo Fonte não foi achado \nFinalizando programa";
//       return -1;
//     }
//
//     if (tabela_de_simbolos.is_open()){
//          cout << "Tabela de simbolos aberta com sucesso \n";
//
//     }else{
//       cout << "Tabela de simbolos não aberta corretamente \nFinalizando programa";
//       return -1;
//     }
//
//     if (tabela_de_diretivas.is_open() && tabela_de_diretivas.good()){
//          cout << "Tabela de diretivas aberta com sucesso \n";
//
//     }else{
//       cout << "Tabela de diretivas não aberta corretamente \nFinalizando programa";
//       return -1;
//     }
//
//     if (tabela_de_instrucoes.is_open() && tabela_de_instrucoes.good()){
//          cout << "Tabela de instrucoes aberta com sucesso \n";
//
//     }else{
//       cout << "Tabela de instrucoes não aberta corretamente \nFinalizando programa";
//       return -1;
//     }
// }

// int checa_abertura_dos_arquivos(ifstream programa, fstream simbolos, ifstream diretivas, ifstream instrucoes){
//
//      if (programa.is_open() && programa.good()){ //Checa se o arquivo de interesse foi aberto corretamente
//
//         cout << "Arquivo Fonte aberto com sucesso \n";
//
//     }else{
//       cout << "Arquivo Fonte não foi achado \nFinalizando programa";
//       return -1;
//     }
//
//     if (simbolos.is_open() && simbolos.good()){
//          cout << "Tabela de simbolos aberta com sucesso \n";
//
//     }else{
//       cout << "Tabela de simbolos não aberta corretamente \nFinalizando programa";
//       return -1;
//     }
//
//     if (diretivas.is_open() && diretivas.good()){
//          cout << "Tabela de diretivas aberta com sucesso \n";
//
//     }else{
//       cout << "Tabela de diretivas não aberta corretamente \nFinalizando programa";
//       return -1;
//     }
//
//     if (instrucoes.is_open() && instrucoes.good()){
//          cout << "Tabela de instrucoes aberta com sucesso \n";
//
//     }else{
//       cout << "Tabela de instrucoes não aberta corretamente \nFinalizando programa";
//       return -1;
//     }
//
//     return 1;
// }

int main(){

     string palavra_codigo_fonte;
     string palavra_da_tabela;
     string auxiliar;
     string new_line = "\n";


     ifstream arquivo_fonte;		        //Cria os objetos para cada arquivo para leitura
     ifstream tabela_de_diretivas;
     ifstream tabela_de_instrucoes;
     fstream tabela_de_simbolos;           //Cria o objeto para a tabela de simbolos para leitura e escrita
     arquivo_fonte.open("programa.asm");   //Abre os arquivos para o codigo fonte e as tabelas de simbolos, instruções e diretivas
     tabela_de_simbolos.open("tabela_de_simbolos.txt", fstream::app | fstream::in);
     tabela_de_instrucoes.open("tabela_de_instrucoes.txt");
     tabela_de_diretivas.open("tabela_de_diretivas.txt");


     // if (!(checa_abertura_dos_arquivos())){
     //       return -1;
     // }
    //
     while(!arquivo_fonte.fail()){ //Itera sobre o arquivo assembly
          getline(arquivo_fonte,palavra_codigo_fonte,' '); //Pega palavra por palavra do arquivo fonte
          tabela_de_simbolos.open("tabela_de_simbolos.txt", fstream::app | fstream::in);
          while(!tabela_de_simbolos.fail()){
             getline(tabela_de_simbolos,palavra_da_tabela, ' ');
             if(palavra_da_tabela == new_line){
                  // Se palavra_da_tabela for igual a \n então não devemos comparar com nada
             }
             else if((palavra_da_tabela == palavra_codigo_fonte)){
                  cout << "A palavra " << palavra_da_tabela << " está no arquivo tabela de simbolos\n";
               }
         }
         tabela_de_simbolos.close();
    }

    arquivo_fonte.close(); // fecha o arquivo após usa-lo

  return 0;
}
