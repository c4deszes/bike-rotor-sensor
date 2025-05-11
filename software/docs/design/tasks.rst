Tasks
=====

Allocating functions to the right schedule is important for satisfying the requirements, such as
how fast braking is detected, but just generally making sure that there are reserves in the runtime
is also important.

1ms task
--------

- Communication handling
    - Responding to frame requests
    - Handling all incoming diagnostic calls
- Sensor control
    - Detecting sensor errors
- Speed data handling
    - Forwarding forward/rear wheel speed samples
- Distance data update
    - Incrementing distance based on wheel rotations
- Cadence data update
    - Updating crank period value
- Gear data update

10ms task
---------

- Speed data calculation
- IMU data handling
- Cadence data calculation
- ITPMS calculation
- System state transitions
- Communication signal updates (fast & debug)

100ms task
----------

- Pressure data handling
- Gear data calculation
- Voltage measurement
- Communication signal updates (slow)

1000ms task
-----------

- Ride status transitions
- Distance status transitions
- Current measurement
- Diagnostic signal updates
