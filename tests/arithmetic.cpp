/* Test program

    FMVI x1, 314.0f
    FMVI x2, 228.0f

    FSIN x4, x1             ; sin(x1)
    FCOSI x5, 1488.0f       ; cos(1488)

    FMUL x4, x2, x4         ; 228 * sin(x1)
    FADD x4, x5, x4         ; cos(1488) + 228 * sin(x1)

    IMVI x7, 4000
    STOREW x7, x4, 0
    INTRINSIC x7, FPRINT    ; print result
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

    float x;
    ss >> x;
    ASSERT_EQ(x, -35.7189f);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}