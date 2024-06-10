CLASS Bar:
    INTEGER a
CLASS_END

CLASS Foo:
    INTEGER x
    Bar y
CLASS_END


FUNC DUMP: 1
    RET

FUNC MAIN: 0
    LOAD_ACCI 1000
    NEWARRAY Foo
    STORE_ACC x1                ; foo

    NEW Foo
    STORE_ACC x2                ; outer

    LOAD_ACCI 1
    STORE_ACC x3                ; i

    LOAD_ACCI 1000
    STORE_ACC x7                ; M

    LOAD_ACCI 4000000
    STORE_ACC x9                ; N

LOOP:                           ; for (let i : number = 1; i <= N; i++)
    NEW Foo                     ; let o1 = new Foo()
    LOAD_FIELD x3, Foo.x        ; o1.x = i
    STORE_ACC x4                ; x4 = o1

    LOAD_ACC x3
    MVI x8, 3
    MOD x8
    STORE_ACC x5                ; i % 3
    BNE x5, LABEL_0             ; if (i % 3 == 0)

    LOAD_ACC x3                 ; i
    MOD x7
    STORE_ACC x5                ; i % M
    LOAD_ACC x1                 ; addr foo
    LOAD_ARR_ELEM x4, x5        ; foo[i % M] = o1
LABEL_0:
    NEW Bar                     ; let o2 = new Bar()
    LOAD_FIELD x3, Bar.a        ; o2.a = i
    STORE_ACC x6                ; x6 = o2

    LOAD_ACC x3
    MVI x8, 5
    MOD x8
    STORE_ACC x5                ; i % 5
    BNE x5, LABEL_1             ; if (i % 5 == 0)

    LOAD_ACC x4                 ; load o1
    LOAD_FIELD x6, Foo.y        ; o1.y = o2
LABEL_1:
    MV x2, x4                   ; outer = o1

    STORE_ACC x0
    CALL DUMP                   ; dump(foo)

    LOAD_ACC x3
    ADDI 1
    STORE_ACC x3

    BLE x9, LOOP
    RET