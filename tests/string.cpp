/* Test program

FUNC MAIN: 0
    LOAD_ACCI "Hello, world!\n"
    CALL_INTRINSIC PRINTS
    
    LOAD_ACCI 100
    CALL_INTRINSIC SCANS
    STORE_ACC x0

    LOAD_ACCI 100
    CALL_INTRINSIC SCANS
    STORE_ACC x1

    LOAD_ACC x0
    CALL_INTRINSIC PRINTS

    LOAD_ACCI "\n"
    CALL_INTRINSIC PRINTS

    LOAD_ACC x1
    CALL_INTRINSIC PRINTS

    LOAD_ACCI "\n"
    CALL_INTRINSIC PRINTS

    LOAD_ACCI "T\tr\ty\tt\ta\tb\n"
    CALL_INTRINSIC PRINTS

    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/string.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CinRedefiner in;
    VM_Tests::CoutRedefiner out;
    in.changeOnMy();
    out.changeOnMy();
    
    in.get() << "Hey!\n" << "Whatsup?\n";
    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    std::string str0;
    std::string str1;
    std::string str2;
    std::string str3;
    ss_out >> str0 >> str1 >> str2 >> str3;
    ASSERT_EQ(str0, "ImAlright!");
    ASSERT_EQ(str1, "Hey!");
    ASSERT_EQ(str2, "Whatsup?");
    ASSERT_EQ(str3, "ThatsFine");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}