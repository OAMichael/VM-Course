FUNC FACTORIAL: 1

    LOAD_ACC x0             ; N in acc
    MVI x1, 1               ; 1 in x1
    BGT x1, CALCULATE_FACT  ; N > 1

    LOAD_ACCI 1             ; 1 in acc
    RET

CALCULATE_FACT:

    MV x1, x0               ; N in x1
    SUBI 1                  ; N-1 in acc
    STORE_ACC x0            ; N-1 in x0
    CALL FACTORIAL          ; (N-1)! in acc
    MUL x1                  ; N*(N-1)! = N! in acc

    RET



FUNC MAIN: 0
    LOAD_ACCI "Enter N: "
    CALL_INTRINSIC PRINTS

    CALL_INTRINSIC SCAN     ; N in acc
    STORE_ACC x0            ; N in acc

    LOAD_ACCI "N! = "
    CALL_INTRINSIC PRINTS
    
    CALL FACTORIAL          ; N! in acc

    CALL_INTRINSIC PRINT    
    RET
