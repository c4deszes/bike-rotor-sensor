#pragma once

#include <stdint.h>
#include "tiny_sil/scheduler.hpp"

class Peripheral {
    Scheduler& scheduler;
protected:
    Peripheral(const Scheduler& scheduler);

    void Update(const uint64_t nanoseconds);
};
