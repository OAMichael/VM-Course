CLASS Point3D:
    FLOATING x
    FLOATING y
    FLOATING z
CLASS_END


CLASS Triangle3D:
    Point3D A
    Point3D B
    Point3D C
CLASS_END


FUNC MAIN: 0
    LOAD_ACCI 100
    STORE_ACC x1

    MVI x2, 0

LOOP:
    LOAD_ACCI 1000000
    NEWARRAY INTEGER

    LOAD_ACC x1
    NEW Triangle3D
    NEW Triangle3D
    NEW Triangle3D
    NEW Triangle3D
    NEW Triangle3D
    NEW Triangle3D
    NEW Triangle3D

    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING
    LOAD_ACCI 100
    NEW STRING

    LOAD_ACCI 1000000
    NEWARRAY FLOATING
    LOAD_ACCI 1000000
    NEWARRAY FLOATING
    LOAD_ACCI 1000000
    NEWARRAY FLOATING
    LOAD_ACCI 1000000
    NEWARRAY FLOATING
    LOAD_ACCI 1000000
    NEWARRAY FLOATING
    LOAD_ACCI 1000000
    NEWARRAY FLOATING
    LOAD_ACCI 1000000
    NEWARRAY FLOATING

    LOAD_ACC x2
    ADDI 1
    STORE_ACC x2

LOOP_END:
    BLT x1, LOOP

    RET
