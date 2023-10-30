MVI x1, -3.14f
MVI x2, 9.81f

MVI x3, 15
MVI x4, -5

; Not equal
LOAD_ACC x1
BNE x2, 12

MVI x5, 10
JMP 8

MVI x5, 20

; Float less than
LOAD_ACC x1
BLTF x2, 12

MVI x6, 30
JMP 8

MVI x6, 40

; Integer greater or equal
LOAD_ACC x4
BGE x3, 12

MVI x7, 50
JMP 8

MVI x7, 60


LOAD_ACC x5
CALL_INTRINSIC PRINT

LOAD_ACC x6
CALL_INTRINSIC PRINT

LOAD_ACC x7
CALL_INTRINSIC PRINT

RET
