#pragma once

enum class SchedulerState {
    NOT_INITIALIZED,
    INITIALIZED,
    RUNNING
};

class Scheduler {

    void Run(const uint64_t milliseconds);

    void Reset();

    uint64_t GetAbsoluteTime();

    SchedulerState GetState();

};
