/* Test program

    CALL_INTRINSIC SCANF
    ADDI 5.0
    CALL_INTRINSIC PRINTF
    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/intrinsic.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CinRedefiner in;
    VM_Tests::CoutRedefiner out;
    in.changeOnMy();
    out.changeOnMy();
    
    in.get() << 5.0;
    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    float x;
    ss_out >> x;
    ASSERT_EQ(x, 10.0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}