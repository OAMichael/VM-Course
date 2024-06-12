CLASS TestClassInner:
    INTEGER intVal
    FLOATING floatVal
CLASS_END


CLASS TestClassOuter:
    INTEGER intVal
    FLOATING floatVal
    TestClassInner innerVal
CLASS_END


FUNC MAIN: 0
    LOAD_ACCI 10
    NEWARRAY TestClassInner                     ; TestClassInner array ptr in accumulator
    STORE_ACC x1                                ; TestClassInner array ptr in x1

    NEW TestClassInner                          ; TestClassInner ptr in accumulator
    MVI x2, 42
    MVI x3, 2.71
    STORE_FIELD x2, TestClassInner.intVal
    STORE_FIELD x3, TestClassInner.floatVal
    STORE_ACC x2

    LOAD_ACC x1                                 ; TestClassInner array ptr in accumulator
    MVI x3, 7
    STORE_ARR_ELEM x2, x3

    NEW TestClassOuter
    STORE_ACC x4

    NEW TestClassInner
    STORE_ACC x2

    LOAD_ACC x1
    MVI x3, 7
    LOAD_ARR_ELEM x2, x3

    LOAD_ACC x4
    STORE_FIELD x2, TestClassOuter.innerVal

    LOAD_FIELD x5, TestClassOuter.innerVal
    LOAD_ACC x5
    LOAD_FIELD x1 TestClassInner.intVal
    LOAD_FIELD x2 TestClassInner.floatVal

    LOAD_ACC x1
    CALL_INTRINSIC PRINT

    LOAD_ACC x2
    CALL_INTRINSIC PRINTF

    RET
