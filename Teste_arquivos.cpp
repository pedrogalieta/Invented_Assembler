#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){

     ifstream arquivo_fonte;		        //cria o objeto "arquivo_fonte" da classe ifstream - leitura
     arquivo_fonte.open("programa.cpp");

     if (arquivo_fonte.is_open() && arquivo_fonte.good()){

        cout << "Arquivo aberto com sucesso \n"

        arq.close(); // fecha o arquivo após usa-lo
    }

  return 0;
}
