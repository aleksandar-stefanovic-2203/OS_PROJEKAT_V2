# RISC-V Microkernel

Educational operating system project developed as part of the "Operating systems 1" course at the School of Electrical Engineering, University of Belgrade, focused on implementing core kernel mechanisms on RISC-V (memory allocator, threads, scheduler, semaphores, and syscall interfaces in C/C++).

## Overview

This repository contains a small teaching kernel and userspace API used to test core operating system concepts:

- dynamic memory allocation (`mem_alloc`, `mem_free`)
- thread management and context switching
- cooperative/preemptive dispatch flow
- semaphore synchronization primitives
- trap/syscall handling on RISC-V
- C and C++ syscall wrappers and API tests

## Tech Stack

- **Language:** C, C++, RISC-V assembly
- **Target architecture:** `rv64ima` (`lp64`)
- **Build system:** `Makefile`
- **Emulation/debug:** QEMU + GDB

## Project Structure

- `src/` - kernel implementation (allocator, scheduler, TCB, syscall handling, traps, low-level assembly)
- `h/` - headers and public kernel/API interfaces
- `test/` - API and synchronization test scenarios
- `lib/` - supporting low-level libraries (`hw`, `mem`, `console`)
- `Makefile` - build, run, debug, and cleanup targets
- `kernel.ld` - linker script

## Build and Run

### Prerequisites

- RISC-V cross toolchain (`riscv64-unknown-elf-*` or compatible)
- `qemu-system-riscv64`
- `make`

### Commands

```bash
make          # build kernel image
make qemu     # run kernel in QEMU
make qemu-gdb # start QEMU paused for GDB attach
make clean    # remove build artifacts
```

If `TOOLPREFIX` is not detected automatically, set it explicitly:

```bash
make TOOLPREFIX=riscv64-unknown-elf-
```

## Implemented Interfaces

- C syscall API in `h/syscall_c.hpp`
- C++ wrappers in `h/syscall_cpp.hpp`
- thread abstraction (`Thread`, `PeriodicThread`)
- semaphore abstraction (`Semaphore`)

## Testing

Test sources are located in `test/` and cover:

- C API threading scenarios
- C++ API threading scenarios
- producer/consumer synchronization
- system mode behavior checks

## License

This project is distributed under the terms of the `LICENSE` file included in this repository.
