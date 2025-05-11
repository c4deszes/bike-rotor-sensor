
After POR or wakeup from Shutdown the ECU shall initialize into it's normal operating mode,
that means turning on all speed sensors, the pressure sensor and the IMU.

During operation the ECU may change it's op status, according to the following logic:

* when in the bootloader, the status may be boot or boot_error
* before the speed sensors are enabled it may be init
* if the sensor states are normal then it may be ok
* may be warn if the following occurs:
    * unable to determine cadence
    * unable to determine altitude
    * unable to detect braking
    * unable to determine front or rear speeds (but not both)
* may be error if the following occurs:
    * unable to determine front and rear speeds

If the ECU receives no command for some time:
* it shall remain in the same state as before, this ensures that an interrupt in the communication
  won't lead to the loss of data

If the ECU receives an IDLE request or the supply voltage drops below 10V it shall:
* transition to idle mode

in IDLE:
* turn off all speed sensors
* enter the lowest current consumption mode in the MCU but one that can still poll the IMU
* keep the IMU on
* save the ride data and end the current ride
* keep listening for messages on the bus
* if the IMU detects significant movement then the ECU may send the wakeup request on the bus, but
  only if no traffic was seen for some time
* if the ECU receives a wakeup message, or any status commands then it shall transition back into
  normal mode (all sensors on, etc.)

If the ECU receives a shutdown request or the supply voltage drops below 5.5V? it shall:
* turn off all speed sensors
* save the ride data
* go to sleep
