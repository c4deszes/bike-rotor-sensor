#pragma once

/**
 * @brief Initializes the scheduler
 */
void sch_init(void);

/**
 * @brief 
 * 
 */
void sch_enter(void);

/**
 * @brief Triggers the scheduler
 */
void sch_trigger(void);

/**
 * @brief Task executed every 100us
 */
void sch_task100us(void);

/**
 * @brief Task executed every 1ms
 */
void sch_task1ms(void);

/**
 * @brief Task executed every 10ms
 */
void sch_task10ms(void);
