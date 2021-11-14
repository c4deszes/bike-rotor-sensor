#pragma once

typedef enum {
    CPUINT_IVSEL_BOOT,
    CPUINT_IVSEL_APP
} cpuint_ivsel_location;

/**
 * @brief Changes the interrupt vector table location
 * 
 * @param loc Boot or Application locations
 */
void cpuint_ivsel(cpuint_ivsel_location loc);
