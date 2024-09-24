#include "../h/syscall_c.hpp"
#include "../lib/hw.h"
#include "../h/riscv.hpp"

void* mem_alloc(size_t size) {

    __asm__ volatile("mv a1, %0" : : "r"(size));
    Riscv::w_a0(0x01);
    __asm__ volatile("ecall");

    void* ret;
    ret = (void*) Riscv::r_a0();
    return ret;
}

int mem_free(void* ptr) {
    __asm__ volatile("mv a1, %0" : : "r"(ptr));
    Riscv::w_a0(0x02);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) {
    __asm__ volatile("mv a7, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    Riscv::w_a0(0x11);

    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int thread_exit() {
    Riscv::w_a0(0x12);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}


void thread_dispatch() {
    Riscv::w_a0(0x13);
    __asm__ volatile("ecall");
}

int thread_create_without_start(thread_t* handle, void (*start_routine)(void*), void* arg) {
    __asm__ volatile("mv a7, %0" : : "r"(arg));
    __asm__ volatile("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    Riscv::w_a0(0x10);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;

}

void thread_start(TCB* tcb) {
    __asm__ volatile("mv a1, %0" : : "r"(tcb));
    Riscv::w_a0(0x09);
    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init) {
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    Riscv::w_a0(0x21);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int sem_close(sem_t handle) {
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    Riscv::w_a0(0x22);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int sem_wait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    Riscv::w_a0(0x23);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    Riscv::w_a0(0x24);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int sem_timedwait(sem_t id, time_t timeout) {
    __asm__ volatile("mv a2, %0" : : "r"(timeout));
    __asm__ volatile("mv a1, %0" : : "r"(id));
    Riscv::w_a0(0x25);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}

int sem_trywait(sem_t id) {
    __asm__ volatile("mv a1, %0" : : "r"(id));
    Riscv::w_a0(0x26);
    __asm__ volatile("ecall");

    int ret = (int) Riscv::r_a0();
    return ret;
}


char getc() {
    Riscv::w_a0(0x41);
    __asm__ volatile("ecall");

    char ret = (char) Riscv::r_a0();
    return ret;
}

void putc(char ch) {
    __asm__ volatile("mv a1, %0" : : "r"(ch));
    Riscv::w_a0(0x42);
    __asm__ volatile("ecall");
}

char cgetc() {
    return getc();
}

void cputc(char ch) {
    putc(ch);
}
