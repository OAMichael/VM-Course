#include <gtest/gtest.h>
#include "VirtualMachine.h"

static bool makeTestFirst(Common::Program& outProg) {
    /* Test program

        int x;
        mov x, 7;
        x = -x;
        print(&x)

    */

    outProg.entryPoint = 0;

    VM::Decoder coder;
    VM::EncodedInstruction encInstr;
    VM::DecodedInstruction decInstr;

    // imvi x1, 7
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = static_cast<VM::RegisterType>(1);
        decInstr.imm = 7;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // loadd x2, x1, 0
    {
        decInstr.opcode = VM::InstructionType::LOADD;
        decInstr.rd = static_cast<VM::RegisterType>(2);
        decInstr.rs1 = static_cast<VM::RegisterType>(1);
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // ineg x3, x2
    {
        decInstr.opcode = VM::InstructionType::INEG;
        decInstr.rd = static_cast<VM::RegisterType>(3);
        decInstr.rs1 = static_cast<VM::RegisterType>(2);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // stored x1, x3, 0
    {
        decInstr.opcode = VM::InstructionType::STORED;
        decInstr.rs1 = static_cast<VM::RegisterType>(1);
        decInstr.rs2 = static_cast<VM::RegisterType>(3);
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // intrinsic x1, iprint
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_IPRINT;
        decInstr.rs1 = static_cast<VM::RegisterType>(1);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // return
    {
        decInstr.opcode = VM::InstructionType::RET;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    return true;
}


TEST(InstructionTest, firstTest)
{
    Common::Program test;
    makeTestFirst(test);

    VM::VirtualMachine vm;
    vm.loadProgram(test);

    ASSERT_TRUE(vm.run());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}