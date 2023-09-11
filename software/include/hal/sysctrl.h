#if !defined(HAL_SYSCTRL_H_)
#define HAL_SYSCTRL_H_

/**
 * @brief Enables the internal 32.768kHz oscillator
 */
void SYSCTRL_EnableInternalOSC32K(void);

// TODO: support for changing the prescaler
/**
 * @brief Configures and enables the internal 8MHz oscillator
 */
void SYSCTRL_ConfigureOSC8M(void);

#endif // HAL_SYSCTRL_H_
