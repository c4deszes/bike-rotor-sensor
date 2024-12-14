#if !defined(APP_CONFIG_H_)
#define APP_CONFIG_H_

/* Speed sensor configuration */

/* Channel allocation constants */
#define SPM_FRONT_SENSOR_CHANNEL 0
#define SPM_REAR_SENSOR_CHANNEL 1
#define SPM_CRANK_SENSOR_CHANNEL 2

#define SPM_SENSOR_STARTUP_TIME 1000

/* Speed sensor buffer size */
#define SPM_SENSOR_BUFFER_SIZE 64
/* Maximum pulse length from the speed sensor */
#define SPM_LOW_SPEED_CUTOFF_PERIOD_US 9000000      /* Maximum time to measure in a single pole configuration */
#define SPM_HIGH_SPEED_CUTOFF_PERIOD_US 2000        /* Minimum time to measure in a 30-pole configuration */
#define SPM_HIGH_SPEED_CUTOFF_DUTY_US 100

/*
 * Distance measurement configuration
 */

/*  */
#define DIST_MEASUREMENT_ERROR_TIMER 60000
/*  */
#define DIST_MEASUREMENT_DIFF_UP_THRESHOLD 100
#define DIST_MEASUREMENT_DIFF_DOWN_THRESHOLD 50

/*
 * Cadence sensor configuration
 */
#define CAD_LOW_SPEED_CUTOFF_US 6000000         /* Maximum time to measure in a single pole configuration (10 RPM)*/
#define CAD_HIGH_SPEED_CUTOFF_US 300000         /* Minimum time to measure in a single pole configuration (200 RPM)*/
#define CAD_NOISE_CUTOFF_DUTY_US 2000

#define CAD_COASTING_EXTRA_TIME 800

#define CAD_POSITION_MAX 15

/*
 * Gear configuration
 */

/*  */
#define GEAR_HOLD_TIME 1000
#define GEAR_MAX_GEARS 12

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
#define ALT_SENSOR_STARTUP_TIME 150

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
#define ITPMS_STARTUP_TIME 2000
#define ITPMS_MINIMUM_SPEED 100
#define ITPMS_SPEED_THRESHOLD 5

#endif // APP_CONFIG_H_
