Output Stage requirements
=========================

.. req-sys:: Startup time
    :id: REQ_SYS_OSH_STARTUP_TIME
    :status: draft

    The output stage shall be turned on ``100ms`` after a Power-on-Reset, but no later than ``1000ms``.

.. req-sys:: Short and open circuit protection
    :id: REQ_SYS_OSH_SHORT_OPEN_PROTECT
    :status: draft

    The device shall protect itself and the rest of the network against shorted and open sensors.

.. req-sys:: Short and open circuit detection
    :id: REQ_SYS_OSH_SHORT_OPEN_DETECT
    :status: draft

    The device shall be able to detect short and open circuit conditions on the sensor output stages.
    The device shall report these errors back over the communication line.
