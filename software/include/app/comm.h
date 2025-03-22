#if !defined(APP_COMM_H_)
#define APP_COMM_H_

#include <stdbool.h>

/**
 * @brief Initializes the communication stack
 * - Application protocol
 * - Transport layer
 * - Physical layer
 */
void COMM_Initialize(void);

/**
 * @brief Updates the signals that are published by this peripheral, called every 10ms
 */
void COMM_UpdateFastSignals(void);

/**
 * @brief Updates the signals that are published by this peripheral, called every 100ms
 */
void COMM_UpdateSlowSignals(void);

/**
 * @brief Updates debug signals
 */
void COMM_UpdateDebugSignals(void);

/**
 * @brief Passes the bytes received from the physical layer to the transport layer
 */
void COMM_UpdatePhy(void);

#endif // APP_COMM_H_
