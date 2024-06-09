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

MemoryBlock* Allocator::getFreeMemoryBlock(const size_t requestBytesize) {
    MemoryBlock* memBlk = m_rootMemBlock;
    MemoryBlock* prevBlk = nullptr;
    while (true) {
        if (memBlk != nullptr) {
            if (!memBlk->used && memBlk->totalBytesize <= requestBytesize) {
                // std::cout << "Returning existing MemoryBlock at: " << memBlk << std::endl;
                return memBlk;
            }
            prevBlk = memBlk;
            memBlk = memBlk->next;
        }
        else {
            MemoryBlock newMemBlk{};
            newMemBlk.totalBytesize = static_cast<uint32_t>(requestBytesize);
            newMemBlk.used = false;
            newMemBlk.next = nullptr;

            // Out of memory
            if (!isAddrInRegion<MemoryType::Arena>(m_arenaPointer + sizeof(MemoryBlock) + requestBytesize)) {
                throw std::runtime_error("out of memory");
            }

            MemoryBlock* newBlkAddr = getMemoryPtr<MemoryBlock, MemoryType::Program>(m_arenaPointer);

            std::memcpy(newBlkAddr, &newMemBlk, sizeof(MemoryBlock));
            m_arenaPointer += sizeof(MemoryBlock) + requestBytesize;
            if (prevBlk != nullptr) {
                prevBlk->next = newBlkAddr;
            }
            // std::cout << "Creating MemoryBlock at: " << newBlkAddr << std::endl;
            return newBlkAddr;
        }
    }
}

// Allocate in a tree-like style
uint64_t Allocator::allocateObjectInternal(const uint16_t classIdx, const size_t size) {
    uint16_t* classMemory = getMemoryPtr<uint16_t, MemoryType::ClassDescs>();
    uint16_t classPtr16 = classMemory[classIdx];
    uint16_t fieldCount = classMemory[classPtr16];

    MemoryBlock* memBlk = getFreeMemoryBlock(fieldCount * size * sizeof(uint64_t));
    memBlk->objHeader.classIdx = classIdx;
    memBlk->objHeader.size = static_cast<uint16_t>(size);

    uint8_t* memory = getVirtualMachineMemory();
    uint64_t addr = ((uint8_t*)&memBlk->objHeader - getMemoryPtr<uint8_t, MemoryType::Program>());

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
    // Handle basic types separately
    if (classIdx == VM::BasicObjectType::INTEGER) {
        MemoryBlock* memBlk = getFreeMemoryBlock(size * sizeof(uint64_t));
        memBlk->objHeader.classIdx = VM::BasicObjectType::INTEGER;
        memBlk->objHeader.size = static_cast<uint16_t>(size);
        memBlk->used = true;
        return ((uint8_t*)&memBlk->objHeader - getMemoryPtr<uint8_t, MemoryType::Program>());
    }

    if (classIdx == VM::BasicObjectType::FLOATING) {
        MemoryBlock* memBlk = getFreeMemoryBlock(size * sizeof(double));
        memBlk->objHeader.classIdx = VM::BasicObjectType::FLOATING;
        memBlk->objHeader.size = static_cast<uint16_t>(size);
        memBlk->used = true;
        return ((uint8_t*)&memBlk->objHeader - getMemoryPtr<uint8_t, MemoryType::Program>());
    }

    if (classIdx == VM::BasicObjectType::STRING) {
        MemoryBlock* memBlk = getFreeMemoryBlock(size * sizeof(char));
        memBlk->objHeader.classIdx = VM::BasicObjectType::STRING;
        memBlk->objHeader.size = static_cast<uint16_t>(size);
        memBlk->used = true;
        return ((uint8_t*)&memBlk->objHeader - getMemoryPtr<uint8_t, MemoryType::Program>());
    }

    return allocateObjectInternal(classIdx, size);
}

}
