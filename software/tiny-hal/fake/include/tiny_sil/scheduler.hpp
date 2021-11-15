#pragma once

enum class SchedulerState {
    NOT_INITIALIZED,
    INITIALIZED,
    RUNNING
};

class Scheduler {

    void RunNanoseconds(const uint64_t nanoseconds);

    void RunMicroseconds(const uint64_t microseconds);

    void RunMilliseconds(const uint64_t milliseconds);

    void Reset();

    uint64_t GetAbsoluteTime();

    SchedulerState GetState();

};
