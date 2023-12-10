Signals
=======

Speed measurement
-----------------

.. req-sw:: Speed signal
    :id: REQ_SW_SIGNAL_SPEED

    The speed signal shall be in the following value range:

    - 0: speed is zero
    - 1-65535: speed in pulse per seconds

.. req-sw:: Speed state
    :id: REQ_SW_SIGNAL_GLOBAL_SPEED_STATE
    
    - Ok (0)
    - Slow response
    - Unreliable 1
    - Error

.. req-sw:: Channel speed state
    :id: REQ_SW_SIGNAL_CHANNEL_SPEED_STATE

    The speed state shall have the following values:

    - Ok (0) - when the output channel is fine and a valid speed signal is available
    - 
    - Sensor warning (2) - when the speed sensor reports back a measurement warning (airgap)
    - Speed unreliable (3) - when the speed signal is inconsistent
    - Sensor error (4) - when the speed sensor reports back a measurement error or no data is available
    - Output open (5) - when the output channel has no sensor connected
    - Output shorted/error (6) - when the output channel is shorted or generally has a physical error
    - Off (7) - when the corresponding output stage is off

    The values here also denote priority, the highest being the output turned off, the idea being
    that lower values correspond to a usable speed signal while high ones above sensor error mean
    that the speed is incorrect.

.. req-sw:: Wheel slip signal
    :id: REQ_SW_SIGNAL_WHEEL_SLIP

    The wheel slip signal shall have the following values:
    
    - Not active (0) - when the wheel isn't s
    - Slip (1) - when the wheel is detected to be slipping

.. req-sw:: Wheel lockup signal
    :id: REQ_SW_SIGNAL_WHEEL_LOCKUP

    The wheel slip signal shall have the following values:
    
    - Not active (0) - when the wheel isn't spinning
    - Lockup (1) - when the wheel is detected to be locked up

.. req-sw:: Brake signal
    :id: REQ_SW_SIGNAL_BRAKE

    The brake signal shall have the following values:

    - Not active (0)
    - Braking (1)

iTPMS
-----

.. req-sw:: iTPMS signal
    :id: REQ_SW_SIGNAL_ITPMS_STATE

    The iTPMS signal shall have the following values:

    - Stopped (0): when the measurement has been stopped with no definite result
    - Running (1): when a measurement is in progress
    - Front pressure low (2): measurement has concluded, front tire is underinflated
    - Rear pressure low (3): measurement has concluded, rear tire is underinflated
    - Ok (4): measurement has concluded, tire pressures are good
    - Invalid (5-7): unused signal value range

Distance
--------

.. req-sw:: Distance signals
    :id: REQ_SW_SIGNAL_DISTANCE

    The distance signal shall be in the following range:

    - 0-2^32: number pulses recorded

Road quality
------------

.. req-sw:: Road quality signal
    :id: REQ_SW_SIGNAL_ROAD_QUALITY

    The road quality signal shall have the following values:

    - Not measured (0)
    - Flat (1)
    - Rough (2)
    - Very rough (3)
