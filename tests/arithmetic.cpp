/* Test program

    MVI x1, 314.0
    MVI x2, 228.0

    LOAD_ACC x1
    SIN                     ; sin(314) in acc
    MULF x2                 ; 228 * sin(314) in acc
    STORE_ACC x1            ; 228 * sin(314) in x1

    LOAD_ACCI 1488.0        ; 1488 in acc
    COS                     ; cos(1488) in acc
    ADDF x1                 ; cos(1488) + 228 * sin(x1) in acc

    CALL_INTRINSIC PRINTF   ; print result

    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/arithmetic.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CoutRedefiner c;
    c.changeOnMy();
    ASSERT_TRUE(vm.run());
    auto ss = c.returnBack();

    double x;
    ss >> x;
    ASSERT_DOUBLE_EQ(x, -35.7189);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}