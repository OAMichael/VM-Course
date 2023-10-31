/* Test program

    MVI x1, 2.0
    MVI x2, -5.0
    MVI x3, 2.0

    LOAD_ACC x3

    MULF x1                 ; a*c in acc
    MULI 4.0                ; 4*a*c in acc
    STORE_ACC x4            ; 4*a*c in x4

    LOAD_ACC x2             ; b in acc
    MULF x2                 ; b*b in acc
    SUBF x4                 ; D = b*b - 4*a*c in acc

    MVI x6, 0.0             ; 0 in x6

    BLTF x6, 100            ; D < 0

    BEQ x6, 72              ; D == 0

    ; D > 0
    SQRT                    ; sqrt(D) in acc
    STORE_ACC x6            ; sqrt(D) in x6

    LOAD_ACCI -2.0          ; -2 in acc
    MULF x1                 ; -2*a in acc
    STORE_ACC x7            ; -2*a in x7

    LOAD_ACC x6             ; sqrt(D) in acc
    DIVF x7                 ; sqrt(D) / (-2 * a) in acc
    STORE_ACC x6            ; sqrt(D) / (-2 * a) in x6

    LOAD_ACC x2             ; b in acc
    DIVF x7                 ; b / (-2 * a) in acc
    STORE_ACC x7            ; b / (-2 * a) in x7

    ADDF x6                 ; x_1

    CALL_INTRINSIC PRINTF   ; print x_1

    LOAD_ACC x7             ; b / (-2 * a) in acc
    SUBF x6                 ; x_2

    CALL_INTRINSIC PRINTF   ; print x_2


    JMP 28

    ; D == 0
    LOAD_ACCI -2.0          ; -2 in acc
    MULF x1                 ; -2*a in acc
    STORE_ACC x7            ; -2*a in x7

    LOAD_ACC x2             ; b in acc
    DIVF x7                 ; b / (-2 * a) in acc

    CALL_INTRINSIC PRINTF   ; print x_2

    RET

*/


#include <gtest/gtest.h>
#include "VirtualMachine.h"
#include "test_common.h"


TEST(InstructionTest, firstTest)
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}