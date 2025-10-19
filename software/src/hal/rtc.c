#include "hal/rtc.h"
#include "hal/evsys.h"

#include "atsamd21e18a.h"

static inline void RTC_WaitForSync(void) {
    while((RTC_REGS->MODE0.RTC_STATUS & RTC_STATUS_SYNCBUSY_Msk) == RTC_STATUS_SYNCBUSY_Msk);
}

static inline void RTC_CountReadSynchronization(void)
{
   /* Enable continuous read-synchronization for COUNT register */
   if( (RTC_REGS->MODE0.RTC_READREQ & RTC_READREQ_RCONT_Msk) != RTC_READREQ_RCONT_Msk)
   {
       RTC_REGS->MODE0.RTC_READREQ = RTC_READREQ_RCONT_Msk | RTC_READREQ_ADDR(0x10);
       RTC_REGS->MODE0.RTC_READREQ |= RTC_READREQ_RREQ_Msk;
       RTC_WaitForSync();
   }
}

void RTC_Initialize(void) {
    /* Writing to CTRL register will trigger write-synchronization */
   RTC_REGS->MODE0.RTC_CTRL = RTC_MODE0_CTRL_SWRST_Msk;
   RTC_WaitForSync();

   /* Writing to CTRL register will trigger write-synchronization */
   RTC_REGS->MODE0.RTC_CTRL = RTC_MODE0_CTRL_MODE(0) | RTC_MODE0_CTRL_PRESCALER(0x3) ;
   RTC_WaitForSync();

   RTC_REGS->MODE0.RTC_EVCTRL = RTC_MODE0_EVCTRL_PEREO0_Msk;
   RTC_WaitForSync();

   //EVSYS_ConfigureGenerator(8, RISING_EDGE, ASYNCHRONOUS, 0x4);

   RTC_CountReadSynchronization();
}

void RTC_Start(void) {
    RTC_REGS->MODE0.RTC_CTRL |= RTC_MODE0_CTRL_ENABLE_Msk;

    RTC_WaitForSync();
}

uint32_t RTC_GetCount(void) {
    RTC_CountReadSynchronization();
    return(RTC_REGS->MODE0.RTC_COUNT);
}