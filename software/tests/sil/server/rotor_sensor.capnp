@0xabcdefabcdefabcdef;

interface RotorSensor {
    initialize @0 () -> ();
    onTick @1 (millis: Int32 = 1) -> ();

    # Pressure sensor emulation
    setPressure @2 (pressure: UInt64);
    getAltitude @3 () -> (altitude: Int16); # TODO: remove
 
    # Speed sensor emulation
    enum SpeedSensorState {
        ok @0;
        short @1;
        open @2;
    }
    setSensorState @4 (channel: UInt8, state: SpeedSensorState);
    queueSpeedData @5 (channel: UInt8, pos: UInt32, period: UInt32) -> ();

    getGlobalSpeed @6 () -> (speed: Int16);
    getTopSpeed @7 () -> (speed: Int16);
}
