    
FUNC MAIN: 0

    MVI x1, -3.14f
    MVI x2, 9.81f

    MVI x3, 15
    MVI x4, -5

    ; Not equal
    LOAD_ACC x1
    BNE x2, LABEL_0

    MVI x5, 10
    JMP LABEL_1

LABEL_0:
    MVI x5, 20

LABEL_1:
    ; Float less than
    LOAD_ACC x1
    BLTF x2, LABEL_2

    MVI x6, 30
    JMP LABEL_3

LABEL_2:
    MVI x6, 40

LABEL_3:
    ; Integer greater or equal
    LOAD_ACC x4
    BGE x3, LABEL_4

    MVI x7, 50
    JMP LABEL_5

LABEL_4:
    MVI x7, 60

LABEL_5:
    LOAD_ACC x5
    CALL_INTRINSIC PRINT

    LOAD_ACC x6
    CALL_INTRINSIC PRINT

    LOAD_ACC x7
    CALL_INTRINSIC PRINT

    RET
