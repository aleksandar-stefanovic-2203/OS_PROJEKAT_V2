#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/mySemaphore.hpp"
#include "../h/printing.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

using Body = void (*)(void *);

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();
    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // Nije se dogodio prekid, sistemski poziv iz korisnickog ili sistemskog rezima rezima
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 code = r_a0();
        void* ptr;
        int ret;
        char chr;
        thread_t* handle;
        Body body;
        void* arg;
        sem_t sem;

        // Semaphore
        switch (code) {
            // void* mem_alloc(size_t size)
            case 0x01:
                size_t size;

                __asm__ volatile("mv %0, a1" : "=r" (size));

                ptr = MemoryAllocator::mem_alloc(size);

                __asm__ volatile("mv a0, %0" : : "r" (ptr));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // int mem_free(void*)
            case 0x02:
                __asm__ volatile("mv %0, a1" : "=r" (ptr));

                ret = MemoryAllocator::mem_free(ptr);

                __asm__ volatile("mv a0, %0" : : "r" (ret));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // void thread_start(TCB* tcb)
            case 0x09:
                TCB* tcb;
                __asm__ volatile("mv %0, a1" : "=r" (tcb));

                TCB::startThread(tcb);

                break;

            //int thread_create_without_start(thread_t* handle, void (*start_routine)(void*), void* arg);
            case 0x10:
                __asm__ volatile("mv %0, a1" : "=r" (handle));
                __asm__ volatile("mv %0, a2" : "=r" (body));
                __asm__ volatile("mv %0, a7" : "=r" (arg));
                *handle = TCB::createThreadWithoutStarting(body, arg);

                if(*handle != nullptr) {
                    __asm__ volatile("li a0, 0");
                    __asm__ volatile("sw a0, 80(x8)");
                } else {
                    __asm__ volatile("li a0, -1");
                    __asm__ volatile("sw a0, 80(x8)");
                }

                break;

            // int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg)
            case 0x11:
                __asm__ volatile("mv %0, a1" : "=r" (handle));
                __asm__ volatile("mv %0, a2" : "=r" (body));
                __asm__ volatile("mv %0, a7" : "=r" (arg));

                *handle = TCB::createThread(body, arg);

                if(*handle != nullptr) {
                    __asm__ volatile("li a0, 0");
                    __asm__ volatile("sw a0, 80(x8)");
                } else {
                    __asm__ volatile("li a0, -1");
                    __asm__ volatile("sw a0, 80(x8)");
                }

                break;

            // int thread_exit()
            case 0x12:
                TCB::running->setFinished(true);
                TCB::dispatch();
                __asm__ volatile("li a0, 0");
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // void thread_dispatch()
            case 0x13:
                TCB::dispatch();
                break;

            // int sem_open(sem_t* handle, unsigned init)
            case 0x21:
                sem_t* handleSem;
                unsigned init;
                __asm__ volatile("mv %0, a1" : "=r" (handleSem));
                __asm__ volatile("mv %0, a2" : "=r" (init));

                *handleSem = MySemaphore::createSemaphore(init);

                if(*handleSem != nullptr) {
                    __asm__ volatile("li a0, 0");
                    __asm__ volatile("sw a0, 80(x8)");
                } else {
                    __asm__ volatile("li a0, -1");
                    __asm__ volatile("sw a0, 80(x8)");
                }

                break;

            // int sem_close(sem_t handle)
            case 0x22:
                __asm__ volatile("mv %0, a1" : "=r" (sem));
                if(sem != nullptr) ret = sem->close();
                else ret = -2;

                __asm__ volatile("mv a0, %0" : : "r" (ret));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // int sem_wait(sem_t id)
            case 0x23:
                __asm__ volatile("mv %0, a1" : "=r" (sem));
                if(sem != nullptr) ret = sem->wait();
                else ret = -2;

                __asm__ volatile("mv a0, %0" : : "r" (ret));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // int sem_signal(sem_t id)
            case 0x24:
                __asm__ volatile("mv %0, a1" : "=r" (sem));
                if(sem != nullptr) ret = sem->signal();
                else ret = -2;

                __asm__ volatile("mv a0, %0" : : "r" (ret));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // int sem_timedwait(sem_t id, time_t timeout)
            case 0x25:
                // time_t timeout;
                // __asm__ volatile("mv %0, a1" : "=r" (sem));
                // __asm__ volatile("mv %0, a2" : "=r" (timeout));
                //
                // //TODO
                //
                // __asm__ volatile("mv a0, %0" : : "r" (ret));
                // __asm__ volatile("sw a0, 80(x8)");

                break;

            // int sem_trywait(sem_t id)
            case 0x26:
                __asm__ volatile("mv %0, a1" : "=r" (sem));
                if(sem != nullptr) sem->trywait();
                else ret = -2;

                __asm__ volatile("mv a0, %0" : : "r" (ret));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // char getc()
            case 0x41:
                chr = __getc();
                __asm__ volatile("mv a0, %0" : : "r" (chr));
                __asm__ volatile("sw a0, 80(x8)");

                break;

            // void putc(char)
            case 0x42:
                __asm__ volatile("mv %0, a1" : "=r" (chr));
                __putc(chr);

                break;

            default:
                break;
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if(scause == 0x8000000000000001UL) {
        // Dogodio se prekid, razlog: prekid od supervizora (tajmer)
        mc_sip(Riscv::SIP_SSIP);
        TCB::timeSliceCounter++;
        if(TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    } else if (scause == 0x8000000000000009UL) {
        // Dogodio se prekid, razlog: spoljasni prekid (konzola)
        console_handler();
    } else {
        // Neocekivani razlog prekida
        Riscv::mc_sstatus(SSTATUS_SIE);
        Riscv::ms_sstatus(SSTATUS_SPP);
        uint64 sepc = r_sepc();
        uint64 stvec = r_stvec();
        uint64 sstatus = r_sstatus();
        uint64 scause = r_scause();
        printString("Vrednost sepc:");
        printInt(sepc);
        printString(" ");
        printString("Vrednost stvec:");
        printInt(stvec);
        printString(" ");
        printString("Vrednost sstatus:");
        printInt(sstatus);
        printString(" ");
        printString("Vrednost scause: ");
        printInt(scause);
        printString("\n");

        Riscv::exitEmulator();
    }
}
