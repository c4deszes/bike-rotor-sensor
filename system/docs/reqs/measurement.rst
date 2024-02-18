Measurement requirements
========================

Speed
-----

.. req-sys:: Speed range
    :id: REQ_SYS_SPEED_RANGE
    :status: accepted

    Each channel shall be able to measure speeds in the range of ``1km/h - 70km/h``, it should also
    have a cutoff speed where the device will report ``0 km/h`` or stand still.

.. req-sys:: Speed Resolution
    :id: REQ_SYS_SPEED_RESOLUTION
    :status: accepted

    Each channel shall be able to measure speeds with a resolution of:

    * ``0.1 km/h`` in the range of ``0-30km/h``
    * ``0.5 km/h`` in the range of ``30-70km/h``

.. req-sys:: Noise rejection
    :id: REQ_SYS_SPEED_NOISE_REJECTION
    :status: draft

    Speed data that falls outside the expected speeds or the range of possible new speeds shall
    be rejected. If this noise is severe the speed status may be set to ``Unreliable``.

.. req-sys:: Acceleration plausibility
    :id: REQ_SYS_ACCELERATION_PLAUSIBILITY
    :status: draft

    Accelerations that go beyond 

Distance
--------

.. req-sys:: Distance range
    :id: REQ_SYS_DISTANCE_RANGE
    :status: accepted

    The device shall be able to measure distances in the range of ``0km - 200km``. If the value is
    about to overflow it might be externally reset by the body computer. If an overflow occurs
    the corresponding signal shall be set.

.. req-sys:: Distance resolution
    :id: REQ_SYS_DISTANCE_RESOLUTION
    :status: accepted

    The device shall be able to measure distances with a resolution of:

    * ``1m`` in the range of ``0-5km``
    * ``100m`` in the range of ``5km-200km``

Braking
-------

.. req-sys:: Brake detection
    :id: REQ_SYS_BRAKE_DETECTION
    :status: draft
    :tags: safety

    Each channel shall monitor the acceleration, when the acceleration is below ``-6 m/s^2`` the
    corresponding brake signal shall be set.

    Once the acceleration value is above ``-2 m/s^2`` the corresponding brake signal shall be
    cleared.

    The rear wheel braking signal should always be prioritized, if it's indicating braking then
    assume the bike is braking as well. The inertial motion sensor should also be considered in the
    determination of braking condition.

    Ignore braking indication from sensors which have reported errors or warnings.

    *TODO: minimum reaction time calculation*

Lockup & Slip
-------------

.. req-sys:: Lockup detection
    :id: REQ_SYS_LOCKUP_DETECTION
    :status: draft

    *TBD*

.. req-sys:: Slip detection
    :id: REQ_SYS_SLIP_DETECTION
    :status: draft

    *TBD*

iTPMS
-----

.. req-sys:: iTPMS measurement
    :id: REQ_SYS_ITPMS_MEASUREMENT
    :status: draft

    When the speed of the wheels is within ``5%`` of each other the speed difference value shall
    contribute to the iTPMS measurement, the measurement shall be accepted after 10 minutes worth
    of samples have been collected.

    If the average difference between the wheel speeds from the samples is above 2% then the wheel
    that was faster on average should be marked as low pressure.

    The measurement shall be trigger-able externally, but the ECU may decide to run it on it's own.
