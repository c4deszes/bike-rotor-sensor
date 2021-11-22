#pragma once

#include <stdint.h>
#include "tiny_sil/sch.hpp"

namespace SIL {

    class Peripheral {
        const Scheduler& scheduler;
    public:
        Peripheral(const Scheduler& scheduler);

        virtual void Update(const uint64_t nanoseconds) = 0;
    };

}
