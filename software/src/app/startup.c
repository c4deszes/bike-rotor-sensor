#include "atsamd21e18a.h"

#include <sys/cdefs.h>
#include <stdbool.h>

/* Initialize segments */
extern uint32_t __svectors;

int main(void);
extern void __attribute__((long_call)) __libc_init_array(void);

extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void __attribute__((optimize("-O1"), section(".text.Reset_Handler"), long_call, noreturn)) Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    /* Initialize the relocate segment */
    pSrc = &_etext;
    pDest = &_srelocate;

    if (pSrc != pDest) {
            for (; pDest < &_erelocate;) {
                    *pDest++ = *pSrc++;
            }
    }

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;) {
            *pDest++ = 0;
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
