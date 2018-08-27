#include "condition_variable.h"
#include "critical_section.h"
#include "fifo_critical_section.h"
#include "..\HSystem.h"

namespace thread {
    condition_variable::condition_variable() {
        InitializeConditionVariable(&this->cv);
    }

    condition_variable::~condition_variable() {
    }

    void condition_variable::wait(critical_section &cs, DWORD milliseconds, bool *timeout) {
        hAssert(cs.owned());
        cs.unown();

        auto res = SleepConditionVariableCS(&this->cv, cs.GetCs(), milliseconds);
        
        cs.own();

        if (!res && timeout) {
            *timeout = GetLastError() == ERROR_TIMEOUT;
        }
    }

    void condition_variable::wait(fifo_critical_section &cs, DWORD milliseconds, bool *timeout) {
        uint32_t queue_me;

        EnterCriticalSection(cs.GetCs());
        hAssert(cs.owned());
        cs.queue_head++;
        cs.unown();
        // wakeup someone before sleep
        WakeAllConditionVariable(&cs.cv);

        // here cv gets unlocked
        auto res = SleepConditionVariableCS(&this->cv, cs.GetCs(), milliseconds);
        // here cv gets locked

        if (!res && timeout) {
            *timeout = GetLastError() == ERROR_TIMEOUT;
        }

        // wait the queue. It will wait only for others who locked this <cs>
        queue_me = cs.queue_tail++;
        while (queue_me != cs.queue_head) {
            SleepConditionVariableCS(&this->cv, cs.GetCs(), INFINITE);
        }

        cs.own();
        LeaveCriticalSection(cs.GetCs());
    }

    void condition_variable::notify() {
        WakeConditionVariable(&this->cv);
    }

    void condition_variable::notify_all() {
        WakeAllConditionVariable(&this->cv);
    }
}