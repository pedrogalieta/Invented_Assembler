# Tradutor_Assembly_Hipotetico
Programadores:  Pedro Saman D N Cesarino   15/0144890
                Pedro Victor Galieta Tomaz 15/0144938

O trabalho consistiu em implementar dois algoritmos separados: O montador e o ligador.

O montador: exemplo de uso "./montador arquivo_assembly"
            Entradas: Ele recebe como entrada na linha de comando um arquivo no formato assembly (sem a extensão assumesse que é .asm) inventado utilizado na sala de aula.  
            Saídas:   São geradas duas saídas neste programa ambos com o mesmo nome do arquivo ".asm":
                            1) arquivo_assembly.pre: É o arquivo pré processado, isto é, sem os comentários e com as diretivas EQU e IF já avaliadas.
                            2) arquivo_assembly.obj: É o arquivo objeto, se o programa não for um módulo ele será apenas uma linha de números com o código máquina, e, se for um módulo, será constituído da tabela de uso, tabela de definições, lista de relativos e o código em si.

O ligador: exemplo de uso "./ligador arquivo_objeto_1 arquivo_objeto_2"
            Entradas: Ele recebe como entrada na linha de comando N arquivos no formato objeto referentes ao assembly utilizado (sem a extensão assumesse que é .obj).
            Saídas:   É gerado apenas uma saída ".e":
                            1) arquivo_objeto_1.e: É o arquivo executável pronto para ser carregado, ele recebe o nome do primeiro arquivo objeto mandado na linha de comando.

Exemplo de uso dos dois algoritmos:
  - Na mesma pasta com os arquivos ligador e montador deve haver todos os códigos assembly a serem traduzidos.
  - Existem três códigos assembly para serem montados e ligados nesta ordem: moduloA.asm, moduloB.asm e moduloC.asm
  - Digitar na linha de comando: "./montador moduloA"
                                 "./montador moduloB"
                                 "./montador moduloC"
  - Os seguintes arquivos serão criados: moduloA.pre, moduloA.obj, moduloC.pre, moduloC.obj, moduloC.pre e moduloC.obj
  - Então digitar na linha de comando:  "./ligador moduloA moduloB moduloC"
  - Por fim, aparecerá o arquivo: moduloA.e
