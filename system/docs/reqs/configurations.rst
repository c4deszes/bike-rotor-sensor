Configurations
==============

Device variants
---------------

.. req-sys:: Sensor support
    :id: REQ_SYS_SENSOR_SUPPORT
    :status: accepted

    The device shall support the following speed sensor types:

    * TLE5045: 2-wire iGMR sensor
    * TLE4942-1C: 2-wire hall effect sensor with direction detection

User configuration
------------------

.. req-sys:: External configuration
    :id: REQ_SYS_EXTERNAL_CONFIGURATION
    :status: draft

    The following settings shall be configurable externally:

    * Front/rear wheel circumferences and pole counts
    * Front/rear wheel sensor types
    * Drivetrain parameters (tooth count)
    * Ride status transition timings
    * Pressure sensor parameters (reference pressure)
    * ITPMS settings

Calibration
-----------

.. req-sys:: IMU calibration
    :id: REQ_SYS_IMU_CALIBRATION
    :status: draft

    The user shall be able to calibrate the intertial motion unit, the calibration takes place in
    the stand still level position and the device records the currently felt acceleration (gravity)
    and later uses that to determine the forward and sideways acceleration components.

    This configuration may be enhanced by an online calibration that tries to measure the gravity
    component.
