Pinda: EQU 0
TAM: EQU 1
BMDS: EQU 2
SECTION               TEXT
ROT: INPUT N1;comentario
COPY N1, N4 ;comentario qualquer
ROT2:
COPY N2, N3
COPY N3, N3 + 1
IF TAM
OUTPUT N3 + 1
STOP
IF Pinda
;comentario qualuer
SECTIO DATA
N2:  CONST -5
           SECTION BSS
N1:  SPACE Pinda
N3:       SPACE      BMDS
OUTPUT TAM BMDS TAM TAM TAM TAM         Pinda
