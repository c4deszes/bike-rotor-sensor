/**
 * @file eic.h
 * @author Balazs Eszes
 * @brief External Interrupt Controller interface
 */
#ifndef ATSAME54_HAL_INCLUDE_HAL_IO_EIC_H_
#define ATSAME54_HAL_INCLUDE_HAL_IO_EIC_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    INPUT_SENSE_NONE = 0x00,
    INPUT_SENSE_RISE = 0x01,
    INPUT_SENSE_FALL = 0x02,
    INPUT_SENSE_BOTH = 0x03,
    INPUT_SENSE_HIGH = 0x04,
    INPUT_SENSE_LOW = 0x05
} eic_input_sense;

//typedef void (*eic_callback)(void);

typedef struct {
    bool event;
    bool filter;
    eic_input_sense sense;
} eic_pin_configuration;

typedef struct {
    bool filter;
    eic_input_sense sense;
} eic_nmi_configuration;

typedef struct {
    eic_nmi_configuration nmi;
    eic_pin_configuration config[16]; 
} eic_configuration;

void EIC_Initialize(const eic_configuration* conf);

void EIC_EnableInterrupt(uint8_t pin);

void EIC_DisableInterrupt(uint8_t pin);

#endif  // ATSAME54_HAL_INCLUDE_HAL_IO_EIC_H_
