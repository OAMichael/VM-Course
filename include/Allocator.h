#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "Instructions.h"

namespace VM {

class VirtualMachine;

struct MemoryBlock {
    uint32_t totalBytesize;     // This is total bytesize of allocated region
    bool used;                  // Region is used
    MemoryBlock* next;          // Next memory block in the list
    ObjectHeader objHeader;     // Object header for region
};


class Allocator {
private:
    VirtualMachine* const m_vm;
    uint64_t m_arenaPointer = VM_ARENA_MEMORY_ADDRESS;
    MemoryBlock* m_rootMemBlock = nullptr;

    uint64_t allocateObjectInternal(const uint16_t classIdx, const size_t size = 1);
    uint8_t* getVirtualMachineMemory();

    MemoryBlock* getFreeMemoryBlock(const size_t requestBytesize);

public:
    void allocateVirtualMachineMemory();
    void deallocateVirtualMachineMemory();

    Allocator(VirtualMachine* p_vm) : m_vm{p_vm} {};
    uint64_t allocateObject(const uint16_t classIdx, const size_t size = 1);
    void setMemoryListUnused();
    void setObjectUsed(const uint64_t addr);

    template<typename T = uint8_t, MemoryType memT = MemoryType::Program>
    inline T* getMemoryPtr(uint64_t addr = 0) {
        uint8_t* memory = getVirtualMachineMemory();
        if constexpr (memT == MemoryType::Program) {
            return (T*)(memory + VM_PROGRAM_MEMORY_ADDRESS + addr);
        }

        if constexpr (memT == MemoryType::ConstantPool) {
            return (T*)(memory + VM_CONSTANT_POOL_MEMORY_ADDRESS + addr);
        }

        if constexpr (memT == MemoryType::StringPool) {
            return (T*)(memory + VM_STRING_POOL_MEMORY_ADDRESS + addr);
        }

        if constexpr (memT == MemoryType::ClassDescs) {
            return (T*)(memory + VM_CLASS_DESCS_MEMORY_ADDRESS + addr);
        }

        if constexpr (memT == MemoryType::Arena) {
            return (T*)(memory + VM_ARENA_MEMORY_ADDRESS + addr);
        }

        return nullptr;
    }

    template<MemoryType memT = MemoryType::Program>
    inline bool isAddrInRegion(const uint64_t addr) const {
        if constexpr (memT == MemoryType::Program) {
            return (addr >= VM_PROGRAM_MEMORY_ADDRESS) && (addr < VM_PROGRAM_MEMORY_ADDRESS + VM_PROGRAM_MEMORY_BYTESIZE);
        }

        if constexpr (memT == MemoryType::ConstantPool) {
            return (addr >= VM_CONSTANT_POOL_MEMORY_ADDRESS) && (addr < VM_CONSTANT_POOL_MEMORY_ADDRESS + VM_CONSTANT_POOL_MEMORY_BYTESIZE);
        }

        if constexpr (memT == MemoryType::StringPool) {
            return (addr >= VM_STRING_POOL_MEMORY_ADDRESS) && (addr < VM_STRING_POOL_MEMORY_ADDRESS + VM_STRING_POOL_MEMORY_BYTESIZE);
        }

        if constexpr (memT == MemoryType::ClassDescs) {
            return (addr >= VM_CLASS_DESCS_MEMORY_ADDRESS) && (addr < VM_CLASS_DESCS_MEMORY_ADDRESS + VM_CLASS_DESCS_MEMORY_BYTESIZE);
        }

        if constexpr (memT == MemoryType::Arena) {
            return (addr >= VM_ARENA_MEMORY_ADDRESS) && (addr < VM_ARENA_MEMORY_ADDRESS + VM_ARENA_MEMORY_BYTESIZE);
        }

        return false;
    }
};

}

#endif  // ALLOCATOR_H
