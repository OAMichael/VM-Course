/* Test program

    FMVI x1, -3.14f
    FMVI x2, 9.81f

    IMVI x3, 15
    IMVI x4, -5

    ; Not equal
    BNE x1, x2, 24

    IMVI x5, 10
    JMP 16

    IMVI x5, 20

    ; Float less than
    FBLT x1, x2, 24

    IMVI x6, 30
    JMP 16

    IMVI x6, 40

    ; Integer greater or equal
    IBGE x4, x3, 24

    IMVI x7, 50
    JMP 16

    IMVI x7, 60


    IMVI x8, 4000
    STOREW x8, x5, 0
    INTRINSIC x8, IPRINT

    IMVI x8, 4008
    STOREW x8, x6, 0
    INTRINSIC x8, IPRINT

    IMVI x8, 4016
    STOREW x8, x7, 0
    INTRINSIC x8, IPRINT

    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/branching.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CoutRedefiner c;
    c.changeOnMy();
    ASSERT_TRUE(vm.run());
    auto ss = c.returnBack();

    int x5, x6, x7;
    ss >> x5 >> x6 >> x7;
    ASSERT_EQ(x5, 20);
    ASSERT_EQ(x6, 40);
    ASSERT_EQ(x7, 50);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}