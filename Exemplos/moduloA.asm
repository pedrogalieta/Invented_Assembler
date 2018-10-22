;Código fonte do módulo A:
MOD_A:  BEGIN
SECTION TEXT
            Y:      EXTERN
            MOD_B:  EXTERN
            X:      EXTERN
            MOD_C:  EXTERN
            PUBLIC  VAL
            PUBLIC  VAL2
            PUBLIC  L1
            INPUT   Y
            LOAD    VAL
            ADD     Y
            STORE   Y + 2
            INPUT   X
            LOAD    VAL2
            STORE   X
            JMPP    MOD_B
            JMPN    MOD_C
            L1:     STOP
SECTION DATA
            VAL:    CONST   5
            VAL2:   CONST   10
END
