Speed measurement
=================

Noise
-----

.. req-sw:: Noise rejection
    :id: REQ_SW_SPEED_NOISE_REJECTION

    Each channel shall be monitored and based on the input pulse lengths noise shall be rejected.

.. req-sw:: Speed unreliable
    :id: REQ_SW_SPEED_UNRELIABLE

    Each channel shall be monitored and when the speed is higher than ``X rps`` and the speed fluctuates
    by more than ``10%`` between each pulse then the ``CHANNELx_ERROR`` flag shall be set to ``Unreliable``.

.. req-sw:: Acceleration plausability
    :id: REQ_SW_OSH_MAX_ACCELERATION

    Each channel shall be monitored and when the speed changes by more than ``X rps/s`` then the
    ``CHANNELx_ERROR`` flag shall be set to ``Unreliable``.

.. req-sw:: Clearing speed unreliable flag
    :id: REQ_SW_SPEED_UNRELIABLE_CLEAR

    The ``Unreliable`` value can cleared once the speed value has stabilized to within ``2.5%`` range
    and the speed is below ``X rps`` for ``4s``.

Braking & Lockup
----------------

.. req-sw:: Brake signal
    :id: REQ_SW_SPEED_BRAKE

    When the speed changes by more than ``X rps/s`` then the ``CHANNELx_BRAKE`` flag shall be set to
    ``Braking``.

.. req-sw:: Clearing brake signal
    :id: REQ_SW_SPEED_BRAKE_CLEAR

    After ``Braking`` has been set the flag can be cleared once the ``X rps/s`` condition is no 
    longer and ``1s`` has been elapsed since the flag was set.

.. req-sw:: Lockup signal
    :id: REQ_SW_SPEED_LOCKUP

    When the speed is above ``X rps`` and then it goes below ``Y rps`` in for the next ``30ms`` then
    the lockup flag shall be set.

.. req-sw:: Clearing lockup signal
    :id: REQ_SW_SPEED_LOCKUP_CLEAR

    After lockup signal has been set the flag can be cleared after ``500ms``.
