#ifndef DECODER_H
#define DECODER_H

#include "Instructions.h"


namespace VM {

template<int high, int low>
static inline uint64_t getPartialBits(const EncodedInstruction val) {
    constexpr const uint64_t mask = ((1ULL << (high - low + 1)) - 1) << low;
    return (val & mask);
}

template<int high, int low>
static inline uint64_t getPartialBitsShifted(const EncodedInstruction val) {
    constexpr const uint64_t mask = ((1ULL << (high - low + 1)) - 1) << low;
    return (val & mask) >> low;
}

template<int high, int low>
static inline uint64_t makePartialBits(const uint64_t val) {
    constexpr const uint64_t mask = ((1ULL << (high - low + 1)) - 1) << low;
    return ((val << low) & mask);
}


class Decoder {

private:


public:
    InstructionType getOpcode(const EncodedInstruction instr) const;
    IntrinsicType getIntrinsicType(const EncodedInstruction instr) const;

    void decodeInstruction(const EncodedInstruction encInstr, DecodedInstruction& decInstr) const;
    void encodeInstruction(const DecodedInstruction& decInstr, EncodedInstruction& encInstr) const;
};

}   // VM

#endif  // DECODER_H
