#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"

static bool makeTestFirst(Common::Program& outProg) {
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

    outProg.entryPoint = 0;

    VM::Decoder coder;
    VM::EncodedInstruction encInstr;
    VM::DecodedInstruction decInstr;

    // fmvi x1, 314.0f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X1;
        decInstr.fpimm = 314.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // fmvi x2, 228.0f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X2;
        decInstr.fpimm = 228.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // fsin x4, x1
    {
        decInstr.opcode = VM::InstructionType::FSIN;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X1;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // fcosi x5, 1488.0f
    {
        decInstr.opcode = VM::InstructionType::FCOSI;
        decInstr.rd = VM::RegisterType::X5;
        decInstr.fpimm = 1488.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // fmul x4, x2, x4
    {
        decInstr.opcode = VM::InstructionType::FMUL;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X2;
        decInstr.rs2 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // fadd x4, x5, x4
    {
        decInstr.opcode = VM::InstructionType::FADD;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X5;
        decInstr.rs2 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // imvi x7, 4000
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = 4000;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // stored x7, x4, 0
    {
        decInstr.opcode = VM::InstructionType::STORED;
        decInstr.rs1 = VM::RegisterType::X7;
        decInstr.rs2 = VM::RegisterType::X4;
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // intrinsic x7, fprint
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_FPRINT;
        decInstr.rs1 = VM::RegisterType::X7;

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

    float x;
    ss >> x;
    ASSERT_EQ(x, -35.7189f);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}