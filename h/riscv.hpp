#ifndef RISCV_H
#define RISCV_H

#include "../lib/hw.h"

class Riscv {
public:
    // Stavljanje/skidanje registara x3,...,x31 na stek/sa steka
    static void pushRegisters();
    static void popRegisters();

    // Uzimanje vrednosti sstatus.spp i sstatus.spie bita (nije inline funkcija)
    static void popSppSpie();

    // Citanje vrednosti registra scause
    static uint64 r_scause();

    // Upisivanje u registar scause
    static void w_scause(uint64 scause);

    // Citanje registra sepc
    static uint64 r_sepc();

    // Upisivanje u registar sepc
    static void w_sepc(uint64 sepc);

    // Citanje registra stvec
    static uint64 r_stvec();

    // Upisivanje u registar stvec
    static void w_stvec(uint64 stvec);

    // Citanje registra stval
    static uint64 r_stval();

    // Upisivanje u registar stval
    static void w_stval(uint64 stval);

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };

    // Postavi masku za registar sip
    static void ms_sip(uint64 mask);

    // Obrisi masku iz registra sip
    static void mc_sip(uint64 mask);

    // Citanje registra sip
    static uint64 r_sip();

    // Upisivanje u registar sip
    static void w_sip(uint64 sip);

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // Postavi masku za registar sstatus
    static void ms_sstatus(uint64 mask);

    // Obrisi masku iz registra sstatus
    static void mc_sstatus(uint64 mask);

    // Citanje registra sstatus
    static uint64 r_sstatus();

    // Upisivanje u registar sstatus
    static void w_sstatus(uint64 sstatus);

    // Supervizorska prekidna rutina
    static void supervisorTrap();

    // Citanje registra a0
    static uint64 r_a0();

    // Upisivanje u registar a0
    static void w_a0(uint64 value);

    static void exitEmulator();

private:
    // Rukovalac prekidnom rutinom
    static void handleSupervisorTrap();
};

inline uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile("csrr %0, scause" : "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile("csrw scause, %0" : : "r"(scause));
}

inline uint64 Riscv::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile("csrr %0, sepc" : "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile("csrw sepc, %0" : : "r"(sepc));
}

inline uint64 Riscv::r_stvec() {
    uint64 volatile stvec;
    __asm__ volatile("csrr %0, stvec" : "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile("csrw stvec, %0" : : "r"(stvec));
}

inline uint64 Riscv::r_stval() {
    uint64 volatile stval;
    __asm__ volatile("csrr %0, stval" : "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval) {
    __asm__ volatile("csrw stval, %0" : : "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask) {
    __asm__ volatile("csrs sip, %0" : : "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask) {
    __asm__ volatile("csrc sip, %0" : : "r"(mask));
}

inline uint64 Riscv::r_sip() {
    uint64 volatile sip;
    __asm__ volatile("csrr %0, sip" : "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip) {
    __asm__ volatile("csrw sip, %0" : : "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile("csrs sstatus, %0" : : "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask) {
    __asm__ volatile("csrc sstatus, %0" : : "r"(mask));
}

inline uint64 Riscv::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile("csrr %0, sstatus" : "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile("csrw sstatus, %0" : : "r"(sstatus));
}

inline uint64 Riscv::r_a0() {
    uint64 volatile a0;
    __asm__ volatile("mv %0, a0" : "=r"(a0));
    return a0;
}

inline void Riscv::w_a0(uint64 value) {
    __asm__ volatile("mv a0, %0" : : "r"(value));
}

inline void Riscv::exitEmulator() {
    __asm__ volatile("li t0, 0x5555");
    __asm__ volatile("li t1, 0x100000");
    __asm__ volatile("sw t0, 0(t1)");
}

#endif