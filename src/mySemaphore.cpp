#include "../h/mySemaphore.hpp"
#include "../h/syscall_c.hpp"

int MySemaphore::wait() {
    if(this->closed) return -1;
    if(--this->value < 0) {
        block();
        TCB::dispatch();
        if(this->closed) return -1;
    }

    return 0;
}

int MySemaphore::signal() {
    if(this->closed) return -1;
    if(++this->value <= 0) {
        unblock();
    }

    return 0;
}

MySemaphore * MySemaphore::createSemaphore(int init) {
    return new MySemaphore(init);
}

int MySemaphore::close() {
    if(closed) return -1;
    closed = true;

    while(TCB* tcb = this->blocked.removeFirst()) {
        tcb->setBlocked(false);
        Scheduler::put(tcb);
    }
    return 0;
}

int MySemaphore::trywait() {
    if(this->closed) return -1;
    if((int)this->value - 1 < 0) {
        return 0;
    }
    return 1;
}

void MySemaphore::block() {
    this->blocked.addLast(TCB::running);
    TCB::running->setBlocked(true);
}

void MySemaphore::unblock() {
    if(this->blocked.peekFirst() == nullptr) return;
    TCB* tcb = this->blocked.removeFirst();
    tcb->setBlocked(false);
    Scheduler::put(tcb);
}
