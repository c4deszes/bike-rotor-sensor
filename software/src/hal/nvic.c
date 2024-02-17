#include "atsamd21e18a.h"

extern uint32_t _estack;

void Dummy_Handler(void);

/* Brief default interrupt handler for unused IRQs.*/
void __attribute__((optimize("-O1"),long_call,noreturn))Dummy_Handler(void)
{
    while (1)
    {
    }
}
/* Device vectors list dummy definition*/
void Reset_Handler              ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void NonMaskableInt_Handler     ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void HardFault_Handler          ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SVCall_Handler             ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void PendSV_Handler             ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SysTick_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void PM_Handler                 ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SYSCTRL_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void WDT_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void RTC_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void EIC_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void NVMCTRL_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void DMAC_Handler               ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void USB_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void EVSYS_Handler              ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SERCOM0_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SERCOM1_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SERCOM2_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void SERCOM3_Handler            ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void TCC0_Handler               ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void TCC1_Handler               ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void TCC2_Handler               ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void TC3_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void TC4_Handler               ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void TC5_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void ADC_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void AC_Handler                 ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void DAC_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void PTC_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));
void I2S_Handler                ( void ) __attribute__((weak, alias("Dummy_Handler"),long_call,noreturn));

/* Mutiple handlers for vector */

__attribute__ ((section(".vectors")))
const DeviceVectors exception_table=
{
    /* Configure Initial Stack Pointer, using linker-generated symbols */
    .pvStack = (void*) (&_estack),

    .pfnReset_Handler              = ( void * ) Reset_Handler,
    .pfnNonMaskableInt_Handler     = ( void * ) NonMaskableInt_Handler,
    .pfnHardFault_Handler          = ( void * ) HardFault_Handler,
    .pfnSVCall_Handler             = ( void * ) SVCall_Handler,
    .pfnPendSV_Handler             = ( void * ) PendSV_Handler,
    .pfnSysTick_Handler            = ( void * ) SysTick_Handler,
    .pfnPM_Handler                 = ( void * ) PM_Handler,
    .pfnSYSCTRL_Handler            = ( void * ) SYSCTRL_Handler,
    .pfnWDT_Handler                = ( void * ) WDT_Handler,
    .pfnRTC_Handler                = ( void * ) RTC_Handler,
    .pfnEIC_Handler                = ( void * ) EIC_Handler,
    .pfnNVMCTRL_Handler            = ( void * ) NVMCTRL_Handler,
    .pfnDMAC_Handler               = ( void * ) DMAC_Handler,
    .pfnUSB_Handler                = ( void * ) USB_Handler,
    .pfnEVSYS_Handler              = ( void * ) EVSYS_Handler,
    .pfnSERCOM0_Handler            = ( void * ) SERCOM0_Handler,
    .pfnSERCOM1_Handler            = ( void * ) SERCOM1_Handler,
    .pfnSERCOM2_Handler            = ( void * ) SERCOM2_Handler,
    .pfnSERCOM3_Handler            = ( void * ) SERCOM3_Handler,
    .pfnTCC0_Handler               = ( void * ) TCC0_Handler,
    .pfnTCC1_Handler               = ( void * ) TCC1_Handler,
    .pfnTCC2_Handler               = ( void * ) TCC2_Handler,
    .pfnTC3_Handler                = ( void * ) TC3_Handler,
    .pfnTC4_Handler                = ( void * ) TC4_Handler,
    .pfnTC5_Handler                = ( void * ) TC5_Handler,
    .pfnADC_Handler                = ( void * ) ADC_Handler,
    .pfnAC_Handler                 = ( void * ) AC_Handler,
    .pfnDAC_Handler                = ( void * ) DAC_Handler,
    .pfnPTC_Handler                = ( void * ) PTC_Handler,
    .pfnI2S_Handler                = ( void * ) I2S_Handler,
};

void NVIC_Initialize(void) {
    __DMB();
    __enable_irq();
}
