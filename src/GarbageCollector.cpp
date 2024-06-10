#include "GarbageCollector.h"
#include "VirtualMachine.h"

namespace VM {

GarbageCollector::GarbageCollector(VirtualMachine* p_vm) :
    m_vm{p_vm},
    allocator{&p_vm->m_allocator},
    interpreter{&m_vm->m_interpreter} {};


// This addr is pointing to object header
void GarbageCollector::visitAliveObject(const uint64_t addr) {
    allocator->setObjectUsed(addr);

    memory = m_vm->m_memory;

    uint8_t* objAddr = allocator->getMemoryPtr<uint8_t, MemoryType::Program>(addr);
    ObjectHeader objectHeader{};
    std::memcpy(&objectHeader, objAddr, sizeof(ObjectHeader));

    uint16_t* classMemory = allocator->getMemoryPtr<uint16_t, MemoryType::ClassDescs>();
    uint16_t classPtr16 = classMemory[objectHeader.classIdx];
    uint16_t fieldCount = classMemory[classPtr16];

    if (objectHeader.classIdx == BasicObjectType::STRING) {
        // TODO: array of strings
        return;
    }

    size_t size = objectHeader.size;

    for (size_t k = 0; k < size; ++k) {
        for (int i = 0; i < fieldCount; ++i) {
            uint16_t fieldClassIdx = classMemory[classPtr16 + 1 + i];
            if (fieldClassIdx == BasicObjectType::INTEGER ||
                fieldClassIdx == BasicObjectType::FLOATING ||
                fieldClassIdx == BasicObjectType::STRING)
            {
                continue;
            }

            // TODO: manage externally allocated strings strings
            uint64_t objAddr = addr + sizeof(ObjectHeader) + k * fieldCount * sizeof(uint64_t);
            uint64_t fieldAddr = objAddr + i * sizeof(uint64_t);

            uint64_t fieldRef = 0;
            std::memcpy(&fieldRef, memory + fieldAddr, sizeof(uint64_t));
            if (fieldRef != 0) {
                visitAliveObject(fieldRef);
            }
        }
    }
}


void GarbageCollector::markAndSweepDeadObjectsInternal(const Frame* frame) {
    if (frame == nullptr) {
        return;
    }

    for (uint32_t i = 0; i < FRAME_REGISTER_COUNT; ++i) {
        if (frame->regfile[i].isRef && frame->regfile[i].i_val != 0) {
            visitAliveObject(frame->regfile[i].i_val);
        }
    }

    markAndSweepDeadObjectsInternal(frame->parent);
}


void GarbageCollector::markAndSweepDeadObjects() {
    // std::cout << "GC Start" << std::endl;
    allocator->setMemoryListUnused();
    
    const Register accVal = interpreter->getAccumulatorValue();
    if (accVal.isRef && accVal.i_val != 0) {
        visitAliveObject(accVal.i_val);
    }
    
    const Frame* currFrame = interpreter->getCurrentFrame();
    markAndSweepDeadObjectsInternal(currFrame);
    // std::cout << "GC End" << std::endl;
}

}   // VM
