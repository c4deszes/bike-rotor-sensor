Measurement failures
====================

Speed
-----

.. fmea:: Wrong speed measurement
    :id: FMEA_SYS_WRONG_SPEED
    :status: draft
    :probability: occasional
    :severity: critical
    :detection: high
    :mitigation: increased sensor error and data error detection in hardware and software

    Offsets in the actual and measured speed is not an issue, however as a lot of the other signals
    are based on the speed measurement it's critical that bad data sources are isolated.

Distance
--------

.. fmea:: Distance measurement error
    :id: FMEA_SYS_WRONG_DISTANCE
    :status: draft
    :probability: occasional
    :severity: none
    :detection: low
    :mitigation: none

    Since the wheel circumferences cannot be measured exactly and will have slight variations during
    the ride there will always be a drift in the measurement. The drift should be minimized but the
    system will have other measurement sources.

Braking
-------

.. fmea:: Brake detection false negative
    :id: FMEA_SYS_BRAKING_FALSE_NEGATIVE
    :status: draft
    :probability: remote
    :severity: minor
    :detection: low
    :mitigation: none
    :relates: REQ_SYS_BRAKE_DETECTION

    Not detecting braking would cause brake lights to not turn on, but the maximum speeds of bikes
    generally cause drivers to be more cautious and it's only an issue when riding mid-lane.

.. fmea:: Brake detection false positive
    :id: FMEA_SYS_BRAKING_FALSE_POSITIVE
    :status: draft
    :probability: remote
    :severity: critical
    :detection: low
    :mitigation: software measures to detect invalid acceleration values
    :relates: REQ_SYS_BRAKE_DETECTION

    Detecting braking while not actually braking is crtical, this could cascade into rapid brakes
    by drivers behind.

    The software needs to detect accelerations that are inconsistent with the different sensors or
    even reality.

iTPMS
-----

.. fmea:: False assumption of low tire pressure
    :id: FMEA_SYS_ITPMS_FALSE_POSITIVE
    :status: draft
    :probability: occasional
    :severity: minor
    :detection: high
    :mitigation: none

    The preride checklist already includes checking tire pressure visually.

.. fmea:: False assumption of good tire pressures
    :id: FMEA_SYS_ITPMS_FALSE_NEGATIVE
    :status: draft
    :probability: occasional
    :severity: minor
    :detection: high
    :mitigation: none

    Low tire pressure will eventually be detected, just like deflated tires.

Altitude
--------

.. fmea:: Wrong altitude measurement
    :id: FMEA_SYS_WRONG_ALTITUDE
    :status: draft
    :probability: occasional
    :severity: minor
    :detection: low
    :mitigation: none

    Altitude measurement can be wrong for multiple reasons, failure of the pressure sensor, not
    setting up the QNH on the device or temperature drifting. However this functionality is not
    critical to the function of the device.
