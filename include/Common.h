#ifndef COMMON_H
#define COMMON_H

#include <vector>

#include "Instructions.h"


namespace Common {

struct Program {
    uint64_t entryPoint;
    std::vector<VM::EncodedInstruction> instructions;
};

}   // Common

#endif  // COMMON_H
