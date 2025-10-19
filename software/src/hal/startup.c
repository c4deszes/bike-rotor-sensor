#include "atsamd21e18a.h"

#include <sys/cdefs.h>
#include <stdbool.h>

/* Initialize segments */
extern uint32_t __svectors;

int main(void);
extern void __attribute__((long_call)) __libc_init_array(void);

extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void __attribute__((optimize("-O1"), section(".text.Reset_Handler"), long_call, noreturn)) Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    /* Initialize the initialized data section */
    pSrc = &__etext;
    pDest = &__data_start__;

    if ((&__data_start__ != &__data_end__) && (pSrc != pDest)) {
        for (; pDest < &__data_end__; pDest++, pSrc++) {
            *pDest = *pSrc;
        }
    }

    /* Clear the zero section */
    if ((&__data_start__ != &__data_end__) && (pSrc != pDest)) {
        for (pDest = &__bss_start__; pDest < &__bss_end__; pDest++) {
            *pDest = 0;
        }
    }


    /* Set the vector table base address */
    pSrc = (uint32_t *) & _sfixed;
    SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

    /* Initialize the C library */
    __libc_init_array();

    /* Branch to main function */
    main();

    /* Infinite loop */
    while (1);
}
