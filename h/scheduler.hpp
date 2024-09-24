#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "list.hpp"

class TCB;

class Scheduler {
public:
    static List<TCB> readyThreadQueue;
    static void deleteThreadQueue();

    static TCB* get();
    static void put(TCB* tcb);
};

#endif