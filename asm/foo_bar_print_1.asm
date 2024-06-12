CLASS Bar:
    INTEGER a
CLASS_END


CLASS Foo:
    INTEGER x
    Bar y
CLASS_END



FUNC dump: 1
    LOAD_ACC x0             ; Array pointer in accumulator
    GET_LENGTH              ; Array length in accumulator
    
    STORE_ACC x2            ; x2 will be length of the array
    MVI x1, 0               ; x1 will be an index (i)

LOOP_START:
    LOAD_ACC x1             ; Loop header
    BGE x2, LOOP_END        ;

    NEW Foo                 ; Allocate instance of Foo

    LOAD_ACC x0
    LOAD_ARR_ELEM x3, x1    ; Get i-th element into x3

    LOAD_ACC x3
    MVI x4, 0
    BNE x4, Foo_nonull      ; f == null

Foo_null:
    LOAD_ACCI "Foo:null\n"
    CALL_INTRINSIC PRINTS
    JMP LOOP_INC

Foo_nonull:
    LOAD_FIELD x4, Foo.y    ; b = f.y
    LOAD_ACC x4
    MVI x4, 0
    BNE x4, Foo_Bar_nonull  ; b == null

Foo_Bar_null:
    LOAD_ACCI "Foo.Bar:null\n"
    CALL_INTRINSIC PRINTS
    JMP LOOP_INC

Foo_Bar_nonull:
    LOAD_FIELD x4, Bar.a    ; b.a
    LOAD_ACC x4
    CALL_INTRINSIC PRINT

LOOP_INC:
    LOAD_ACC x1
    ADDI 1
    STORE_ACC x1
    JMP LOOP_START

LOOP_END:
    RET



FUNC MAIN: 0
    LOAD_ACCI 1000
    STORE_ACC x7                ; M
    NEWARRAY Foo
    STORE_ACC x1                ; foo

    NEW Foo
    STORE_ACC x2                ; outer

    LOAD_ACCI 1
    STORE_ACC x3                ; i

    LOAD_ACCI 4000
    STORE_ACC x9                ; N

LOOP:                           ; for (let i : number = 1; i <= N; i++)
    NEW Foo                     ; let o1 = new Foo()
    STORE_FIELD x3, Foo.x       ; o1.x = i
    STORE_ACC x4                ; x4 = o1

    LOAD_ACC x3
    MVI x8, 3
    MOD x8
    STORE_ACC x5                ; i % 3
    LOAD_ACCI 0
    BNE x5, LABEL_0             ; if (i % 3 == 0)

    LOAD_ACC x3                 ; i
    MOD x7
    STORE_ACC x5                ; i % M
    LOAD_ACC x1                 ; addr foo
    STORE_ARR_ELEM x4, x5       ; foo[i % M] = o1
LABEL_0:
    NEW Bar                     ; let o2 = new Bar()
    STORE_FIELD x3, Bar.a       ; o2.a = i
    STORE_ACC x6                ; x6 = o2

    LOAD_ACC x3
    MVI x8, 5
    MOD x8
    STORE_ACC x5                ; i % 5
    LOAD_ACCI 0
    BNE x5, LABEL_1             ; if (i % 5 == 0)

    LOAD_ACC x4                 ; load o1
    STORE_FIELD x6, Foo.y       ; o1.y = o2
LABEL_1:
    MV x2, x4                   ; outer = o1

    MV x0, x1
    CALL dump                   ; dump(foo)

    LOAD_ACC x3
    ADDI 1
    STORE_ACC x3

    BLE x9, LOOP
    RET
