#include "rotor_sensor.capnp.h"
#include <kj/debug.h>
#include <capnp/ez-rpc.h>
#include <capnp/message.h>
#include <iostream>
#include <queue>

extern "C" {

    #include "app/altitude.h"
    #include "bmp5.h"
    #include "app/config.h"
    #include "app/init.h"
    #include "common/swtimer.h"
    #include "bsp/osh_phy.h"
    #include "bsp/sensor.h"
    #include "bsp/bmp581.h"
    #include "bsp/bmi088.h"
    #include "bmi08.h"
    #include "bmi08_defs.h"
    #include "bmi08x.h"
    #include "common/scheduler.h"

    #include "app/comm.h"
    #include "app/diagnostics.h"

    #include "hal/tcc.h"
    #include "hal/nvic.h"
    #include "bsp/board.h"

    #include "app/speed.h"
    #include "app/ride.h"
}

CONFIG_Properties_t CONFIG_Props;

// Pressure and temperature values for simulation
static uint64_t SimulatedPressure = 101300;  // Simulated pressure in Pa (101300 Pa = 1013 hPa = 1 atm)
static int16_t SimulatedTemperature = 20;

int8_t bmp5_get_sensor_data(struct bmp5_sensor_data *sensor_data,
    const struct bmp5_osr_odr_press_config *osr_odr_press_cfg,
    struct bmp5_dev *dev)
{
    sensor_data->pressure = SimulatedPressure * 100;
    sensor_data->temperature = SimulatedTemperature * 100; 

    return BMP5_OK;  // Return success
}

void BMP581_Initialize(void)
{

}

// Accelerometer and gyroscope simulation
struct bmi08_dev BMI088_Device;

void BMI088_Initialize(void)
{

}

int8_t bmi08xa_init(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful initialization
}

int8_t bmi08g_init(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful initialization
}

int8_t bmi08a_load_config_file(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful loading of configuration file
}

int8_t bmi08a_set_power_mode(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful setting of power mode
}

int8_t bmi08xa_set_meas_conf(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful setting of measurement configuration
}

int8_t bmi08g_set_power_mode(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful setting of power mode
}

int8_t bmi08g_set_meas_conf(struct bmi08_dev *dev)
{
    return BMI08_OK;  // Simulate successful setting of measurement configuration
}

int8_t bmi08a_get_data(struct bmi08_sensor_data *accel, struct bmi08_dev *dev)
{
    // Simulate accelerometer data
    // TODO: connect
    accel->x = 0;  // Simulated X-axis acceleration
    accel->y = 0;  // Simulated Y-axis acceleration
    accel->z = 0;  // Simulated Z-axis acceleration

    return BMI08_OK;  // Return success
}

int8_t bmi08g_get_data(struct bmi08_sensor_data *gyro, struct bmi08_dev *dev)
{
    // Simulate gyroscope data
    // TODO: connect
    gyro->x = 0;  // Simulated X-axis angular velocity
    gyro->y = 0;  // Simulated Y-axis angular velocity
    gyro->z = 0;  // Simulated Z-axis angular velocity

    return BMI08_OK;  // Return success
}

// Speed sensor simulation
osh_phy_channel_state_t osh_channel_states[3] = {osh_phy_channel_state_ok, osh_phy_channel_state_ok, osh_phy_channel_state_ok};
bool osh_channel_enabled[3] = {false, false, false};

void OSH_PhyInit(void)
{

}

void OSH_PhyTurnOn(void)
{
    osh_channel_enabled[0] = true;
}

void OSH_PhyTurnOff(void)
{
    osh_channel_enabled[0] = false;
}

void OSH_PhyUpdate(void)
{

}

osh_phy_channel_state_t OSH_PhyGetChannelState(uint8_t channel)
{
    if (osh_channel_enabled[channel]) {
        return osh_channel_states[channel];
    } else {
        return osh_phy_channel_state_off;
    }
}

void SENSOR_Initialize(void) {

}

osh_sensor_state_t SENSOR_GetState(uint8_t channel) {
    return osh_sensor_state_ok;
}

std::queue<osh_sensor_sample_t> front_samples;

uint8_t SENSOR_HasData(uint8_t channel)
{
    if (front_samples.empty()) {
        return 0;
    } else {
        return 1;
    }
}

osh_sensor_sample_t SENSOR_GetSample(uint8_t channel) {
    if (front_samples.empty()) {
        return {.pos=0, .period=0};
    } else {
        auto sample = front_samples.front();
        front_samples.pop();
        return sample;
    }
}

/******************************************************************************/
// Low level hardware initialization functions

void BSP_ClockInitialize(void) {

}

void TCC_Reset(uint8_t timer) {
    // Reset the timer/counter
}

void TCC_SetupTrigger(uint8_t timer, uint32_t period)
{

}

void TCC_Enable(uint8_t timer)
{

}

void NVIC_Initialize(void)
{

}
/******************************************************************************/

static volatile uint8_t task1ms_flag = 0;
static volatile uint8_t task10ms_flag = 0;
static volatile uint8_t task100ms_flag = 0;
static volatile uint8_t task500ms_flag = 0;
static volatile uint8_t task1000ms_flag = 0;

void SCH_Init(void) {
    task1ms_flag = 0;
    task10ms_flag = 0;
    task100ms_flag = 0;
    task500ms_flag = 0;
    task1000ms_flag = 0;
}

extern "C" void SCH_DummyTask(void) {
    /** Do nothing */
}

void SCH_Task1ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task10ms_A(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task10ms_B(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task100ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task500ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Task1000ms(void) __attribute__ ((weak, alias("SCH_DummyTask")));

void SCH_Trigger()
{
    task1ms_flag = 1;

    if (task1ms_flag) {
        SCH_Task1ms();
        task1ms_flag = 0;
        task10ms_flag++;
    }
    if (task10ms_flag >= 10) {
        SCH_Task10ms_A();
        task10ms_flag = 0;
        task100ms_flag++;
    }
    if (task100ms_flag >= 10) {
        SCH_Task100ms();
        task100ms_flag = 0;
        task500ms_flag++;
    }
    if (task500ms_flag >= 5) {
        SCH_Task500ms();
        task500ms_flag = 0;
        task1000ms_flag++;
    }
    if (task1000ms_flag >= 2) {
        SCH_Task1000ms();
        task1000ms_flag = 0;
    }
}

/******************************************************************************/

void COMM_UpdatePhy(void) {
    // Simulate PHY update
}

void COMM_UpdateFastSignals(void) {
    // Simulate fast signals update
}

void COMM_UpdateSlowSignals(void) {
    // Simulate slow signals update
}

void COMM_UpdateDebugSignals(void) {
    // Simulate debug signals update
}

void COMM_Initialize(void) {
    // Simulate communication initialization
}

void DIAG_Initialize(void) {
    // Simulate diagnostics initialization
}

void CONFIG_ReloadUdsProperties(void) {
    // Simulate reloading UDS properties
}

bool DIAG_BootRequest(void) {
    return false;
}

void DIAG_Update(void) {
    // Simulate diagnostics update
}

/******************************************************************************/

class RotorSensorImpl final: public RotorSensor::Server {
  // Implementation of the Calculator Cap'n Proto interface.

public:

    kj::Promise<void> initialize(InitializeContext context) override {
        // Simulate initialization.
        auto params = context.getParams();
        auto results = context.getResults();

        CONFIG_Props.Altitude_Enabled = true;
        CONFIG_Props.Altitude_QNH = 1013;  // Set QNH to standard atmospheric pressure.

        CONFIG_Props.FrontWheel_Circumference = 2275;
        CONFIG_Props.RearWheel_Circumference = 2275;
        CONFIG_Props.FrontWheel_PoleCount = 1;
        CONFIG_Props.RearWheel_PoleCount = 1;

        CONFIG_Props.Ride_AutoStart = true;
        CONFIG_Props.Ride_AutoPause = true;
        CONFIG_Props.Ride_AutoResume = true;
        CONFIG_Props.Ride_IdleSpeed = 25;
        CONFIG_Props.Ride_IdleTimeout = 2;
        CONFIG_Props.Ride_PauseTimeout = 120;
        CONFIG_Props.Ride_ResumeTimeout = 5;

        APP_Initialize();

        return kj::READY_NOW;
    }

    kj::Promise<void> onTick(OnTickContext context) override {
        // Simulate a tick event.
        auto params = context.getParams();
        
        for (int i = 0; i < params.getMillis(); i++) {
            // Simulate some processing.
            SCH_Trigger();
        }
        return kj::READY_NOW;
    }

    kj::Promise<void> setPressure(SetPressureContext context) override {
        // Simulate setting the pressure.
        auto params = context.getParams();
        auto pressure = params.getPressure();

        SimulatedPressure = pressure;  // Store the simulated pressure.

        return kj::READY_NOW;
    }

    kj::Promise<void> getAltitude(GetAltitudeContext context) override {
        // Simulate an altitude reading.
        auto params = context.getParams();
        auto results = context.getResults();
        results.setAltitude(ALT_GetAltitude());  // Simulated altitude in meters.
        return kj::READY_NOW;
    }

    kj::Promise<void> setSensorState(SetSensorStateContext context) override {
        // Simulate setting the sensor state.
        auto params = context.getParams();
        auto channel = params.getChannel();
        auto state = params.getState();

        // Simulate setting the sensor state based on the input.
        if (state == RotorSensor::SpeedSensorState::OK) {
            osh_channel_states[channel] = osh_phy_channel_state_ok;
        } else if (state == RotorSensor::SpeedSensorState::SHORT) {
            osh_channel_states[channel] = osh_phy_channel_state_short;
        } else if (state == RotorSensor::SpeedSensorState::OPEN) {
            osh_channel_states[channel] = osh_phy_channel_state_open;
        }

        return kj::READY_NOW;
    }

    kj::Promise<void> queueSpeedData(QueueSpeedDataContext context) override {
        // Simulate queuing speed data.
        auto params = context.getParams();
        auto channel = params.getChannel();
        auto pos = params.getPos();
        auto period = params.getPeriod();

        // Simulate adding the sample to the queue.
        front_samples.push({.pos = pos, .period = period});

        return kj::READY_NOW;
    }

    kj::Promise<void> getGlobalSpeed(GetGlobalSpeedContext context) override {
        // Simulate getting global speed data.
        auto params = context.getParams();
        auto results = context.getResults();

        results.setSpeed(SPEED_GetSpeed());

        return kj::READY_NOW;
    }

    kj::Promise<void> getTopSpeed(GetTopSpeedContext context) override {
        // Simulate getting the top speed.
        auto params = context.getParams();
        auto results = context.getResults();

        results.setSpeed(RIDE_GetTopSpeed());

        return kj::READY_NOW;
    }
};

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " ADDRESS[:PORT]\n"
        "Runs the server bound to the given address/port.\n"
        "ADDRESS may be '*' to bind to all local addresses.\n"
        ":PORT may be omitted to choose a port automatically." << std::endl;
    return 1;
  }

  // Set up a server.
  capnp::EzRpcServer server(kj::heap<RotorSensorImpl>(), argv[1]);

  // Write the port number to stdout, in case it was chosen automatically.
  auto& waitScope = server.getWaitScope();
  unsigned int port = server.getPort().wait(waitScope);
  if (port == 0) {
    // The address format "unix:/path/to/socket" opens a unix domain socket,
    // in which case the port will be zero.
    std::cout << "Listening on Unix socket..." << std::endl;
  } else {
    std::cout << "Listening on port " << port << "..." << std::endl;
  }

  // Run forever, accepting connections and handling requests.
  kj::NEVER_DONE.wait(waitScope);
}