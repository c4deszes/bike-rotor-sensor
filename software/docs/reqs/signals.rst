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
    :id: REQ_SW_SIGNAL_SPEED_STATE

    The speed state shall have the following values:

    - Off (0) - when the corresponding output stage is off
    - Ok/Forward (1) - when the output channel is fine and a valid speed signal is available
    - 
    - Sensor warning (3) - when the speed sensor reports back a measurement warning (airgap)
    - Speed unreliable (4) - when the speed signal is inconsistent
    - Sensor error (5) - when the speed sensor reports back a measurement error or no data is available
    - Output shorted/error (6) - when the output channel is shorted or generally has a physical error
    - Output open (7) - when the output channel has no sensor connected

.. req-sw:: Wheel slip signal
    :id: REQ_SW_SIGNAL_WHEEL_SLIP

    The wheel slip signal shall have the following values:
    
    - Not active (0) - when the wheel isn't s
    - Slip (1) - when the wheel is detected to be slipping

.. req-sw:: Wheel lockup signal
    :id: REQ_SW_SIGNAL_WHEEL_LOCKUP

    The wheel slip signal shall have the following values:
    
    - Not active (0) - when the wheel isn't spinning
    - Slip (1) - when the wheel is detected to be locked up

.. req-sw:: Brake signal
    :id: REQ_SW_SIGNAL_BRAKE

    The brake signal shall have the following values:

    - Not active (0)
    - Braking (1)

iTPMS
-----

.. req-sw:: iTPMS signal
    :id: REQ_SW_SIGNAL_ITPMS_STATE

    The TPMS signal shall have the following values:

    - Stopped (0): 
    - Running = 1:
    - Front pressure low = 2:
    - Rear pressure low = 3:
    - Ok (4): 
    - Invalid (5-7): 

Distance
--------

.. req-sw:: Distance signals
    :id: REQ_SW_SIGNAL_DISTANCE

    The distance signal shall be in the following range:

    - 0-2^32: number pulses recorded

.. req-sw:: Distance mode
    :id: REQ_SW_SIGNAL_DISTANCE_MODE

    The distance mode signal shall have the following values:

    - Absolute (0)
    - Estimated (1)


Road quality
------------

.. req-sw:: Road quality signal
    :id: REQ_SW_SIGNAL_ROAD_QUALITY

    The road quality signal shall have the following values:

    - Not measured (0)
    - Flat (1)
    - Rough (2)
    - Very rough (3)
