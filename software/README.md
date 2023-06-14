## Feature list

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
