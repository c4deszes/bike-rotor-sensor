#include "gtest/gtest.h"
#include "fff.h"

extern "C" {
    #include "app/altitude.h"
    #include "common/swtimer.h"

    #include "bmp5.h"
    #include "bmp5_defs.h"
}

DEFINE_FFF_GLOBALS;

struct bmp5_dev BMP581_Device = {

};

FAKE_VALUE_FUNC1(int8_t, bmp5_init, bmp5_dev*);
FAKE_VALUE_FUNC2(int8_t, bmp5_set_power_mode, bmp5_powermode, bmp5_dev*);
FAKE_VALUE_FUNC2(int8_t, bmp5_get_osr_odr_press_config, bmp5_osr_odr_press_config*, bmp5_dev*);
FAKE_VALUE_FUNC2(int8_t, bmp5_set_osr_odr_press_config, const bmp5_osr_odr_press_config*, bmp5_dev*);
FAKE_VALUE_FUNC2(int8_t, bmp5_set_iir_config, const bmp5_iir_config*, bmp5_dev*);
FAKE_VALUE_FUNC3(int8_t, bmp5_get_sensor_data, bmp5_sensor_data*, const bmp5_osr_odr_press_config*, bmp5_dev*);

FAKE_VALUE_FUNC0(swtimer_t*, SWTIMER_Create);
FAKE_VALUE_FUNC1(bool, SWTIMER_Elapsed, swtimer_t*);
FAKE_VOID_FUNC2(SWTIMER_Setup, swtimer_t*, uint16_t);

class AltitudeTest : public testing::Test {
    public:
    void SetUp() override {
        // Reset common FFF internal structures
        FFF_RESET_HISTORY();
    }
};

int8_t get_sensor_data(bmp5_sensor_data* sensor_data, const bmp5_osr_odr_press_config* osr_odr_press_cfg, bmp5_dev* dev) {
    sensor_data->pressure = 101480 * 100;
    sensor_data->temperature = 20 * 100;
    return BMP5_OK;
}

TEST_F(AltitudeTest, PressureToAltitude) {
    bmp5_get_sensor_data_fake.custom_fake = get_sensor_data;
    SWTIMER_Elapsed_fake.return_val = true;

    ALT_SetQNH(102900);
    ALT_Initialize();
    ALT_Update();   // Sets up the sensor

    EXPECT_EQ(bmp5_init_fake.call_count, 1);
    EXPECT_EQ(bmp5_get_sensor_data_fake.call_count, 1);
    EXPECT_EQ(ALT_GetPressure(), 101480);
    EXPECT_EQ(ALT_GetAltitude(), 100);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
