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
    STORE_ARR_ELEM x3, x1   ; Get i-th element into x3

    LOAD_ACC x3
    MVI x4, 0
    BNE x4, Foo_nonull      ; f == null

Foo_null:
    LOAD_ACCI "Foo:null\n"
    CALL_INTRINSIC PRINTS
    JMP LOOP_INC

Foo_nonull:
    STORE_FIELD x4, Foo.y   ; b = f.y
    LOAD_ACC x4
    MVI x4, 0
    BNE x4, Foo_Bar_nonull  ; b == null

Foo_Bar_null:
    LOAD_ACCI "Foo.Bar:null\n"
    CALL_INTRINSIC PRINTS
    JMP LOOP_INC

Foo_Bar_nonull:
    STORE_FIELD x4, Bar.a   ; b.a
    LOAD_ACC x4
    CALL_INTRINSIC PRINT

LOOP_INC:
    LOAD_ACC x1
    ADDI 1
    STORE_ACC x1
    JMP LOOP_START

LOOP_END:
    RET



FUNC foo: 2
    MV x9, x0                   ; N
    MV x7, x1                   ; M

    LOAD_ACC x1
    NEWARRAY Foo
    STORE_ACC x0                ; foo

    NEW Foo
    STORE_ACC x2                ; outer

    MVI x3, 1                   ; i

MAIN_LOOP:                      ; for (let i : number = 1; i <= N; i++)
    LOAD_ACC x3
    BGT x9, MAIN_LOOP_END

    NEW Foo                     ; let o1 = new Foo()
    LOAD_FIELD x3, Foo.x        ; o1.x = i
    STORE_ACC x4                ; x4 = o1

    LOAD_ACC x3
    MVI x8, 3
    MOD x8                      ; i % 3

    MVI x5, 0
    BNE x5, NO_MOD_3            ; i % 3 == 0

MOD_3:
    LOAD_ACC x3
    MOD x7                      ; i % M
    STORE_ACC x5
    LOAD_ACC x0
    LOAD_ARR_ELEM x4, x5        ; foo[i % M] = o1

NO_MOD_3:
    NEW Bar                     ; let o2 = new Bar()
    LOAD_FIELD x3, Bar.a        ; o2.a = i
    STORE_ACC x6                ; x6 = o2

    LOAD_ACC x3
    MVI x8, 5
    MOD x8

    MVI x5, 0
    BNE x5, NO_MOD_5

MOD_5:
    LOAD_ACC x4
    LOAD_FIELD x6, Foo.y

NO_MOD_5:

    MV x2, x4
    CALL dump       ; dump(foo) (x0 already has array pointer)

    LOAD_ACC x3
    ADDI 1
    STORE_ACC x3
    JMP MAIN_LOOP

MAIN_LOOP_END:
    RET



FUNC MAIN: 0
    MVI x0, 4000    ; let N : number = 4000
    MVI x1, 1000    ; let M : number = 1000
    CALL foo        ; foo(N, M)
    RET