/**
 * @file signals.h
 * @author your name (you@domain.com)
 * @brief 
 * 
 * @details List of signals:
 * Position
 * Distance
 * Speed
 * 
 */
#pragma once

/**
 * @brief Returns the current angular speed
 * 
 * @return uint16_t 
 */
uint16_t sig_get_speed(void);

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t sig_get_position(void);

uint32_t sig_get_rotations(void);
