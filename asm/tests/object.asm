CLASS Point3D:
    INTEGER x
    INTEGER y
    INTEGER z
CLASS_END

CLASS SuperTest:
    Point3D testFieldInnerObj
    INTEGER testFieldInt
    FLOATING testFieldFloat
    STRING testFieldString
CLASS_END


FUNC MAIN: 0
    NEW SuperTest
    STORE_ACC x1

    MVI x3, 2287
    STORE_FIELD x3, SuperTest.testFieldInt
    LOAD_FIELD x2, SuperTest.testFieldInt
    LOAD_ACC x2
    CALL_INTRINSIC PRINT

    LOAD_ACC x1
    MVI x3, 3.14f
    STORE_FIELD x3, SuperTest.testFieldFloat
    LOAD_FIELD x2, SuperTest.testFieldFloat
    LOAD_ACC x2
    CALL_INTRINSIC PRINTF

    LOAD_ACCI "TestString\n"
    STORE_ACC x3
    LOAD_ACC x1
    STORE_FIELD x3, SuperTest.testFieldString
    LOAD_FIELD x2, SuperTest.testFieldString
    LOAD_ACC x2
    CALL_INTRINSIC PRINTS
    
    LOAD_ACCI 1448
    NEW STRING
    CALL_INTRINSIC STRLEN
    CALL_INTRINSIC PRINT

    RET
