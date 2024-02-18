#if !defined(HAL_GCLK_H_)
#define HAL_GCLK_H_

#include <stdint.h>

#define GCLK_GEN0 0u
#define GCLK_GEN1 1u
#define GCLK_GEN2 2u
#define GCLK_GEN3 3u
#define GCLK_GEN4 4u
#define GCLK_GEN5 5u

/**
 * @brief Configures a Generic Clock
 * 
 * @code
 * // Generator 0 driven by DFLL48M with no division
 * GCLK_ConfigureGenerator(GCLK_GEN0, GCLK_GENCTRL_SRC_DFLL48M_Val, 0u);
 * @endcode
 * 
 * @param generator Generator number
 * @param source Generator's clock source
 * @param division Clock division (0 means no division)
 */
void GCLK_ConfigureGenerator(uint8_t generator, uint8_t source, uint16_t division);

/**
 * @brief Selects the Generic Clock source for the given peripheral
 * 
 * @code
 * // Selecting generator 3 for EIC
 * GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EIC_Val, GCLK_GEN3);
 * @endcode
 * 
 * @param id Peripheral ID
 * @param generator Generator number
 */
void GCLK_SelectGenerator(uint8_t id, uint8_t generator);

/**
 * @brief Resets all Generic Clock configurations
 */
void GCLK_Reset(void);

#endif // HAL_GCLK_H_
