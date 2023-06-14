#if !defined(BSP_OSH_PHY_H_)
#define BSP_OSH_PHY_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    osh_channel_status_ok,
    osh_channel_status_short,
    osh_channel_status_open
} osh_channel_status_t;

void osh_phy_init(void);

void osh_phy_init_internal(void);

void osh_phy_update(void);

osh_channel_status_t osh_phy_channel_status(uint8_t channel);

void osh_phy_turn_on(void);

void osh_phy_turn_off(void);

void osh_phy_restart(void);

uint8_t osh_phy_get_evgen(uint8_t channel);

// For PHYs that can control channels independently

bool osh_phy_has_separate_channel_control(void);

void osh_phy_turn_on_channel(uint8_t channel);

void osh_phy_turn_off_channel(uint8_t channel);

void osh_phy_restart_channel(uint8_t channel);

#endif