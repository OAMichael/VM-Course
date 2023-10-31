
FUNC MAIN: 0

    CALL_INTRINSIC SCANF
    STORE_ACC x1            ; a in x1

    CALL_INTRINSIC SCANF
    STORE_ACC x2            ; b in x2

    CALL_INTRINSIC SCANF
    STORE_ACC x3            ; c in x3

    MULF x1                 ; a*c in acc
    MULI 4.0                ; 4*a*c in acc
    STORE_ACC x4            ; 4*a*c in x4

    LOAD_ACC x2             ; b in acc
    MULF x2                 ; b*b in acc
    SUBF x4                 ; D = b*b - 4*a*c in acc

    MVI x6, 0.0             ; 0 in x6

    BLTF x6, 100            ; D < 0

    BEQ x6, 72              ; D == 0

    ; D > 0
    SQRT                    ; sqrt(D) in acc
    STORE_ACC x6            ; sqrt(D) in x6

    LOAD_ACCI -2.0          ; -2 in acc
    MULF x1                 ; -2*a in acc
    STORE_ACC x7            ; -2*a in x7

    LOAD_ACC x6             ; sqrt(D) in acc
    DIVF x7                 ; sqrt(D) / (-2 * a) in acc
    STORE_ACC x6            ; sqrt(D) / (-2 * a) in x6

    LOAD_ACC x2             ; b in acc
    DIVF x7                 ; b / (-2 * a) in acc
    STORE_ACC x7            ; b / (-2 * a) in x7

    ADDF x6                 ; x_1

    CALL_INTRINSIC PRINTF   ; print x_1

    LOAD_ACC x7             ; b / (-2 * a) in acc
    SUBF x6                 ; x_2

    CALL_INTRINSIC PRINTF   ; print x_2


    JMP 28

    ; D == 0
    LOAD_ACCI -2.0          ; -2 in acc
    MULF x1                 ; -2*a in acc
    STORE_ACC x7            ; -2*a in x7

    LOAD_ACC x2             ; b in acc
    DIVF x7                 ; b / (-2 * a) in acc

    CALL_INTRINSIC PRINTF   ; print x_2

    RET
