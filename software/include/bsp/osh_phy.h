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
void OSH_PhyInit(void);

/**
 * @brief Turns on the output stage (all channels)
 */
void OSH_PhyTurnOn(void);

/**
 * @brief Turns off the output stage (all channels)
 */
void OSH_PhyTurnOff(void);

/**
 * @brief Update function called by the scheduler, the driver can update the internal states for
 * example if IO needs to be polled regularly.
 */
void OSH_PhyUpdate(void);

/**
 * @brief Returns the status of the given channel
 * 
 * @param channel Output stage channel number
 * @return osh_phy_channel_status_t Channel status (ok, shorted, open)
 */
osh_phy_channel_status_t OSH_PhyGetChannelStatus(uint8_t channel);

// For PHYs that can control channels independently

// bool osh_phy_has_separate_channel_control(void);

// void osh_phy_turn_on_channel(uint8_t channel);

// void osh_phy_turn_off_channel(uint8_t channel);

// void osh_phy_restart_channel(uint8_t channel);

#endif