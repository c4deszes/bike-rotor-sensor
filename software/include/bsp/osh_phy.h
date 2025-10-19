#if !defined(BSP_OSH_PHY_H_)
#define BSP_OSH_PHY_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    osh_phy_channel_state_ok,
    osh_phy_channel_state_open,
    osh_phy_channel_state_short,
    osh_phy_channel_state_off
} osh_phy_channel_state_t;

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
 * @return osh_phy_channel_state_t Channel status (ok, shorted, open)
 */
osh_phy_channel_state_t OSH_PhyGetChannelState(uint8_t channel);

void OSH_PhySetChannelStatus(uint8_t channel, osh_phy_channel_state_t status);

#endif