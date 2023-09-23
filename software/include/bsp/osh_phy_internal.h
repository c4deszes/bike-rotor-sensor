#if !defined(BSP_OSH_PHY_INTERNAL_H_)
#define BSP_OSH_PHY_INTERNAL_H_

#include <stdint.h>
#include "bsp/osh_phy.h"

/**
 * @brief Initializes the output stage handler low level implementation
 * 
 * This could include setting up pins as inputs, outputs, connecting interrupts to event generators
 */
void OSH_PhyInit_Impl(void);

/**
 * @brief Turns on the output stage (low level)
 * 
 * This could include driving pins high or low
 */
void OSH_PhyTurnOn_Impl(void);

/**
 * @brief Turns off the output stage (low level)
 * 
 * This could include driving pins high or low
 */
void OSH_PhyTurnOff_Impl(void);

/**
 * @brief Returns the event generator setup for the given channel
 * 
 * @param channel Output stage channel number
 * @return uint8_t Event generator ID
 */
uint8_t OSH_PhyGetEventGenerator(uint8_t channel);

void OSH_PhySetChannelStatus(uint8_t channel, osh_phy_channel_status_t status);

#endif // BSP_OSH_PHY_INTERNAL_H_
