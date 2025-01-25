## TODOs

- Speed measurement improvements
    - inclusion of IMU data
    - Filtering speed sensors
    - Speed sensor unreliable detection

- Gear detection?
- Distance measurement only when ride is active
- Crank sensor error detection
- IMU error detection
- Barometer error detection
- Altitude calculation (lookup tables)
- Cadence status
- ITPMS
- Ride logs
- State management
    - standby
    - shutdown

- Diagnostics
    - Ride end/Reset
    - Ride forced start
    - IMU calibration
    

## Speed measurement

If both sensors are OK
    

## Feature list

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

---------------------

Input stage
    Speed sensing
        Current speed
        Filtered speed

    Position sensing
        Current position
        Position validity

    Distance measurement
        Number of rotations
        Number of partial rotations

Signal RPC communication
    Data object assignment
    Manufacturing info
        Serial number
        Chip rev
    Sensor info
        Temperature
        Sensor errors
    Diag
        Output stage control
        Reset

Output stage
    Overcurrent protection

## Programming

pymcuprog -C -d attiny202 write -f .\build\user-avr-release\sensor-app\rotor-sensor-standalone.hex --verify -c 125k -v debug

pymcuprog -C -d attiny416 write -f .\build\user-avr-release\sensor-app\rotor-sensor-app-merged.hex

## Bootloader

https://github.com/microchip-pic-avr-examples/avr128da48-cnano-bootloader-mplabx/blob/master/avr128da48-cnano-avr-da-bootloader.X/boot.c

:02000004008278
:090000000000010000C40500002D
:0400000500000200F5
:00000001FF
