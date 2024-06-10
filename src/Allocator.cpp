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
    bool tryAfterGC = false;
    while (true) {
        if (memBlk != nullptr) {
            if (!memBlk->used && requestBytesize <= memBlk->totalBytesize) {
                // std::cout << "Returning existing MemoryBlock at: " << memBlk << std::endl;
                return memBlk;
            }
            prevBlk = memBlk;
            memBlk = memBlk->next;
        }
        else {
            // Out of memory
            if (!isAddrInRegion<MemoryType::Arena>(m_arenaPointer + sizeof(MemoryBlock) + requestBytesize)) {
                if (tryAfterGC) {
                    // If we are out of memory even after cleanup
                    throw std::runtime_error("out of memory");
                }
                else {
                    // Perform cleanup and try again
                    m_vm->invokeGC();
                    memBlk = m_rootMemBlock;
                    prevBlk = nullptr;
                    tryAfterGC = true;
                    continue;
                }
            }

            MemoryBlock newMemBlk{};
            newMemBlk.totalBytesize = static_cast<uint32_t>(requestBytesize);
            newMemBlk.used = false;
            newMemBlk.next = nullptr;

            MemoryBlock* newBlkAddr = getMemoryPtr<MemoryBlock, MemoryType::Program>(m_arenaPointer);

            std::memcpy(newBlkAddr, &newMemBlk, sizeof(MemoryBlock));
            m_arenaPointer += sizeof(MemoryBlock) + requestBytesize;
            if (prevBlk != nullptr) {
                prevBlk->next = newBlkAddr;
            }
            else {
                m_rootMemBlock = newBlkAddr;
            }
            // std::cout << "Creating MemoryBlock at: " << newBlkAddr << std::endl;
            return newBlkAddr;
        }
    }
}

// Allocate in a tree-like style
uint64_t Allocator::allocateObjectInternal(const uint16_t classIdx, const size_t size) {
    size_t objBytesize = 0;
    if (size > 1) {
        // Array
        objBytesize = size * sizeof(uint64_t);
    }
    else {
        // Single
        uint16_t* classMemory = getMemoryPtr<uint16_t, MemoryType::ClassDescs>();
        uint16_t classPtr16 = classMemory[classIdx];
        uint16_t fieldCount = classMemory[classPtr16];

        objBytesize = fieldCount * sizeof(uint64_t);
    }


    MemoryBlock* memBlk = getFreeMemoryBlock(objBytesize);
    memBlk->objHeader.classIdx = classIdx;
    memBlk->objHeader.size = static_cast<uint16_t>(size);
    memBlk->used = true;

    uint8_t* memory = getVirtualMachineMemory();
    uint64_t addr = ((uint8_t*)&memBlk->objHeader - getMemoryPtr<uint8_t, MemoryType::Program>());

    // Initialize every complex object with zeros
    std::memset(memory + addr + sizeof(ObjectHeader), 0, objBytesize);
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

void Allocator::setMemoryListUnused() {
    MemoryBlock* memBlk = m_rootMemBlock;
    while (memBlk) {
        memBlk->used = false;
        memBlk = memBlk->next;
    }
}

void Allocator::setObjectUsed(const uint64_t addr) {
    MemoryBlock* objMemBlk = getMemoryPtr<MemoryBlock, MemoryType::Program>(addr - sizeof(MemoryBlock*) - sizeof(bool) - sizeof(uint32_t));
    objMemBlk->used = true;
}

}
