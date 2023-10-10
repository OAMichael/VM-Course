#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"

static bool makeTestFirst(Common::Program& outProg) {
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

    outProg.entryPoint = 0;

    VM::Decoder coder;
    VM::EncodedInstruction encInstr;
    VM::DecodedInstruction decInstr;

    // fmvi x1, -3.14f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X1;
        decInstr.fpimm = -3.14f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmvi x2, 9.81f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X2;
        decInstr.fpimm = 9.81f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x3, 15
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X3;
        decInstr.imm = 15;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x4, -5
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.imm = -5;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // bne x1, x2, 24
    {
        decInstr.opcode = VM::InstructionType::BNE;
        decInstr.rs1 = VM::RegisterType::X1;
        decInstr.rs1 = VM::RegisterType::X2;
        decInstr.imm = 24;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x5, 10
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X5;
        decInstr.imm = 10;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // jmp 16
    {
        decInstr.opcode = VM::InstructionType::JMP;
        decInstr.imm = 16;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x5, 20
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X5;
        decInstr.imm = 20;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fblt x1, x2, 24
    {
        decInstr.opcode = VM::InstructionType::FBLT;
        decInstr.rs1 = VM::RegisterType::X1;
        decInstr.rs2 = VM::RegisterType::X2;
        decInstr.imm = 24;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x6, 30
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X6;
        decInstr.imm = 30;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // jmp 16
    {
        decInstr.opcode = VM::InstructionType::JMP;
        decInstr.imm = 16;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x6, 40
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X6;
        decInstr.imm = 40;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // ibge x4, x3, 24
    {
        decInstr.opcode = VM::InstructionType::IBGE;
        decInstr.rs1 = VM::RegisterType::X4;
        decInstr.rs2 = VM::RegisterType::X3;
        decInstr.imm = 24;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x7, 50
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = 50;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // jmp 16
    {
        decInstr.opcode = VM::InstructionType::JMP;
        decInstr.imm = 16;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x7, 60
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = 60;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x8, 4000
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X8;
        decInstr.imm = 4000;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // stored x8, x5, 0
    {
        decInstr.opcode = VM::InstructionType::STORED;
        decInstr.rs1 = VM::RegisterType::X8;
        decInstr.rs2 = VM::RegisterType::X5;
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // intrinsic x8, iprint
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_IPRINT;
        decInstr.rs1 = VM::RegisterType::X8;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x8, 4008
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X8;
        decInstr.imm = 4008;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // stored x8, x6, 0
    {
        decInstr.opcode = VM::InstructionType::STORED;
        decInstr.rs1 = VM::RegisterType::X8;
        decInstr.rs2 = VM::RegisterType::X6;
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // intrinsic x8, iprint
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_IPRINT;
        decInstr.rs1 = VM::RegisterType::X8;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x8, 4016
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X8;
        decInstr.imm = 4016;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // stored x8, x5, 0
    {
        decInstr.opcode = VM::InstructionType::STORED;
        decInstr.rs1 = VM::RegisterType::X8;
        decInstr.rs2 = VM::RegisterType::X7;
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // intrinsic x8, iprint
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_IPRINT;
        decInstr.rs1 = VM::RegisterType::X8;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // ret
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