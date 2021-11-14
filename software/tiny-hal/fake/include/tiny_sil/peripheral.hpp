#pragma once

#include "tiny_sil/scheduler.hpp"

class Peripheral {
    Scheduler& scheduler;
protected:
    Peripheral(const Scheduler& scheduler): scheduler(scheduler);
};
