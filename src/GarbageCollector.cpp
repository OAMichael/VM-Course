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

    // If object is integer, float, string, array of integers or array of floats,
    // then we have already marked them as used objects
    if (objectHeader.classIdx == BasicObjectType::INTEGER ||
        objectHeader.classIdx == BasicObjectType::FLOATING ||
        objectHeader.classIdx == BasicObjectType::STRING) {
        // TODO: array of strings
        return;
    }

    // Now only complex objects are left
    size_t size = objectHeader.size;
    if (size > 1) {
        // Array
        for (size_t i = 0; i < size; ++i) {
            uint64_t refAddr = addr + sizeof(ObjectHeader) + i * sizeof(uint64_t);
            uint64_t ref = 0;
            std::memcpy(&ref, memory + refAddr, sizeof(uint64_t));
            if (ref != 0) {
                visitAliveObject(ref);
            }
        }
    }
    else {
        // Single
        uint16_t* classMemory = allocator->getMemoryPtr<uint16_t, MemoryType::ClassDescs>();
        uint16_t classPtr16 = classMemory[objectHeader.classIdx];
        uint16_t fieldCount = classMemory[classPtr16];

        for (int i = 0; i < fieldCount; ++i) {
            uint16_t fieldClassIdx = classMemory[classPtr16 + 1 + i];
            if (fieldClassIdx == BasicObjectType::INTEGER ||
                fieldClassIdx == BasicObjectType::FLOATING ||
                fieldClassIdx == BasicObjectType::STRING)
            {
                continue;
            }

            uint64_t fieldAddr = addr + sizeof(ObjectHeader) + i * sizeof(uint64_t);
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
