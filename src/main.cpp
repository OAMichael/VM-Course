#include <iostream>

#include "VirtualMachine.h"

static bool makeTestProgram_Quadratic(Common::Program& outProg) {
    outProg.entryPoint = 0;

    VM::Decoder coder;
    VM::EncodedInstruction encInstr;
    VM::DecodedInstruction decInstr;

    const uint64_t storeAddress = 4000;

    // imvi x4, 4000
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.imm = storeAddress;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // intrinsic x4, fscan
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_FSCAN;
        decInstr.rs1 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // loadd x1, x4, 0
    {
        decInstr.opcode = VM::InstructionType::LOADW;
        decInstr.rd = VM::RegisterType::X1;
        decInstr.rs1 = VM::RegisterType::X4;
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x4, 4000 + sizeof(float)
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.imm = storeAddress + sizeof(float);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // intrinsic x4, fscan
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_FSCAN;
        decInstr.rs1 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // loadd x2, x4, 0
    {
        decInstr.opcode = VM::InstructionType::LOADW;
        decInstr.rd = VM::RegisterType::X2;
        decInstr.rs1 = VM::RegisterType::X4;
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // imvi x4, 4000 + 2 * sizeof(float)
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X4;
        decInstr.imm = storeAddress + 2 * sizeof(float);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // intrinsic x4, fscan
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_FSCAN;
        decInstr.rs1 = VM::RegisterType::X4;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }
    // loadd x3, x4, 0
    {
        decInstr.opcode = VM::InstructionType::LOADW;
        decInstr.rd = VM::RegisterType::X3;
        decInstr.rs1 = VM::RegisterType::X4;
        decInstr.imm = 0;

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
    // imvi x7, 4000 + 3 * sizeof(float)
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = storeAddress + 3 * sizeof(float);

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
    // imvi x7, 4000 + 4 * sizeof(float)
    {
        decInstr.opcode = VM::InstructionType::IMVI;
        decInstr.rd = VM::RegisterType::X7;
        decInstr.imm = storeAddress + 4 * sizeof(float);

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



int main(int argc, char* argv[]) {

    Common::Program testQuadratic;
    makeTestProgram_Quadratic(testQuadratic);

    VM::VirtualMachine vm;
    vm.loadProgram(testQuadratic);

    bool runResult = vm.run();

    if (runResult) {
        std::cout << "Program has been successfully interpreted" << std::endl;
    }
    else {
        std::cerr << "Program execution has been finished with errors" << std::endl;
    }

    return 0;
}
