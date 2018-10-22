<<<<<<< HEAD:programa.asm
;Código fonte do módulo A:
MOD_A:  BEGIN
Y:      EXTERN
MOD_B:  EXTERN
PUBLIC  VAL
PUBLIC  L1
INPUT   Y
LOAD    VAL
ADD     Y
STORE   Y
JMPP    MOD_B
L1:     STOP
VAL:    CONST   5
END
=======
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
>>>>>>> a94285a970f1024c56f40f3d2ad23bde201b04a2:Exemplos/programa.asm
