#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(e2eTest, squareEqTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/square_eq.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CoutRedefiner c;
    c.changeOnMy();
    ASSERT_TRUE(vm.run());
    auto ss = c.returnBack();

    double x1, x2;
    ss >> x1 >> x2;
    ASSERT_DOUBLE_EQ(x1, 0.5);
    ASSERT_DOUBLE_EQ(x2, 2.0);
}

TEST(e2eTest, arithmeticTest)
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

TEST(e2eTest, gcTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/gc.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    ASSERT_TRUE(vm.run());
}

TEST(InstructionTest, intrinsicTest)
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

    double x;
    ss_out >> x;
    ASSERT_DOUBLE_EQ(x, 10.0);
}

TEST(InstructionTest, callTest)
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

TEST(InstructionTest, branchTest)
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

TEST(InstructionTest, arrayTest)
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

TEST(InstructionTest, stringTest)
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

TEST(InstructionTest, strfuncTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/strfunc.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CoutRedefiner out;
    out.changeOnMy();

    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    std::string str1;
    std::string str2;
    uint32_t size1;
    uint32_t size2;

    ss_out >> str1 >> size1 >> str2 >> size2;

    ASSERT_EQ(str1,  "WhatsoeverString");
    ASSERT_EQ(size1, 16);
    ASSERT_EQ(str2,  "Ayo!");
    ASSERT_EQ(size2, 4);
}

TEST(InstructionTest, recursionTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/recursion.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CinRedefiner in;
    VM_Tests::CoutRedefiner out;
    in.changeOnMy();
    out.changeOnMy();

    in.get() << 5 << "\n";
    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    uint64_t fact;
    ss_out >> fact;
    ASSERT_EQ(fact, 120);
}

TEST(InstructionTest, objectTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/object.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CoutRedefiner out;
    out.changeOnMy();

    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    uint64_t intField;
    double floatField;
    std::string strField;
    uint64_t emptyStrlen;
    ss_out >> intField >> floatField >> strField >> emptyStrlen;

    ASSERT_EQ(intField, 2287);
    ASSERT_DOUBLE_EQ(floatField, 3.14);
    ASSERT_EQ(strField, "TestString");
    ASSERT_EQ(emptyStrlen, 1448);
}

TEST(InstructionTest, objarrayTest)
{
    Common::Program test;
    deserializeProgram("../asm/tests/objarray.prog", test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    VM_Tests::CoutRedefiner out;
    out.changeOnMy();

    ASSERT_TRUE(vm.run());
    auto ss_out = out.returnBack();

    uint64_t intVal;
    double floatVal;
    ss_out >> intVal >> floatVal;

    ASSERT_EQ(intVal, 42);
    ASSERT_DOUBLE_EQ(floatVal, 2.71);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}