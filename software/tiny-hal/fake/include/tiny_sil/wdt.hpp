#pragma once

#include "tiny_sil/peripheral.hpp"
#include <stdint.h>

extern "C" {
    #include "hal/wdt.h"
}

namespace SIL {

    class Watchdog : public Peripheral {
        uint32_t timer;
    public:
        Watchdog(const Scheduler& scheduler);

        virtual void Update(const uint64_t nanoseconds);
    };

    extern Watchdog watchdog_peripheral;

}