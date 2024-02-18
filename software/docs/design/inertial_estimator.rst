Inertial estimator
==================

Responsibilities are:

* Monitoring IMU state
* Accumulating accelerations into speed
* Estimating ground inclination

State machine
-------------

During startup the estimator starts at zero speed.

While the IMU is operating correctly the sampled accelerometer values are integrated into the speed.

If the IMU reports error then the estimator's state is also set to error, if the accelerations are
beyond limits then it also goes into error state. Once the IMU error clears the speed shall be zero'd.

Questions
---------

* What samples do discard?
