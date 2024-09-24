#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include "../lib/hw.h"
#include "tcb.hpp"
#include "mySemaphore.hpp"

class TCB;
typedef TCB* thread_t;

class MySemaphore;
typedef MySemaphore* sem_t;

typedef unsigned long time_t;

const int EOF = -1;

// Upravljanje memorijom
void* mem_alloc(size_t size); //Alociranje najmanje size bajtova memorije
int mem_free(void*); //Dealociranje memorije sa date adrese

// Upravljanje procesima
// Niti
int thread_create(
    thread_t* handle,
    void (*start_routine)(void*),
    void* arg
);
int thread_exit();
void thread_dispatch();

int thread_create_without_start(thread_t* handle, void (*start_routine)(void*), void* arg);
void thread_start(TCB* tcb);

// Semafori
int sem_open(
    sem_t* handle,
    unsigned init
);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_timedwait(
    sem_t id,
    time_t timeout
);
int sem_trywait(sem_t id);

int time_sleep(time_t);

char getc();
void putc(char);
char cgetc();
void cputc(char);

#endif