#include "tiny_sil/wdt.hpp"

#include "tiny_sil/sch.hpp"

using namespace SIL;

Watchdog watchdog_peripheral = Watchdog(global_scheduler);

Watchdog::Watchdog(const Scheduler& scheduler): Peripheral(scheduler) {

}

void wdt_init_normal(const wdt_period period) {
    
}
