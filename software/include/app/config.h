#if !defined(APP_CONFIG_H_)
#define APP_CONFIG_H_

/* Speed sensor configuration */

/* Channel allocation constants */
#define SPM_FRONT_SENSOR_CHANNEL 0
#define SPM_REAR_SENSOR_CHANNEL 1
#define SPM_CRANK_SENSOR_CHANNEL 2

#define SPM_SENSOR_STARTUP_TIME 300

/* Speed sensor buffer size */
#define SPM_SENSOR_BUFFER_SIZE 64
/* Maximum pulse length from the speed sensor */
#define SPM_LOW_SPEED_CUTOFF_PERIOD_100US 60000u      /* Maximum time to measure in a single pole configuration */
#define SPM_HIGH_SPEED_CUTOFF_PERIOD_US 1000u         /* Minimum time to measure in a single pole configuration */
#define SPM_MINIMUM_POSEDGE_100US 10u                 /* Minimum high/low time in the pulse (noise rejection) */

/*
 * Distance measurement configuration
 */

/*  */
#define DIST_MEASUREMENT_SENSOR_ERROR_TIMER_10MS 6000 /* 60 seconds */
/*  */
#define DIST_MEASUREMENT_DIFF_UP_THRESHOLD 100000
#define DIST_MEASUREMENT_DIFF_DOWN_THRESHOLD 50000

/*
 * Cadence sensor configuration
 */
#define CAD_LOW_SPEED_CUTOFF_100US 60000         /* Maximum time to measure in a single pole configuration (10 RPM)*/
#define CAD_HIGH_SPEED_CUTOFF_100US 3000         /* Minimum time to measure in a single pole configuration (200 RPM)*/

#define CAD_COASTING_EXTRA_TIME 100

#define CAD_POSITION_MAX 15
#define CAD_SENSOR_ERROR_HEALING_TIME 5000        /* Time to heal from a sensor error */

/*
 * Gear configuration
 */

/*  */
#define GEAR_HOLD_TIME 1000
#define GEAR_MAX_GEARS 12

// Shimano Cues Cassette 11-48
#define GEAR_CASSETTE_DEFAULT_RANGE 10
#define GEAR_CASSETTE_DEFAULT_GEAR_1 11
#define GEAR_CASSETTE_DEFAULT_GEAR_2 13
#define GEAR_CASSETTE_DEFAULT_GEAR_3 15
#define GEAR_CASSETTE_DEFAULT_GEAR_4 17
#define GEAR_CASSETTE_DEFAULT_GEAR_5 20
#define GEAR_CASSETTE_DEFAULT_GEAR_6 23
#define GEAR_CASSETTE_DEFAULT_GEAR_7 28
#define GEAR_CASSETTE_DEFAULT_GEAR_8 34
#define GEAR_CASSETTE_DEFAULT_GEAR_9 41
#define GEAR_CASSETTE_DEFAULT_GEAR_10 48
#define GEAR_CASSETTE_DEFAULT_GEAR_11 0
#define GEAR_CASSETTE_DEFAULT_GEAR_12 0

/*
 * Ride monitoring settings
 */
#define RIDE_MONITOR_AUTOSTART_ENABLED 1        /* When enabled the ride will start when above the minimum speed */
#define RIDE_MONITOR_AUTOSTART_MINIMUM_SPEED 50 /* Minimum speed for the ride to automatically begin */

/* Ride status transition parameters */
#define RIDE_MONITOR_AUTOPAUSE_ENABLED 1        /* When enabled the ride will pause when idle for  */
#define RIDE_MONITOR_AUTOUNPAUSE_ENABLED 1      /* When enabled the ride will be resumed automatically if speed is above the idle threshold */
#define RIDE_MONITOR_IDLE_SPEED 20              /* Speed below which the bicycle is considered idle */  
#define RIDE_MONITOR_IDLE_TIMEOUT_1S 2          /* Time before transitioning from Active to Idle state */
#define RIDE_MONITOR_PAUSE_TIMEOUT_1S 120       /* Time before transitioning from Idle to Paused state */
#define RIDE_MONITOR_UNPAUSE_TIMEOUT_1S 2       /* Time before transitioning from Paused to Active state */

/* To register as top speed the speed must be within a delta within this window */
#define RIDE_MONITOR_TOP_SPEED_WINDOW 5         /* 5 seconds */
#define RIDE_MONITOR_TOP_SPEED_DELTA 50         /* 5 km/h */

/*
 * Road quality monitoring configuration
 */
#define ROAD_QUALITY_ENABLED 1
/* Whether road quality monitoring is depedent on the ride status being active */
#define ROAD_QUALITY_RIDE_DEPENDENT 0
/* The minimum speed required to start road quality monitoring */
#define ROAD_QUALITY_MIN_SPEED 50   /* 5 km/h */

/*
 * Intertial Measurement Unit configuration
 */

/* When enabled the IMU will be initialized */
#define IMU_SENSOR_ENABLED 1

#define IMU_SENSOR_STARTUP_TIME 300
#define IMU_SENSOR_STARTUP_ATTEMPTS 1
#define IMU_SENSOR_STARTUP_ATTEMPT_DELAY 1000

#define IMU_SENSOR_POLL_INTERVAL 10
#define IMU_SENSOR_POLL_MAX_ERRORS 5
#define IMU_SENSOR_POLL_HEALING_TIME 100

#define IMU_SENSOR_GYRO_ENABLED 0
#define IMU_SENSOR_ACCEL_ENABLED 1

/*
 * Pressure sensor configuration
 */

/* When enabled the pressure sensor will be initialized */
#define ALT_SENSOR_ENABLED 1

/* Delays the startup (milliseconds) */
#define ALT_SENSOR_STARTUP_TIME 500

/* Maximum number of sensor startup attempts (0 = infinite) */
#define ALT_SENSOR_STARTUP_ATTEMPTS 1
/* Delay between startup attempts (milliseconds) */
#define ALT_SENSOR_STARTUP_ATTEMPT_DELAY 1000

/* Interval between sensor readouts (milliseconds) */
#define ALT_SENSOR_POLL_INTERVAL 100
/* Maximum number of errors before the sensor is considered faulty (0 = disabled) */
#define ALT_SENSOR_POLL_MAX_ERRORS 5
/* Healing time after the sensor is considered faulty (milliseconds) */
#define ALT_SENSOR_POLL_HEALING_TIME 5000

/* Default sea level pressure reference */
#define ALT_SENSOR_DEFAULT_QNH 101325

/* ITPMS Service configuration */

/* ITPMS Automatic startup enable/disable */
#define ITPMS_AUTO_STARTUP 1
/* ITPMS startup time in milliseconds */
#define ITPMS_STARTUP_TIME 300
#define ITPMS_MINIMUM_SPEED 100
#define ITPMS_SPEED_THRESHOLD 5

#endif // APP_CONFIG_H_
