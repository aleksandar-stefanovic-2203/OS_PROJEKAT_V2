#ifndef MYSEMAPHORE_H
#define MYSEMAPHORE_H

#include "list.hpp"
#include "tcb.hpp"

class MySemaphore {
public:
    explicit MySemaphore(int init = 1) {
        this->value = init;
        this->closed = false;
    }

    int wait();
    int signal();
    static MySemaphore* createSemaphore(int init = 1);
    unsigned getValue() const { return value; }
    int close();

    int trywait();

protected:
    void block();
    void unblock();

private:
    bool closed;
    int value;
    List<TCB> blocked;
};

#endif