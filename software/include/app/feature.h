#if !defined(APP_FEATURE_H_)
#define APP_FEATURE_H_

/******************************************************************************
 * Speed sensor configuration
 ******************************************************************************/
#define SPM_FRONT_SENSOR_CHANNEL 0                          /* Front wheel sensor channel */
#define SPM_REAR_SENSOR_CHANNEL 1                           /* Rear wheel sensor channel */
#define SPM_CRANK_SENSOR_CHANNEL 2                          /* Crank sensor channel */

#define SPM_SENSOR_STARTUP_TIME 300                         /* Sensor startup time (milliseconds) */

#define SPM_SENSOR_BUFFER_SIZE 64                           /* Speed sensor buffer size */

#define SPM_LOW_SPEED_CUTOFF_PERIOD_100US 60000u            /* Maximum time to measure in a single pole configuration */
#define SPM_HIGH_SPEED_CUTOFF_PERIOD_US 1000u               /* Minimum time to measure in a single pole configuration */
#define SPM_MINIMUM_POSEDGE_100US 10u                       /* Minimum high/low time in the pulse (noise rejection) */

/******************************************************************************
 * Distance measurement configuration
 ******************************************************************************/
#define DIST_MEASUREMENT_SENSOR_ERROR_TIMER_10MS 6000       /* Multiple sensor failures  */

#define DIST_MEASUREMENT_DIFF_UP_THRESHOLD 100000           /* Maximum distance difference between the two wheels */
#define DIST_MEASUREMENT_DIFF_DOWN_THRESHOLD 50000          /* Minimum distance difference between the two wheels */

/******************************************************************************
 * Cadence sensor configuration
 ******************************************************************************/
#define CAD_LOW_SPEED_CUTOFF_100US 60000                    /* Maximum time to measure in a single pole configuration (10 RPM)*/
#define CAD_HIGH_SPEED_CUTOFF_100US 3000                    /* Minimum time to measure in a single pole configuration (200 RPM)*/

// TODO: this should be dynamic, dependent on the current cadence
#define CAD_COASTING_EXTRA_TIME 100                         /* Extra time to consider the crank is coasting */

#define CAD_POSITION_MAX 15                                 /* Maximum number of crank positions distinguished */
#define CAD_SENSOR_ERROR_HEALING_TIME 5000                  /* Time to heal from a sensor error */

/******************************************************************************
 * Gear configuration
 ******************************************************************************/
#define GEAR_HOLD_TIME 1000                                 /* Time to hold the gear before changing */
#define GEAR_MAX_GEARS 12                                   /* Maximum number of gears */
#define GEAR_CASSETTE_MIN_TEETH 10
#define GEAR_CASSETTE_MAX_TEETH 50

/******************************************************************************
 * Altitude sensor configuration
 ******************************************************************************/
#define ALT_SENSOR_STARTUP_TIME 500                         /* Sensor startup time (milliseconds) */
#define ALT_SENSOR_STARTUP_ATTEMPTS 1                       /* Maximum number of sensor startup attempts (0 = infinite) */
#define ALT_SENSOR_STARTUP_ATTEMPT_DELAY 1000               /* Delay between startup attempts (milliseconds) */

#define ALT_SENSOR_POLL_MAX_ERRORS 5                        /* Maximum number of errors before the sensor is considered faulty (0 = disabled) */
#define ALT_SENSOR_POLL_HEALING_TIME 5000                   /* Healing time after the sensor is considered faulty (milliseconds) */

/******************************************************************************
 * Inertial Measurement Unit configuration
 ******************************************************************************/
#define IMU_SENSOR_GYRO_ENABLED 0                           /* Enables the gyroscope */
#define IMU_SENSOR_ACCEL_ENABLED 1                          /* Enables the accelerometer */

#define IMU_SENSOR_STARTUP_TIME 300                         /* Sensor startup time (milliseconds) */
#define IMU_SENSOR_STARTUP_ATTEMPTS 1                       /* Maximum number of sensor startup attempts (0 = infinite) */
#define IMU_SENSOR_STARTUP_ATTEMPT_DELAY 1000               /* Delay between startup attempts (milliseconds) */

#define IMU_SENSOR_POLL_MAX_ERRORS 5
#define IMU_SENSOR_POLL_HEALING_TIME 100

// TODO: selectable accelerometer and gyroscope range

/******************************************************************************
 * Ride monitoring configuration
 ******************************************************************************/
#define RIDE_MONITOR_TOP_SPEED_WINDOW   5                   /* Number of samples to consider for the top speed */
#define RIDE_MONITOR_TOP_SPEED_DELTA    50                  /* Maximum speed difference in the top speed window */

/******************************************************************************
 * iTPMS configuration
 ******************************************************************************/

/******************************************************************************
 * Road quality monitoring configuration
 ******************************************************************************/
#define ROAD_QUALITY_ENABLED            1                   /* Whether road quality monitoring is enabled */

#define ROAD_QUALITY_RIDE_DEPENDENT     1                   /* Whether road quality monitoring is depedent on the ride status being active */
#define ROAD_QUALITY_MIN_SPEED          50                  /* The minimum speed required to start road quality monitoring */

/******************************************************************************
 * System state configuration
 ******************************************************************************/
#define SYS_STATE_RESET_DELAY 100
#define SYS_STATE_BOOT_ENTRY_DELAY 100

#endif // APP_FEATURE_H_
