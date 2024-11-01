Speed monitoring
================

Responsibilities are:

* Measuring the individual wheel speeds
* Calculating global speed based on the wheel speeds
* Detecting issues with the speed value

Absolute measurement
--------------------

The absolute speed is determined using the magnetic/optical wheel speed sensor, it's a very
accurate way of measuring speed but in a single pole configuration the update rate is quite low,
could be as low as ``500 ms``.

Inertial measurement
--------------------

The onboard accelerometer and gyro is used to provide an estimated velocity, since it's integrated
over time there's quite a lot of noise.

The speed is zero'd when:

* the absolute speed value is received
* the accelerations and rotations sensed are below a threshold for 10 seconds

States
------

Questions
~~~~~~~~~

* What about the case of double sensor error but IMU is still good?
    * Enter unreliable state, channel states should still be error

* What about the case when sensor speeds are mismatched?
    
    * Case 1: front and rear difference (beyond tire pressure variance)

        * If both sensors report ok, there's no noise in the data then global state should be unreliable
        * If either reports unreliability, etc. then follow the procedure above

    * Case 2: IMU estimate is way off the speed sensors

        * Should trust the speed sensors more, maybe zeroing helps in this case but that should
          happen regularly anyway if we have speed data.
          There could be a case where the sensors timeout (no pulse is received) but the IMU detects
          movement.

Estimation algorithm
--------------------

When both absolute and inertial measurements are available then estimate the speed using the
following process:

When the speed sensors fails (e.g.: output channel error or measurement error) then use the inertial
estimate and set the given channel's speed status to unreliable.

When the inertial measurement unit fails then the absolute measurement should use rolling decrements,
which involves decreasing the calculated speed when new samples are not received within a window
that has the same length as the last valid sample. This method will have quite a low response time
in cases of wheel lockup or maximum braking, therefore the channel's speed status is set to slow response.

When both sensors fail then set the speed status to sensor error.

Global speed calculation
------------------------


