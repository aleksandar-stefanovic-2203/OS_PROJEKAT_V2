#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/printing.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"

extern void userMain();

int main() {
    MemoryAllocator::initFreeSegment();

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_t threads[2];
    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];
    thread_create(&threads[1], reinterpret_cast<void (*) (void *)>(userMain), nullptr);

    while(!(threads[1]->isFinished())) thread_dispatch();

    for(auto &thread : threads) {
        delete thread;
    }

    Scheduler::deleteThreadQueue();

    printString("Proces zavrsen\n");

    Riscv::exitEmulator();

    return 0;
}