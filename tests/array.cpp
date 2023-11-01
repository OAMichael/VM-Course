/* Test program

FUNC FACTORIALS: 1

    LOAD_ACC x0

    NEWARRAY INTEGER        ; Allocate array for factorials
    STORE_ACC x1            ; Address of allocated array in x1

    LOAD_ACCI 1             ; 0! = 1
    STORE_ACC_MEM x1, 0

    LOAD_ACCI 1
    STORE_ACC x2            ; x2 will be index in the array


    JMP 52

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

    BLT x0, -48


    LOAD_ACC x1
    RET



FUNC MAIN: 0

    CALL_INTRINSIC SCAN
    ADDI 1                  ; Make one more
    STORE_ACC x0

    LOAD_ACCI 0
    BGE x0, 60              ; If 0 >= x0 then no calculations

    CALL FACTORIALS

    ; Print whole array
    STORE_ACC x1            ; Array address in x1
    LOAD_ACCI 0
    STORE_ACC x2            ; Array index in x2


    LOAD_ACC x2
    MULI 8
    ADD x1
    STORE_ACC x3
    LOAD_ACC_MEM x3, 0

    CALL_INTRINSIC PRINT

    LOAD_ACC x2
    ADDI 1
    STORE_ACC x2
    BLT x0, -36


    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/array.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CinRedefiner in;
    VM_Tests::CoutRedefiner out;
    in.changeOnMy();
    out.changeOnMy();
    
    in.get() << 5 << "\n";
    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    uint64_t fact0;
    uint64_t fact1;
    uint64_t fact2;
    uint64_t fact3;
    uint64_t fact4;
    uint64_t fact5;
    ss_out >> fact0 >> fact1 >> fact2 >> fact3 >> fact4 >> fact5;
    ASSERT_EQ(fact0, 1);
    ASSERT_EQ(fact1, 1);
    ASSERT_EQ(fact2, 2);
    ASSERT_EQ(fact3, 6);
    ASSERT_EQ(fact4, 24);
    ASSERT_EQ(fact5, 120);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}