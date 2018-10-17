#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>

using namespace std;

void cria_map_instrucoes(std::map <string, pair< pair<int, int>, pair<int, int> > > &i_map){

  i_map.insert(make_pair("ADD", make_pair(make_pair(1, 2), make_pair(1, 0))));
  i_map.insert(make_pair("SUB", make_pair(make_pair(2, 2), make_pair(1, 0))));
  i_map.insert(make_pair("MULT", make_pair(make_pair(3, 2), make_pair(1, 0))));
  i_map.insert(make_pair("DIV", make_pair(make_pair(4, 2), make_pair(1, 0))));
  i_map.insert(make_pair("JMP", make_pair(make_pair(5, 2), make_pair(1, 0))));
  i_map.insert(make_pair("JMPN", make_pair(make_pair(6, 2), make_pair(1, 0))));
  i_map.insert(make_pair("JMPP", make_pair(make_pair(7, 2), make_pair(1, 0))));
  i_map.insert(make_pair("JMPZ", make_pair(make_pair(8, 2), make_pair(1, 0))));
  i_map.insert(make_pair("COPY", make_pair(make_pair(9, 3), make_pair(2, 0))));
  i_map.insert(make_pair("LOAD", make_pair(make_pair(10, 2), make_pair(1, 0))));
  i_map.insert(make_pair("STORE", make_pair(make_pair(11, 2), make_pair(1, 0))));
  i_map.insert(make_pair("INPUT", make_pair(make_pair(12, 2), make_pair(1, 0))));
  i_map.insert(make_pair("OUTPUT", make_pair(make_pair(13, 2), make_pair(1, 0))));
  i_map.insert(make_pair("STOP", make_pair(make_pair(14, 1), make_pair(0, 0))));
}

void cria_map_diretivas(std::map<string, pair<int, int> > &d_map){

  d_map.insert(make_pair("SECTION", make_pair(0, 1)));
  d_map.insert(make_pair("SPACE", make_pair(1, 1)));
  d_map.insert(make_pair("CONST", make_pair(1, 1)));
  d_map.insert(make_pair("PUBLIC", make_pair(0, 0)));
  d_map.insert(make_pair("EQU", make_pair(0, 1)));
  d_map.insert(make_pair("IF", make_pair(0, 1)));
  d_map.insert(make_pair("EXTERN", make_pair(0, 0)));
  d_map.insert(make_pair("BEGIN", make_pair(0, 0)));
  d_map.insert(make_pair("END", make_pair(0, 0)));
}

void str_upper(string &str){

  int i;
  int str_length = str.length();

  for(i = 0; i < str_length; i++){

    str[i] = toupper(str[i]);
  }
}

void pre_processamento(ifstream &arq_fonte ,fstream &arq_pre_processado){

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
      arq_pre_processado << endl;
      continue;
    }

    // Esse if faz com que o pre processador ignore a próxima linha caso tenha um IF 0 antes dela
    if(pular_proxima_linha == 1){
      pular_proxima_linha = 0;
      arq_pre_processado << endl;
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
        arq_pre_processado << endl;
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
      arq_pre_processado << endl;
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

void primeira_passagem(fstream &arq_fonte, std::map <string, pair< pair<int, int>, pair<int, int> > > &i_map, std::map<string, int> &s_map, std::map<string, pair<int, int> > &d_map){

  string linha;
  string mnemonico;
  string simbolo;
  int i;
  int length_linha;
  int cont_linha = 0, cont_pos = 0;
  int label_detect;

  //iteradores dos maps
  std::map <string, pair< pair<int, int>, pair<int, int> > >::iterator i_it;
  std::map<string, int>::iterator s_it;
  std::map<string, pair<int, int> >::iterator d_it;

  while(getline(arq_fonte, linha)){

    cout << "Linha lida: " << linha << endl;

    length_linha = linha.length();
    label_detect = 0;
    cont_linha++;

    if(length_linha == 0 || linha[0] == ';') continue;

    //varre a linha procurando ':'
    for(i=0; i<length_linha; i++){

      if(linha[i] == ':'){

        label_detect = i;
        break;
      }
    }

    //se há label, devemos identificá-lo pelo ':'
    if(label_detect != 0){

      //identificacao do label
      for(i=0; i < label_detect; i++){

        simbolo += linha[i];
      }

      //converte para maiusculo
      str_upper(simbolo);

      //se já existe = erro
      if(s_map.find(simbolo) != s_map.end()){

        cout << "Linha " << cont_linha << ": ERRO! Repetição do símbolo " << simbolo << endl;
        exit(0);
      }
      //se não, insere e verifica qual o mnemonico que segue
      else{

        s_map.insert(make_pair(simbolo, cont_pos));
        cout << "Endereço " << cont_pos << ": detecção do símbolo " << simbolo << endl;

        //se há apenas o rótulo na linha
        if(label_detect == length_linha -1){

          simbolo.clear();
          continue;
        }

        for(i = (label_detect + 2); linha[i] != ' ' && i < length_linha; i++){

            mnemonico += linha[i];
        }

        //converte para maiúsculo
        str_upper(mnemonico);

        cout << "Mnemonico verificado: " << mnemonico << endl;

        //verifica se o mnemonico é instrucao
        i_it = i_map.find(mnemonico);

        //se for, aumenta o contador de posição de acordo com o tamanho da instrução
        if (i_it != i_map.end()){

          cont_pos += i_it->second.first.second;
        }
        //se não for, vericica se é diretiva
        else{

          d_it = d_map.find(mnemonico);
          //se for, incrementa o contador de posição de acordo com o tamanho da diretiva
          if (d_it != d_map.end()){

            cont_pos += d_it->second.first;
          }
          //se não for, erro
          else{

            cout << "Linha " << cont_linha << ": Comando inválido!" << endl;
            exit(1);
          }
        }
      }
    }


    //se não há label, devemos identificar a primeira instrução pelo ' '
    else{

      for(i = 0; linha[i] != ' ' && i < length_linha; i++){

          mnemonico += linha[i];
      }

      //converte para maiúsculo
      str_upper(mnemonico);

      cout << "Mnemonico verificado: " << mnemonico << endl;

      //verifica se o mnemonico é instrucao
      i_it = i_map.find(mnemonico);

      //se for, aumenta o contador de posição de acordo com o tamanho da instrução
      if (i_it != i_map.end()){

        cont_pos += i_it->second.first.second;
      }
      //se não for, vericica se é diretiva
      else{

        d_it = d_map.find(mnemonico);
        //se for, incrementa o contador de posição de acordo com o tamanho da diretiva
        if (d_it != d_map.end()){

          cont_pos += d_it->second.first;
        }
        //se não for, erro
        else{

          cout << "Linha " << cont_linha << ": Comando inválido!" << endl;
          exit(2);
        }
      }
    }

    simbolo.clear();
    mnemonico.clear();
  }
}

int main(int argc, char const *argv[]) {

  string concatenador;
  ifstream arq_fonte;
  fstream arq_pre_processado;
  //mapa de instruções: <Instrução, < <Opcode, Tamanho>, <Operandos, 0> > >
  std::map <string, pair< pair<int, int>, pair<int, int> > > i_map;
  //mapa de símbolos: <Símbolo, Endereço>
  std::map<string, int> s_map;
  //mapa de diretivas: <Diretiva, <Tamanho, Operandos> >
  std::map<string, pair<int, int> > d_map;
  //iteradores
  std::map <string, pair< pair<int, int>, pair<int, int> > >::iterator i_it;
  std::map<string, int>::iterator s_it;
  std::map<string, pair<int, int> >::iterator d_it;

  cria_map_instrucoes(i_map);
  cria_map_diretivas(d_map);

  concatenador += argv[1];
  concatenador += ".asm";
  arq_fonte.open(&concatenador[0]);
  concatenador.clear();

  concatenador += argv[1];
  concatenador += ".pre";
  arq_pre_processado.open(&concatenador[0],ios::out);

  if(!arq_fonte.is_open() || !arq_pre_processado.is_open()){

    cout << "Impossível abrir arquivo solicitado!";
    return -1;
  }

  // Passagem de pre processamento
  pre_processamento(arq_fonte, arq_pre_processado);

  arq_fonte.close();
  arq_pre_processado.close();

  arq_pre_processado.open(&concatenador[0],ios::in);

  if(!arq_pre_processado.is_open()){

    cout << "Impossível abrir arquivo solicitado!";
    return -1;
  }

  primeira_passagem(arq_pre_processado, i_map, s_map, d_map);

  arq_pre_processado.close();


  //TESTE DO CONTEÚDO DE S_MAP
  cout << endl << "Conteúdo do s_map:" << endl;
  for (map<string,int>::iterator it = s_map.begin(); it != s_map.end(); it++){

    cout << it->first << ' ' << it->second <<  endl;
  }

  return 0;
}
