FUNC FACTORIALS: 1

    LOAD_ACC x0

    NEWARRAY INTEGER        ; Allocate array for factorials
    STORE_ACC x1            ; Address of allocated array in x1

    LOAD_ACCI 1             ; 0! = 1
    STORE_ACC_MEM x1, 0

    LOAD_ACCI 1
    STORE_ACC x2            ; x2 will be index in the array


    JMP END_LOOP

LOOP:
    ; Main loop
    LOAD_ACC x2
    MULI 8
    ADD x1                  ; x1 + sizeof(uint64_t) * x2

    STORE_ACC x3            ; Save current array entry address in x3

    SUBI 8
    STORE_ACC x4
    LOAD_ACC_MEM x4, 0      ; Load previous factorial

    MUL x2
    STORE_ACC_MEM x3, 0     ; Store new factorial

    LOAD_ACC x2
    ADDI 1
    STORE_ACC x2            ; Increment index

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
    LOAD_ACCI 0
    STORE_ACC x2            ; Array index in x2

LOOP_MAIN:
    LOAD_ACC x2
    MULI 8
    ADD x1
    STORE_ACC x3
    LOAD_ACC_MEM x3, 0

    CALL_INTRINSIC PRINT

    LOAD_ACC x2
    ADDI 1
    STORE_ACC x2
    BLT x0, LOOP_MAIN

EXIT:
    RET
