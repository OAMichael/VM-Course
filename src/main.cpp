#include <iostream>

#include "VirtualMachine.h"


static bool makeTestProgram_Quadratic(Common::Program& outProg) {
    /* Test program

        int x;
        scan(&x);
        x = -x;
        print(&x)

    */

    outProg.entryPoint = 0;

    VM::Decoder coder;
    VM::EncodedInstruction encInstr;
    VM::DecodedInstruction decInstr;

    // mvi x1, 4000
    {
        decInstr.opcode = VM::InstructionType::MVI;
        decInstr.rd = static_cast<VM::RegisterType>(1);
        decInstr.imm = 4000;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // intrinsic x1, iscan
    {
        decInstr.opcode = VM::InstructionType::INTRINSIC;
        decInstr.intrinType = VM::IntrinsicType::INTRINSIC_ISCAN;
        decInstr.rs1 = static_cast<VM::RegisterType>(1);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // ld x2, x1, 0
    {
        decInstr.opcode = VM::InstructionType::LD;
        decInstr.rd = static_cast<VM::RegisterType>(2);
        decInstr.rs1 = static_cast<VM::RegisterType>(1);
        decInstr.imm = 0;

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // neg x3, x2
    {
        decInstr.opcode = VM::InstructionType::NEG;
        decInstr.rd = static_cast<VM::RegisterType>(3);
        decInstr.rs1 = static_cast<VM::RegisterType>(2);

        coder.encodeInstruction(decInstr, encInstr);
        outProg.instructions.push_back(encInstr);
    }

    // sd x1, x3, 0
    {
        decInstr.opcode = VM::InstructionType::SD;
        decInstr.rs1 = static_cast<VM::RegisterType>(1);
        decInstr.rs2 = static_cast<VM::RegisterType>(3);

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
