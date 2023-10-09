#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"

static bool makeTestFirst(Common::Program& outProg) {
    /* Test program

        FMVI x1, 2.0f           ; a = 2.0f
        FMVI x2, -5.0f          ; b = -5.0f
        FMVI x3, 2.0f           ; c = 4.0f

        FMUL x4, x1, x3         ; a*c
        FMULI x4, x4, 4         ; 4*a*c
        FMUL x5, x2, x2         ; b*b
        FSUB x4, x5, x4         ; D = b*b - 4*a*c
        FSQRT x4, x4            ; sqrt(D)
        FMULI x6, x1, -2        ; -2 * a
        FDIV x4, x4, x6         ; sqrt(D) / (-2 * a)

        FDIV x5, x2, x6         ; b / (-2 * a)

        FSUB x6, x5, x4         ; x_1

        IMVI x7, 4000
        STOREW x7, x6, 0
        INTRINSIC x7, FPRINT    ; print x_1

        FADD x6, x5, x4         ; x_2

        IMVI x7, 4004
        STOREW x7, x6, 0
        INTRINSIC x7, FPRINT    ; print x_2

        RET

    */

    outProg.entryPoint = 0;

    VM::Decoder coder;
    VM::EncodedInstruction encInstr;
    VM::DecodedInstruction decInstr;

    const uint64_t storeAddress = 4000;

    // fmvi x1, 2.0f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X1;
        decInstr.fpimm = 2.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmvi x2, -5.0f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X2;
        decInstr.fpimm = -5.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmvi x3, 2.0f
    {
        decInstr.opcode = VM::InstructionType::FMVI;
        decInstr.rd = VM::RegisterType::X3;
        decInstr.fpimm = 2.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmul x4, x1, x3
    {
        decInstr.opcode = VM::InstructionType::FMUL;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X1;
        decInstr.rs2 = VM::RegisterType::X3;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmuli x4, x4, 4.0f
    {
        decInstr.opcode = VM::InstructionType::FMULI;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X4;
        decInstr.fpimm = 4.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmul x5, x2, x2
    {
        decInstr.opcode = VM::InstructionType::FMUL;
        decInstr.rd = VM::RegisterType::X5;
        decInstr.rs1 = VM::RegisterType::X2;
        decInstr.rs2 = VM::RegisterType::X2;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fsub x4, x5, x4
    {
        decInstr.opcode = VM::InstructionType::FSUB;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X5;
        decInstr.rs2 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fsqrt x4, x4
    {
        decInstr.opcode = VM::InstructionType::FSQRT;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fmuli x6, x1, -2.0f
    {
        decInstr.opcode = VM::InstructionType::FMULI;
        decInstr.rd = VM::RegisterType::X6;
        decInstr.rs1 = VM::RegisterType::X1;
        decInstr.fpimm = -2.0f;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fdiv x4, x4, x6
    {
        decInstr.opcode = VM::InstructionType::FDIV;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.rs1 = VM::RegisterType::X4;
        decInstr.rs2 = VM::RegisterType::X6;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fdiv x5, x2, x6
    {
        decInstr.opcode = VM::InstructionType::FDIV;
        decInstr.rd = VM::RegisterType::X5;
        decInstr.rs1 = VM::RegisterType::X2;
        decInstr.rs2 = VM::RegisterType::X6;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // fsub x6, x5, x4
    {
        decInstr.opcode = VM::InstructionType::FSUB;
        decInstr.rd = VM::RegisterType::X6;
        decInstr.rs1 = VM::RegisterType::X5;
        decInstr.rs2 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x7, 4000
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = storeAddress;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // stored x7, x6, 0
    {
        decInstr.opcode = VM::InstructionType::STOREW;
        decInstr.rs1 = VM::RegisterType::X7;
        decInstr.rs2 = VM::RegisterType::X6;
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
    // fadd x6, x5, x4
    {
        decInstr.opcode = VM::InstructionType::FADD;
        decInstr.rd = VM::RegisterType::X6;
        decInstr.rs1 = VM::RegisterType::X5;
        decInstr.rs2 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x7, 4000 + sizeof(float)
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = storeAddress + sizeof(float);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // stored x7, x6, 0
    {
        decInstr.opcode = VM::InstructionType::STOREW;
        decInstr.rs1 = VM::RegisterType::X7;
        decInstr.rs2 = VM::RegisterType::X6;
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

    float x1, x2;
    ss >> x1 >> x2;
    ASSERT_EQ(x1, 2.f);
    ASSERT_EQ(x2, 0.5f);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}