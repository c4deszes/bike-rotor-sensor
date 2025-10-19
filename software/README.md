## TODOs

- Speed measurement improvements
    - inclusion of IMU data
    - Filtering speed sensors
    - Speed sensor unreliable detection

- Distance measurement only when ride is active
- Crank sensor error detection
- IMU error detection
- Barometer error detection
- ITPMS
- Ride logs
- State management
    - standby
    - shutdown

- Diagnostics
    - Ride end/Reset
    - Ride forced start
    - IMU calibration

## Task list

Messages:
    10ms - SpeedStatus
        Speed               connected, still under work     valid when speedstatus ok
        SpeedStatus         connected, still under work
        Braking             disabled, yet to implement      valid when speedstatus ok
        Slip                disabled, yet to implement      valid when speedstatus ok

    100ms - DrivetrainStatus
        Cadence             connected, still under work, valid when cadence status ok
        CadenceStatus
        GearEstimate        missing validity signal

    1s -  RideStatus
        Duration            no validity, not needed
        Distance            valid when distancestatus valid
        DistanceStatus
        Status (idle, moving, etc.)

    1s - RoadStatus
        Altitude            missing validity signal
        Grade               missing validity signal
        Quality
        ITPMS

    5s - RideStats
        TopSpeed
        AvgSpeed
        Elevation
        Descent
