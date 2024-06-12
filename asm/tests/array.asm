FUNC FACTORIALS: 1

    LOAD_ACC x0

    NEWARRAY INTEGER        ; Allocate array for factorials
    MVI x1, 1
    MVI x2, 0
    STORE_ARR_ELEM x1, x2   ; Load 1 into 0-th element

    MVI x2, 1               ; x2 will be index in the array

    STORE_ACC x1            ; Array address in x1
    LOAD_ACC x2             ; Load current index into accumulator
    JMP END_LOOP


LOOP:
    ; Main loop
    LOAD_ACC x2             ; In x2 current index
    SUBI 1
    STORE_ACC x3            ; In x3 previous index

    LOAD_ACC x1             ; Array address in accumulator
    LOAD_ARR_ELEM x3, x3    ; In x3 value of previous index

    LOAD_ACC x3
    MUL x2
    STORE_ACC x3

    LOAD_ACC x1             ; Array address in accumulator
    STORE_ARR_ELEM x3, x2   ; Save factorial to current index

    LOAD_ACC x2
    ADDI 1                  ; Increment x2 (array index)
    STORE_ACC x2

END_LOOP:
    BLT x0, LOOP


    LOAD_ACC x1
    RET




FUNC MAIN: 0

    CALL_INTRINSIC SCAN
    ADDI 1                  ; Make one more
    STORE_ACC x0

    LOAD_ACCI 0
    BGE x0, EXIT            ; If 0 >= x0 then no calculations

    CALL FACTORIALS

    ; Print whole array
    STORE_ACC x1            ; Array address in x1
    MVI x2, 0

LOOP_MAIN:
    LOAD_ACC x1
    LOAD_ARR_ELEM x3, x2
    LOAD_ACC x3

    CALL_INTRINSIC PRINT

    LOAD_ACC x2
    ADDI 1
    STORE_ACC x2

    BLT x0, LOOP_MAIN

EXIT:
    RET
