#if !defined(BSP_OSH_PHY_H_)
#define BSP_OSH_PHY_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    osh_channel_status_ok,
    osh_channel_status_short,
    osh_channel_status_open
} osh_phy_channel_status_t;

/**
 * @brief Initializes the output stage handler, including timers and initializing the specific
 * driver implementation
 */
void osh_phy_init(void);

/**
 * @brief Turns on the output stage (all channels)
 */
void osh_phy_turn_on(void);

/**
 * @brief Turns off the output stage (all channels)
 */
void osh_phy_turn_off(void);

/**
 * @brief Initializes the output stage handler low level implementation
 * 
 * This could include setting up pins as inputs, outputs, connecting interrupts to event generators
 */
void osh_phy_impl_init(void);

/**
 * @brief Turns on the output stage (low level)
 * 
 * This could include driving pins high or low
 */
void osh_phy_impl_turn_on(void);

/**
 * @brief Turns off the output stage (low level)
 * 
 * This could include driving pins high or low
 */
void osh_phy_impl_turn_off(void);

/**
 * @brief Update function called by the scheduler, the driver can update the internal states for
 * example if IO needs to be polled regularly.
 */
void osh_phy_update(void);

/**
 * @brief Returns the event generator setup for the given channel
 * 
 * @param channel Output stage channel number
 * @return uint8_t Event generator ID
 */
uint8_t osh_phy_get_evgen(uint8_t channel);

/**
 * @brief Returns the status of the given channel
 * 
 * @param channel Output stage channel number
 * @return osh_phy_channel_status_t Channel status (ok, shorted, open)
 */
osh_phy_channel_status_t osh_phy_channel_status(uint8_t channel);

// For PHYs that can control channels independently

// bool osh_phy_has_separate_channel_control(void);

// void osh_phy_turn_on_channel(uint8_t channel);

// void osh_phy_turn_off_channel(uint8_t channel);

// void osh_phy_restart_channel(uint8_t channel);

#endif