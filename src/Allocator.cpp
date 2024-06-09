#include "Allocator.h"
#include "VirtualMachine.h"

namespace VM {

void Allocator::allocateVirtualMachineMemory() {
    m_vm->m_memory = new uint8_t[VM_TOTAL_MEMORY_BYTESIZE];
}

void Allocator::deallocateVirtualMachineMemory() {
    delete[] m_vm->m_memory;
    m_vm->m_memory = nullptr;
}

uint8_t* Allocator::getVirtualMachineMemory() {
    return m_vm->m_memory;
}

// Allocate in a tree-like style
uint64_t Allocator::allocateObjectInternal(const uint16_t classIdx, const size_t size) {
    uint64_t addr = m_arenaPointer;
    uint8_t* memory = m_vm->m_memory;

    uint16_t* classMemory = getMemoryPtr<uint16_t, MemoryType::ClassDescs>();
    uint16_t classPtr16 = classMemory[classIdx];
    uint16_t fieldCount = classMemory[classPtr16];

    ObjectHeader objHeader{};
    objHeader.classIdx = classIdx;
    objHeader.size = static_cast<uint16_t>(size);

    std::memcpy(memory + m_arenaPointer, &objHeader, sizeof(ObjectHeader));
    m_arenaPointer += sizeof(ObjectHeader) + fieldCount * size * sizeof(uint64_t);

    for (size_t k = 0; k < size; ++k) {
        for (int i = 0; i < fieldCount; ++i) {
            uint16_t allocClassIdx = classMemory[classPtr16 + 1 + i];
            // Use inplace memory for integers, floats and string which are allocated inside an object
            if (allocClassIdx == BasicObjectType::INTEGER ||
                allocClassIdx == BasicObjectType::FLOATING ||
                allocClassIdx == BasicObjectType::STRING)
            {
                continue;
            }
            uint64_t allocAddr = allocateObjectInternal(allocClassIdx);
            uint64_t objAddr = addr + sizeof(ObjectHeader) + k * fieldCount;
            uint64_t fieldAddr = objAddr + i * sizeof(uint64_t);
            std::memcpy(memory + fieldAddr, &allocAddr, sizeof(uint64_t));
        }
    }

    return addr;
}


uint64_t Allocator::allocateObject(const uint16_t classIdx, const size_t size) {
    uint64_t retVal = m_arenaPointer;
    uint8_t* memory = m_vm->m_memory;

    // Handle basic types separately
    if (classIdx == VM::BasicObjectType::INTEGER) {
        ObjectHeader objHeader{};
        objHeader.classIdx = VM::BasicObjectType::INTEGER;
        objHeader.size = static_cast<uint16_t>(size);
        std::memcpy(memory + m_arenaPointer, &objHeader, sizeof(ObjectHeader));
        m_arenaPointer += sizeof(ObjectHeader) + size * sizeof(uint64_t);
        return retVal;
    }

    if (classIdx == VM::BasicObjectType::FLOATING) {
        ObjectHeader objHeader{};
        objHeader.classIdx = VM::BasicObjectType::FLOATING;
        objHeader.size = static_cast<uint16_t>(size);
        std::memcpy(memory + m_arenaPointer, &objHeader, sizeof(ObjectHeader));
        m_arenaPointer += sizeof(ObjectHeader) + size * sizeof(double);
        return retVal;
    }

    if (classIdx == VM::BasicObjectType::STRING) {
        ObjectHeader objHeader{};
        objHeader.classIdx = VM::BasicObjectType::STRING;
        objHeader.size = static_cast<uint16_t>(size);
        std::memcpy(memory + m_arenaPointer, &objHeader, sizeof(ObjectHeader));
        m_arenaPointer += sizeof(ObjectHeader) + objHeader.size * sizeof(uint8_t);
        return retVal;
    }

    return allocateObjectInternal(classIdx, size);
}

}
