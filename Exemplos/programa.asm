Pinda:  EQU   1
Pinda:  EQU          1
TAM2: EQU 1

TAM: EQU 0
BMDS: EQU 2
SECTION               TEXT
ROT: INPUT N1;comentario
COPY N1, N4 ;comentario qualquer
ROT2:
COPY N2, N3
COPY N3, N3 + 1
OUTPUT N3 + 1
IF TAM
STOP
;comentario qualuer
SECTIO DATA
N2:  CONST -5
           SECTION BSS
N1:  SPACE Pinda
N3:       SPACE      BMDS
N4: SPACE
