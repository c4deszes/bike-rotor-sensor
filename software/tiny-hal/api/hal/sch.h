#pragma once

/**
 * @brief Initializes the scheduler
 */
void sch_init(void);

/**
 * @brief Starts running tasks
 */
void sch_enter(void);

/**
 * @brief Triggers the scheduler
 */
void sch_trigger(void);

/**
 * @brief Task executed every 1ms
 */
void sch_task1ms(void);

/**
 * @brief Task executed every 10ms
 */
void sch_task10ms(void);
