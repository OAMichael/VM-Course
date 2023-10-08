#ifndef DECODER_H
#define DECODER_H

#include "Instructions.h"


namespace VM {

template<int high, int low>
static inline uint32_t getPartialBits(const EncodedInstruction val) {
    constexpr const uint32_t mask = ((1 << (high - low + 1)) - 1) << low;
    return (val & mask);
}

template<int high, int low>
static inline uint32_t getPartialBitsShifted(const EncodedInstruction val) {
    constexpr const uint32_t mask = ((1 << (high - low + 1)) - 1) << low;
    return (val & mask) >> low;
}

template<int high, int low>
static inline uint32_t makePartialBits(const uint32_t val) {
    constexpr const uint32_t mask = ((1 << (high - low + 1)) - 1) << low;
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
