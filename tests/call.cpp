/* Test program

FUNC ADD: 2

    LOAD_ACC x0
    ADDF x1
    RET


FUNC MAIN: 0

    CALL_INTRINSIC SCANF
    STORE_ACC x0

    CALL_INTRINSIC SCANF
    STORE_ACC x1

    CALL ADD
    CALL_INTRINSIC PRINTF

    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/call.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CinRedefiner in;
    VM_Tests::CoutRedefiner out;
    in.changeOnMy();
    out.changeOnMy();
    
    in.get() << 3.14 << "\n" << 9.81;
    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    double x;
    ss_out >> x;
    ASSERT_DOUBLE_EQ(x, 12.95);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}