#include <iostream>

#include "VirtualMachine.h"

static bool makeTestProgram_Quadratic(Common::Program& outProg) {
    outProg.entryPoint = 0;

    // Used Decoder::encodeInstruction(...) to generate this stuff
    VM::EncodedInstruction squareEquation[35] = {
        0x00000fa00000043aULL,      // IMVI x4, 4000
        0x000000000004035aULL,      // INTRINSIC x4, FSCAN
        0x0000000000002449ULL,      // LOADW x1, x4, 0         ; a

        0x00000fa40000043aULL,      // IMVI x4, 4004
        0x000000000004035aULL,      // INTRINSIC x4, FSCAN
        0x0000000000004449ULL,      // LOADW x2, x4, 0         ; b

        0x00000fa80000043aULL,      // IMVI x4, 4008
        0x000000000004035aULL,      // INTRINSIC x4, FSCAN
        0x0000000000006449ULL,      // LOADW x3, x4, 0         ; c

        0x0000000000102307ULL,      // FMUL x4, x1, x3         ; a*c
        0x4080000000008426ULL,      // FMULI x4, x4, 4.0f      ; 4*a*c
        0x0000000000144207ULL,      // FMUL x5, x2, x2         ; b*b
        0x000000000010a406ULL,      // FSUB x4, x5, x4         ; D = b*b - 4*a*c

        0x000000000000063cULL,      // FMVI x6, 0.0f

        0x000000a00000c443ULL,      // FBLT x4, x6, 160        ; D < 0

        0x000000700000c43fULL,      // BEQ x4, x6, 112         ; D == 0

                                    // ; D > 0
        0x0000000000008412ULL,      // FSQRT x4, x4            ; sqrt(D)
        0xc00000000000c126ULL,      // FMULI x6, x1, -2.0f     ; -2 * a
        0x0000000000108608ULL,      // FDIV x4, x4, x6         ; sqrt(D) / (-2 * a)

        0x0000000000144608ULL,      // FDIV x5, x2, x6         ; b / (-2 * a)

        0x000000000018a406ULL,      // FSUB x6, x5, x4         ; x_1

        0x00000fac0000073aULL,      // IMVI x7, 4012
        0x000000000000c74dULL,      // STOREW x7, x6, 0
        0x000000000007045aULL,      // INTRINSIC x7, FPRINT    ; print x_1

        0x000000000018a405ULL,      // FADD x6, x5, x4         ; x_2

        0x00000fb00000073aULL,      // IMVI x7, 4016
        0x000000000000c74dULL,      // STOREW x7, x6, 0
        0x000000000007045aULL,      // INTRINSIC x7, FPRINT    ; print x_2

        0x0000003000000059ULL,      // JMP 48

                                    // ; D == 0
        0xc00000000000c126ULL,      // FMULI x6, x1, -2.0f     ; -2 * a
        0x0000000000184608ULL,      // FDIV x6, x2, x6         ; b / (-2 * a)

        0x00000fac0000073aULL,      // IMVI x7, 4012
        0x000000000000c74dULL,      // STOREW x7, x6, 0
        0x000000000007045aULL,      // INTRINSIC x7, FPRINT    ; print x_1

        0x0000000000000055ULL,      // RET
    };

    for (int i = 0; i < 35; ++i) {
        outProg.instructions.push_back(squareEquation[i]);
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
