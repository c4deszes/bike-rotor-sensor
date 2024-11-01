Sensor failure modes
====================

Speed sensor
------------

.. fmea:: Speed sensor shorted
    :id: FMEA_SYS_SPEED_SENSOR_SHORT
    :status: draft
    :probability: occasional
    :severity: critical
    :detection: high
    :mitigation: none

    The local effect is the lack of speed information, as long as the other speed sensor is working
    it's not a problem.

.. fmea:: Speed sensor cable open
    :id: FMEA_SYS_SPEED_SENSOR_OPEN
    :status: draft
    :probability: occasional
    :severity: critical
    :detection: high
    :mitigation: none

    The local effect is the lack of speed information, as long as the other speed sensor is working
    it's not a problem.

.. fmea:: Lack of magnetic field
    :id: FMEA_SYS_SPEED_SENSOR_NO_MAGNETIC_FIELD
    :status: draft
    :probability: occasional
    :severity: critical
    :detection: low
    :mitigation: checking inconsistencies between sensors

    Lack of magnetic field causes the speed sensors to timeout, the effect and detection depends
    on when the field was lost.

    * Starting from idle if the magnets are missing then the speed will remain at zero,
      this could be detected by the other sensors detecting a larger speed.

    * When lost at high speeds initially it will be detected as braking or wheel lockup, given
      enough time has passed we assume that the sensor information cannot be trusted.

Inertial measurement unit
-------------------------

.. fmea:: IMU failure
    :id: FMEA_SYS_IMU_FAILURE
    :status: draft
    :probability: occasional
    :severity: critical
    :detection: high
    :mitigation: none

    Lack of communication with the IMU chip is detected, we revert to wheel speed sensors in this
    case.

    Wrong IMU data is not detected unless the IMU directly reports the error, other than that we
    can monitor the global speed value and disable the inertial compensation.
