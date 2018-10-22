MOD_A: BEGIN
SECTION TEXT
              MOD_B: EXTERN
              PUBLIC FAT
              PUBLIC N
              INPUT N
              LOAD N
              FAT: SUB ONE
              JNPZ FIM
              JMP MOD_A
              FIM: OUTPUT N
              STOP
SECTION DATA
              ONE: CONST 1
SECTION BSS
              N: SPACE
END
