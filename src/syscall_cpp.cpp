#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"

void *operator new (size_t size){
    return MemoryAllocator::mem_alloc(size);
}

void *operator new[] (size_t size){
    return MemoryAllocator::mem_alloc(size);
}

void operator delete (void *ptr) noexcept {
    MemoryAllocator::mem_free(ptr);
}

void operator delete[] (void *ptr) noexcept {
    MemoryAllocator::mem_free(ptr);
}

Thread::Thread(void (*body)(void*), void* arg) {
    thread_create(&this->myHandle, body, arg);
}

Thread::~Thread() {
    //thread_exit();
    delete this->myHandle;
}

int Thread::start() {
    thread_start(this->myHandle);
    return 0;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return 0;
}

Thread::Thread() {
    thread_create_without_start(&this->myHandle, runWrapper, this);
}

Semaphore::Semaphore(unsigned init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

int Semaphore::timedWait(time_t timeout) {
    return sem_timedwait(this->myHandle, timeout);
}

int Semaphore::tryWait() {
    return sem_trywait(this->myHandle);
}

void Console::putc(char ch) {
    cputc(ch);
}

char Console::getc() {
    return cgetc();
}