#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int main () {

  string linha;
  string palavra;
  int i;
  int length_linha;
  ifstream arq_fonte;
  std::map<string, pair<int,int> > i_map; //mapa de instruções


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

  if(!arq_fonte.is_open()){

    cout << "Impossível abrir arquivo solicitado!";
    exit(1);
  }

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

            cout << palavra << ' ' << i_map.find(palavra)->second.first << endl;
          }
          else{

            cout << palavra << ' ' << "não existente!" << endl;
          }
        }

        palavra.clear();
      }

      //fim de linha (e de uma palavra)
      else if(i == (length_linha - 1)){

        if (i_map.find(palavra) != i_map.end()){

          cout << palavra << ' ' << i_map.find(palavra)->second.first << endl;
        }
        else{

          cout << palavra << ' ' << "não existente!" << endl;
        }

        palavra.clear();
      }
    }
  }

  arq_fonte.close();

  /*TESTE DO CONTEÚDO DE I_MAP
  cout << "Conteúdo do i_map:" << endl;
  for (map<string, pair<int, int> >::iterator it = i_map.begin(); it != i_map.end(); it++){

    cout << it->first << ' ' << it->second.first << ' ' << it->second.second <<  endl;
  }*/

  return 0;
}
