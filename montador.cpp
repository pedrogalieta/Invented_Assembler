#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <list>

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

//converte a string para maiúsculo
void str_upper(string &str){

  int i;
  int str_length = str.length();

  for(i = 0; i < str_length; i++){

    str[i] = toupper(str[i]);
  }
}

//verifica o próximo token a partir de pos_linha, e após fazê-lo, atualiza o valor dessa variável para o início da próxima leitura (> length em caso de último token)
string get_next(string &linha, int *pos_linha, int length_linha){

  string token;

  for (int i=*pos_linha; linha[i] != ' ' && i<length_linha; i++){

    *pos_linha = i+2;
    token += linha[i];
  }

  return token;
}

int check_int(string &operando){

  int i = 5;

  std::istringstream iss(operando);
  iss >> i;
  if (iss.eof() == false){

    return 0;
  }
  else {

    return 1;
  }
}

void output(ofstream &arq_obj, std::map<string, int> td_map, std::multimap<string, int> tu_map, std::list<int> cod_list, std::list<int> rel_list){

  //iteradores
  std::map<string, int>::iterator td_it;
  std::multimap<string, int>::iterator tu_it;
  std::list <int>::iterator l_it;
  std::list <int>::iterator r_it;

  arq_obj << "TABLE USE" << endl;

  for (tu_it = tu_map.begin(); tu_it != tu_map.end(); tu_it++){

    arq_obj << tu_it->first << ' ' << tu_it->second << endl;
  }

  arq_obj << "TABLE DEFINITION" << endl;

  for (td_it = td_map.begin(); td_it != td_map.end(); td_it++){

    arq_obj << td_it->first << ' ' << td_it->second << endl;
  }

  arq_obj << "RELATIVE" << endl;

  for (r_it = rel_list.begin(); r_it != rel_list.end(); r_it++){

    arq_obj << *r_it << ' ';
  }

  arq_obj << endl << "CODE" << endl;

  for (l_it = cod_list.begin(); l_it != cod_list.end(); l_it++){

    arq_obj << *l_it << ' ';
  }

}

void output_nom(ofstream &arq_obj, std::map<string, int> td_map, std::multimap<string, int> tu_map, std::list<int> cod_list, std::list<int> rel_list){

  //iteradores
  std::map<string, int>::iterator td_it;
  std::multimap<string, int>::iterator tu_it;
  std::list <int>::iterator l_it;
  std::list <int>::iterator r_it;

  for (l_it = cod_list.begin(); l_it != cod_list.end(); l_it++){

    arq_obj << *l_it << ' ';
  }

}

int pre_processamento(ifstream &arq_fonte ,fstream &arq_pre_processado){

  // Entradas:  arq_fonte = Arquivo assembly que vai ser pre processado
  // Saídas:    arq_pre_processado = Arquivo assembly já pre processado
  //            A função retorna um caso o pre processamento tenho sido sucesso e 0 caso não
  // Comentários: Essa função retira comentários do código fonte e trata as diretivas EQU e IF

  int i, length_linha, posicao, acumulador_erro, flag_rotulo, length_palavra, flag_IF, pular_proxima_linha = 0, contador_de_linha = 0;
  string linha, nova_linha, palavra, rotulo, valor_EQU;
  map<string, string> EQU_map; //mapa de EQU's definidos

  // Varre o arquivo fonte
  while(getline(arq_fonte,linha)){

    contador_de_linha++;
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

      if(linha[i] ==  '\t'){
        linha[i] = ' ';
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
      if(nova_linha[posicao+2] == 'E' && nova_linha[posicao+3] == 'Q' && nova_linha[posicao+4] == 'U'){
        // Checa se o EQU foi declarado sem argumento
        if(nova_linha[posicao+5] == ' ' ){
          for(i=posicao+6;i<length_linha;i++){
            valor_EQU+=nova_linha[i];
          }
        }else{
          cout<< "Linha "<< contador_de_linha << ": Erro sintático! EQU definido sem argumento" << endl;
          return 0;
        }
        // Checa se o EQU já foi declarado anteriormente
        if(EQU_map.find(rotulo) == EQU_map.end()){
          // Se não, adicionar no map e continuar o pre processamento
          EQU_map.insert(make_pair(rotulo,valor_EQU));
          valor_EQU.clear();
          rotulo.clear();
          nova_linha.clear();
          arq_pre_processado << endl;
          continue;
        } else {
          // Se sim, erro
          cout<< "Linha "<< contador_de_linha << ": Erro semântico! EQU redefinido" << endl;
          return 0;
        }
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
  return 1;
}
int primeira_passagem(fstream &arq_fonte, std::map <string, pair< pair<int, int>, pair<int, int> > > &i_map, std::map<string, pair <int, int > > &s_map, std::map<string, pair<int, int> > &d_map, std::map<string, int> &td_map){

  string linha;
  string mnemonico;
  string simbolo;
  string operando;
  int i, length_linha, pos_linha;
  int cont_linha = 0, cont_pos = 0;
  int label_detect;
  int section_flag = 0;
  int flag_erro = 0;
  int flag_begin = 0, flag_end = 0;

  //iteradores dos maps
  std::map <string, pair< pair<int, int>, pair<int, int> > >::iterator i_it;
  std::map<string, pair <int, int > >::iterator s_it;
  std::map<string, pair<int, int> >::iterator d_it;
  std::map<string, int>::iterator td_it;

  while(getline(arq_fonte, linha)){

    //cout << "Linha lida: " << linha << endl;

    length_linha = linha.length();
    //cout << "Tamanho: " << length_linha << endl;
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

      //procura no map
      s_it = s_map.find(simbolo);

      //se já existe = erro
      if(s_it != s_map.end()){

        cout << "Linha " << cont_linha << ": Erro semântico! Repetição do símbolo " << simbolo << endl;
        flag_erro = 1;
      }
      //se não, insere e verifica qual o mnemonico que segue
      else{

        s_map.insert(make_pair(simbolo, make_pair(cont_pos, 0)));
        //cout << "Endereço " << cont_pos << ": detecção do símbolo " << simbolo << endl;

        //se há apenas o rótulo na linha
        if(label_detect == length_linha -1){

          simbolo.clear();
          continue;
        }

        pos_linha = label_detect + 2;

        //busca próximo termo na linha
        mnemonico = get_next(linha, &pos_linha, length_linha);

        //converte para maiúsculo
        str_upper(mnemonico);

        //cout << "Mnemonico verificado: " << mnemonico << endl;

        //verifica se o mnemonico é instrucao
        i_it = i_map.find(mnemonico);

        //se for, aumenta o contador de posição de acordo com o tamanho da instrução
        if (i_it != i_map.end()){

          //verifica se a instrução é chamada numa section proibida (DATA/BSS)
          if (section_flag == 2 || section_flag == 3){

            cout << "Linha " << cont_linha << ": Instruções não permitidas nessa seção!" << endl;
            flag_erro = 1;
            simbolo.clear();
            mnemonico.clear();
            continue;
          }
          cont_pos += i_it->second.first.second;
        }
        //se não for instrução, vericica se é diretiva
        else{

          d_it = d_map.find(mnemonico);
          //se não for diretiva, erro
          if (d_it == d_map.end()){

            cout << "Linha " << cont_linha << ": Comando inválido!" << endl;
            flag_erro = 1;
          }
          //se for diretiva, incrementa o contador de posição de acordo com o tamanho dela
          else{

            if (mnemonico == "SPACE"){

              //verifica se SPACE é chamada na section TEXT ou DATA
              if (section_flag == 2 || section_flag == 1){

                cout << "Linha " << cont_linha << ": SPACE não permitida nessa seção!" << endl;
                flag_erro = 1;
                simbolo.clear();
                mnemonico.clear();
                continue;
              }

              //se existe operando
              if(pos_linha < length_linha){

                //verifica se o operando é um inteiro
                operando = get_next(linha, &pos_linha, length_linha);
                //se não for inteiro, erro
                if (check_int(operando) == 0){

                  cout << "Linha " << cont_linha << ": Erro léxico! O operando de SPACE deve ser um valor inteiro positivo!"<< endl;
                  flag_erro = 1;
                }
                //se for inteiro, verificar se é > 0
                else{

                  int aux = atoi(operando.c_str());

                  if(aux <= 0){

                    cout << "Linha " << cont_linha << ": Erro léxico! O operando de SPACE deve ser um valor inteiro positivo!"<< endl;
                    flag_erro = 1;
                  }
                  //se for > 0, reservar o espaço solicitado
                  else{

                    cont_pos += aux;
                    simbolo.clear();
                    mnemonico.clear();
                    operando.clear();
                    continue;
                  }
                }
              }
            }
            else if (mnemonico == "CONST"){

              //verifica se CONST é chamada em BSS ou TEXT
              if (section_flag == 1 || section_flag == 3){

                cout << "Linha " << cont_linha << ": CONST não permitida nessa seção!" << endl;
                flag_erro = 1;
                simbolo.clear();
                mnemonico.clear();
                continue;
              }

              // se há operando
              if (pos_linha < length_linha){
                //verifica se o operando é um inteiro
                operando = get_next(linha, &pos_linha, length_linha);
                //se não for inteiro, erro
                if (check_int(operando) == 0){

                  cout << "Linha " << cont_linha << ": Erro léxico! O operando de CONST deve ser um valor inteiro!"<< endl;
                  flag_erro = 1;
                }
              }
              //se não há Operandos
              else {

                cout << "Linha " << cont_linha << ": Erro sintático! CONST deve conter operando!"<< endl;
                flag_erro = 1;
              }
            }
            else if (mnemonico == "SECTION"){

              cout << "Linha " << cont_linha << ": Erro sintático! A diretiva SECTION não deve acompanhar símbolo!" << endl;
              flag_erro = 1;
            }
            else if(mnemonico == "EXTERN"){

              //devemos encontrar o simbolo inserido e alterar seu "extern"
              s_it = s_map.find(simbolo);
              s_it->second.first = 0;
              s_it->second.second = 1;
            }
            else if (mnemonico == "PUBLIC"){

              cout << "Linha " << cont_linha << ": Erro sintático! A diretiva PUBLIC não deve acompanhar símbolo!" << endl;
              flag_erro = 1;
            }
            else if (mnemonico == "BEGIN"){

              flag_begin = 1;
            }
            else if (mnemonico == "END"){

              flag_end = 1;
            }

            cont_pos += d_it->second.first;
          }
        }
      }
    }

    //se não há label, devemos identificar a primeira operação pelo ' '
    else{

      pos_linha = 0;

      mnemonico = get_next(linha, &pos_linha, length_linha);

      //converte para maiúsculo
      str_upper(mnemonico);

      //cout << "Mnemonico verificado: " << mnemonico << endl;

      //verifica se o mnemonico é instrucao
      i_it = i_map.find(mnemonico);

      //se for, aumenta o contador de posição de acordo com o tamanho da instrução
      if (i_it != i_map.end()){

        //verifica se a instrução é chamada numa section proibida (DATA/BSS)
        if (section_flag == 2 || section_flag == 3){

          cout << "Linha " << cont_linha << ": Instruções não permitidas nessa seção!" << endl;
          flag_erro = 1;
          simbolo.clear();
          mnemonico.clear();
          continue;
        }
        cont_pos += i_it->second.first.second;
      }
      //se não for instrução, vericica se é diretiva
      else{

        d_it = d_map.find(mnemonico);
        //se não for, erro
        if (d_it == d_map.end()){

          cout << "Linha " << cont_linha << ": Comando inválido!" << endl;
          flag_erro = 1;

        }
        //se for, chama subrotina e incrementa o contador de posição de acordo com o tamanho da diretiva
        else{

          if (mnemonico == "SPACE"){

            cout << "Linha " << cont_linha << ": Erro sintático! A diretiva SPACE deve acompanhar um símbolo!" << endl;
            flag_erro = 1;
          }
          else if (mnemonico == "CONST"){

            cout << "Linha " << cont_linha << ": Erro sintático! A diretiva CONST deve acompanhar um símbolo!" << endl;
            flag_erro = 1;
          }
          else if (mnemonico == "SECTION"){

            operando = get_next(linha, &pos_linha, length_linha);
            str_upper(operando);

            //dependendo do operando, a flag é setada com um valor diferente
            if ("TEXT" == operando){

              section_flag = 1;
            }
            else if ("DATA" == operando){

              section_flag = 2;
            }
            else if ("BSS" == operando){

              section_flag = 3;
            }
            //se o operando for diferente dos 3 acima, erro
            else{

              cout << "Linha " << cont_linha << ": Erro léxico! SECTION inválida!" << endl;
              flag_erro = 1;
            }

            cout << section_flag << endl;
          }
          else if (mnemonico == "EXTERN"){

            cout << "Linha " << cont_linha << ": Erro sintático! A diretiva EXTERN deve acompanhar um símbolo!" << endl;
            flag_erro = 1;
          }
          else if (mnemonico == "PUBLIC"){

            //se não há operando, Erro
            if(pos_linha > length_linha){

              cout << "Linha " << cont_linha << ": Erro sintático! Não há operando para a diretiva PUBLIC!" << endl;
              flag_erro = 1;
            }
            operando = get_next(linha, &pos_linha, length_linha);
            str_upper(operando);
            //cout << "Operando verificado: " << operando << endl;

            td_map.insert(make_pair(operando, 0));
          }
          else if (mnemonico == "BEGIN"){

            flag_begin = 1;
          }
          else if (mnemonico == "END"){

            flag_end = 1;
          }

          cont_pos += d_it->second.first;

        }
      }
    }

    simbolo.clear();
    mnemonico.clear();
    operando.clear();
  }
  //completa a tabela de definição a partir da de símbolos
  for(td_it = td_map.begin(); td_it != td_map.end(); td_it++){

    simbolo = td_it->first;
    //cout << "Simbolo de td_map: " << simbolo << endl;

    //procura o símbolo da tabela de definição, na tabela de símbolos
    s_it = s_map.find(simbolo);
    //se não achou, erro
    if(s_it == s_map.end()){

      cout << "Erro semântico! Simbolo da tabela de definição não consta na de símbolos" << endl;
      flag_erro = 1;
    }
    else{

      td_it->second = s_it->second.first;
    }
  }

  //se flag_begin e flag_end são diferentes, algum deles deixou de ser declarado
  if(flag_begin == 1 && flag_end == 0){

    cout << "Erro semântico! Esperava-se directiva END!" << endl;
    flag_erro = 1;
  }
  else if(flag_begin == 0 && flag_end == 1){

    cout << "Erro semântico! Esperava-se directiva BEGIN!" << endl;
    flag_erro = 1;
  }
  //cout << "Flag erro: " << flag_erro << endl;
  //cout << "Flag begin: " << flag_begin << endl;
  //cout << "End flag: " << flag_end << endl;

  //DEFINIÇÃO DO RETORNO DA FUNÇÃO: -1 == Erro, 0 == CÓDIGO SIMPLES, 1 == MÓDULO
  //se há erro
  if(flag_erro == 1){

    return -1;
  }
  //se não há, verifica se é um módulo
  else{

    if(flag_begin == 0 && flag_end == 0){

      return 0;
    }
    else if(flag_begin == 1 && flag_end == 1){

      return 1;
    }
  }
}

int segunda_passagem(fstream &arq_fonte, std::map <string, pair< pair<int, int>, pair<int, int> > > &i_map, std::map<string, pair <int, int > > &s_map, std::map<string, pair<int, int> > &d_map, std::map<string, int> &td_map, std::multimap<string, int> &tu_map, std::list<int> &cod_list, std::list<int> &rel_list){

  string linha;
  string mnemonico;
  string simbolo;
  string operando;
  int i, length_linha, pos_linha;
  int cont_linha = 0, cont_pos = 0;
  int label_detect = 0;
  int section_flag = 0;
  int flag_erro = 0;

  //iteradores dos maps
  std::map <string, pair< pair<int, int>, pair<int, int> > >::iterator i_it;
  std::map<string, pair <int, int > >::iterator s_it;
  std::map<string, pair<int, int> >::iterator d_it;
  std::map<string, int>::iterator td_it;
  std::multimap<string, int>::iterator tu_it;

  cout << endl;
  cout << "SEGUNDA PASSAGEM" << endl;

  while(getline(arq_fonte, linha)){

    length_linha = linha.length();
    label_detect = 0;
    cont_linha++;
    pos_linha = 0;

    if(length_linha == 0) continue;

    //cout << "Linha lida: " << linha << endl;

    //varre a linha procurando ':'
    for(i=0; i<length_linha; i++){

      if(linha[i] == ':'){

        label_detect = i;
        break;
      }
    }

    //se há um label, nessa passagem ele deve ser ignorado
    if (label_detect != 0){

      simbolo = get_next(linha, &pos_linha, length_linha);

      //se há apenas o rótulo na linha
      if(label_detect == length_linha -1){

        simbolo.clear();
        continue;
      }
    }

    //verificar o mnemonico em questão
    //as diretivas são todas trabalhadas na primeira passagem, então a segunda é voltada apenas às instruções
    mnemonico = get_next(linha, &pos_linha, length_linha);

    //busca a diretiva no mapa
    d_it = d_map.find(mnemonico);
    //se for diretiva, apenas CONST e SPACE contribuem para o .obj
    if (d_it != d_map.end()){

      //se for CONST, apenas pega o operando (verificou-se anteriormente caso não exista/seja de tipo inválido)
      if (mnemonico == "CONST"){

        operando = get_next(linha, &pos_linha, length_linha);
        cod_list.push_back(atoi(operando.c_str()));
      }
      else if (mnemonico == "SPACE"){

        //se existe operando
        if(pos_linha < length_linha){

          //pega o operando, cujo tipo já foi verificado
          operando = get_next(linha, &pos_linha, length_linha);
          //insere na lista e atualiza o contador
          for (int i = 0; i<atoi(operando.c_str()); i++){

            cod_list.push_back(0);
          }
          cont_pos += atoi(operando.c_str());
          continue;
        }
        else {

          cod_list.push_back(0);
        }
      }
      cont_pos += d_it->second.first;
    }
    //caso não seja diretiva, verifica se é instrução
    else{

      i_it = i_map.find(mnemonico);
      //se também não for instrução, a operação é inválida
      if(i_it == i_map.end()){

        cout << "Linha " << cont_linha << ": Erro sintático! Operação inválida!" << endl;
        flag_erro = 1;
      }
      //caso seja instrução, tratar de acordo com o número de operandos
      else{

        //2 operandos
        if (mnemonico == "COPY"){

          //se não houver operandos, erro
          if (pos_linha > length_linha){

            cout << "Linha " << cont_linha << ": Erro sintático! A instrução COPY deve receber dois operandos!" << endl;
            flag_erro = 1;
            continue;
          }
          //get primeiro operando
          operando = get_next(linha, &pos_linha, length_linha);

          //se não houver o segundo operando, erro
          if (pos_linha > length_linha){

            cout << "Linha " << cont_linha << ": Erro sintático! A instrução COPY deve receber dois operandos!" << endl;
            flag_erro = 1;
            continue;
          }

          //se não há vírgula após o primeiro operando, Erro
          if (*(operando.end() - 1) != ','){

            cout << "Linha " << cont_linha << ": Erro sintático! Operandos separados incorretamente!" << endl;
            flag_erro = 1;
            continue;
          }
          //salva o primeiro operando em buffer auxiliar
          operando.erase(operando.end()-1);
          string aux = operando;
          //get segundo operando
          operando = get_next(linha, &pos_linha, length_linha);

          //se ainda houver operando, erro
          if (pos_linha < length_linha){

            cout << "Linha " << cont_linha << ": Erro sintático! A instrução COPY deve receber dois operandos!" << endl;
            flag_erro = 1;
            continue;
          }

          //tendo coletado dois operandos, verificamos se são válidos
          s_it = s_map.find(aux);
          if (s_it == s_map.end()){

            cout << "Linha " << cont_linha << ": Erro semântico! Operando 1 não é um símbolo definido!" << endl;
            flag_erro = 1;
            continue;
          }
          //inserção de opcode e endereço do operando na lista de construção do código, e endereço na de relativos
          cod_list.push_back(i_it->second.first.first);
          cod_list.push_back(s_it->second.first);
          rel_list.push_back((cont_pos + 1));

          s_it = s_map.find(operando);
          if (s_it == s_map.end()){

            cout << "Linha " << cont_linha << ": Erro semântico! Operando 2 não é um símbolo definido!" << endl;
            flag_erro = 1;
            continue;
          }
          //inserção do endereço do operando na lista de construção do código e endereço na de relativos
          cod_list.push_back(s_it->second.first);
          rel_list.push_back((cont_pos + 2));
        }
        //sem operando
        else if (mnemonico == "STOP"){

          //se get next retornou valor < length_linha, ainda há o que ser lido, ou seja, operandos
          if (pos_linha < length_linha){

            cout << "Linha " << cont_linha << ": Erro sintático! A instrução STOP não deve receber operandos!";
            flag_erro = 1;
          }

          cod_list.push_back(i_it->second.first.first);
        }
        //1 operando
        else{

          operando = get_next(linha, &pos_linha, length_linha);
          //se get next retornou valor < length_linha, ainda há o que ser lido, ou seja, operandos
          if (pos_linha < length_linha){

            cout << "Linha " << cont_linha << ": Erro sintático! A instrução " << mnemonico << " deve receber apenas um operando!" << endl;
            flag_erro = 1;
            continue;
          }
          //verifica se o operando é um símbolo definido
          s_it = s_map.find(operando);
          if (s_it == s_map.end()){

            cout << "Linha " << cont_linha << ": Erro semântico! Operando não é um símbolo definido!" << endl;
            flag_erro = 1;
          }
          else{

            //inserção de opcode e endereço do operando na lista de construção do código
            cod_list.push_back(i_it->second.first.first);
            cod_list.push_back(s_it->second.first);
            //inserção do endereço na lista de relativos
            rel_list.push_back((cont_pos +1));

            if(s_it->second.second == 1){

              tu_map.insert(make_pair(operando, (cont_pos + 1)));
            }
          }
        }
        cont_pos += i_it->second.first.second;
      }
    }
  }
  //Retorno da segunda passagem: 0 == ok e 1 == erro
  return flag_erro;
}

int main(int argc, char const *argv[]) {

  int flag_primeira;
  int flag_segunda;

  string concatenador;
  ifstream arq_fonte;
  fstream arq_pre_processado;
  ofstream arq_obj;

  //mapa de instruções: <Instrução, < <Opcode, Tamanho>, <Operandos, 0> > >
  std::map <string, pair< pair<int, int>, pair<int, int> > > i_map;
  //mapa de símbolos: <Símbolo, <Endereço, Extern>
  std::map<string, pair <int, int > > s_map;
  //mapa de diretivas: <Diretiva, <Tamanho, Operandos> >
  std::map<string, pair<int, int> > d_map;
  //mapa da tabela de definição: <Símbolo, Endereço>
  std::map<string, int> td_map;
  //mapa da tabela de uso: <Símbolo (extern == 1), Endereço de uso>
  std::multimap<string, int> tu_map;

  //iteradores
  std::map <string, pair< pair<int, int>, pair<int, int> > >::iterator i_it;
  std::map<string, pair <int, int > >::iterator s_it;
  std::map<string, pair<int, int> >::iterator d_it;
  std::map<string, int>::iterator td_it;

  //lista para armazenar os códigos do arquivo objeto
  std::list<int> cod_list;
  //lista para armazenar os endereços que são relativos
  std::list<int> rel_list;

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
  if(pre_processamento(arq_fonte, arq_pre_processado) == 0){
    return -1;
  }

  arq_fonte.close();
  arq_pre_processado.close();

  arq_pre_processado.open(&concatenador[0],ios::in);

  if(!arq_pre_processado.is_open()){

    cout << "Impossível abrir arquivo solicitado!";
    return -1;
  }

  flag_primeira = primeira_passagem(arq_pre_processado, i_map, s_map, d_map, td_map);
  //cout << "Flag da primeira: " << flag_primeira << endl;
  arq_pre_processado.close();

  if(flag_primeira == -1){

    cout << "Erro verificado durante a montagem! Operação abortada." << endl;
    return 0;
  }

  arq_pre_processado.open(&concatenador[0],ios::in);

  if(!arq_pre_processado.is_open()){

    cout << "Impossível abrir arquivo solicitado!";
    return -1;
  }

  flag_segunda = segunda_passagem(arq_pre_processado, i_map, s_map, d_map, td_map, tu_map, cod_list, rel_list);
  //cout << "Flag da segunda: " << flag_segunda << endl;
  arq_pre_processado.close();

  //INSERÇÂO DOS DADOS NO ARQUIVO OBJETO
  //se há erro
  if(flag_segunda == 1){

    cout << "Erro verificado durante a montagem! Operação abortada." << endl;
    return 0;
  }
  else{

    concatenador.clear();
    concatenador += argv[1];
    concatenador += ".obj";
    arq_obj.open(&concatenador[0]);

    //se não é um módulo
    if(flag_primeira == 0){

      output_nom(arq_obj, td_map, tu_map, cod_list, rel_list);
    }
    //se é módulo
    else if(flag_primeira == 1){

      output(arq_obj, td_map, tu_map, cod_list, rel_list);
    }
  }
  arq_obj.close();


  //TESTE DO CONTEÚDO DE S_MAP
  cout << endl << "Conteúdo do s_map:" << endl;
  for (std::map<string, pair <int, int > >::iterator it = s_map.begin(); it != s_map.end(); it++){

    cout << it->first << ' ' << it->second.first << ' ' << it->second.second <<  endl;
  }

  //TESTE DO CONTEÚDO DE TD_MAP
  cout << endl << "Conteúdo do td_map:" << endl;
  for (std::map<string, int>::iterator it = td_map.begin(); it != td_map.end(); it++){

    cout << it->first << ' ' << it->second <<  endl;
  }

  //TESTE DO CONTEÚDO DE TU_MAP
  cout << endl << "Conteúdo do tu_map:" << endl;
  for (std::multimap<string, int>::iterator it = tu_map.begin(); it != tu_map.end(); it++){

    cout << it->first << ' ' << it->second <<  endl;
  }

  return 0;
}
