#if !defined(APP_CONFIG_H_)
#define APP_CONFIG_H_

#define SENSOR_STARTUP_TIME 150

/* Speed sensor configuration */

/* Speed sensor buffer size */
#define SPM_SENSOR_BUFFER_SIZE 64
/* Maximum pulse length from the speed sensor */
#define SPM_LOW_SPEED_CUTOFF_PERIOD_US 9000000      /* Maximum time to measure in a single pole configuration */
#define SPM_HIGH_SPEED_CUTOFF_PERIOD_US 2000        /* Minimum time to measure in a 30-pole configuration */

/* Pressure sensor configuration */
#define ALT_SENSOR_STARTUP_TIME 150

/* ITPMS Service configuration */

/* ITPMS Automatic startup enable/disable */
#define ITPMS_AUTO_STARTUP 1
/* ITPMS startup time in milliseconds */
#define ITPMS_STARTUP_TIME 2000
#define ITPMS_MINIMUM_SPEED 100
#define ITPMS_SPEED_THRESHOLD 5

#endif // APP_CONFIG_H_
