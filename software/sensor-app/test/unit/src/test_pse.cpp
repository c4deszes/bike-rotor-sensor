#include "gtest/gtest.h"
#include <tuple>
#include <vector>

extern "C" {
    #include "app/pse.h"
}

const pse_configuration conf = {
    .pulse_per_rotation = 64,
    .min_pulse_width = 10,
    .index_threshold = 50
};

class TestPositionEstimator : public testing::Test {
protected:
    void SetUp() override {

    }
};

TEST_F(TestPositionEstimator, Init) {
    pse_init(&conf);

    EXPECT_EQ(pse_get_position_state(), PSE_POSITION_STATE_UNKNOWN);
}

class TestPositionEstimatorSequences :
    public testing::TestWithParam<std::tuple<std::vector<uint16_t>, pse_position_state, uint16_t>> {

};

TEST_P(TestPositionEstimatorSequences, SequenceShouldEndInCorrectStateAndPosition) {
    pse_configuration conf = {
        .pulse_per_rotation = 8,
        .min_pulse_width = 5,
        .index_threshold = 50
    };
    pse_init(&conf);

    std::vector<uint16_t> sequence = std::get<0>(GetParam());
    pse_position_state expected_state = std::get<1>(GetParam());
    uint16_t expected_position = std::get<2>(GetParam());

    for (size_t i = 0; i < sequence.size(); i++) {
        pse_update(sequence[i]);
    }

    EXPECT_EQ(pse_get_position_state(), expected_state);

    if (expected_state == PSE_POSITION_STATE_OK) {
        EXPECT_EQ(pse_get_position(), expected_position);
    }
}

INSTANTIATE_TEST_CASE_P(
    ,TestPositionEstimatorSequences,
    ::testing::Values(
        // No pulses
        std::make_tuple(std::vector<uint16_t>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0}), PSE_POSITION_STATE_UNKNOWN, 0),

        // Standard pulses
        std::make_tuple(std::vector<uint16_t>({12, 12, 12, 12, 12, 12, 12, 12, 12, 12}), PSE_POSITION_STATE_UNKNOWN, 0),

        // Standard then index
        std::make_tuple(std::vector<uint16_t>({12, 12, 12, 12, 12, 100, 12, 12, 12, 12}), PSE_POSITION_STATE_OK, 4),

        // Standard then index
        std::make_tuple(std::vector<uint16_t>({12, 100, 12, 12, 12, 12, 12, 12, 12, 12}), PSE_POSITION_STATE_OK, 8),

        // Immediate index
        std::make_tuple(std::vector<uint16_t>({100, 12, 12, 12, 12, 12, 12, 12, 12, 12}), PSE_POSITION_STATE_UNKNOWN, 0),

        // Standard with noise
        std::make_tuple(std::vector<uint16_t>({12, 12, 0, 12, 12, 12, 12, 12, 12, 12}), PSE_POSITION_STATE_UNKNOWN, 0),

        // Standard with noise then index
        std::make_tuple(std::vector<uint16_t>({12, 12, 0, 12, 12, 100, 12, 12, 12, 12}), PSE_POSITION_STATE_OK, 4),

        // Standard with index then noise
        std::make_tuple(std::vector<uint16_t>({12, 12, 100, 12, 12, 12, 0, 12, 12, 12}), PSE_POSITION_STATE_OK, 6),

        // Standard with index then fast slow down
        std::make_tuple(std::vector<uint16_t>({12, 100, 12, 12, 12, 12, 100, 100, 100, 100}), PSE_POSITION_STATE_OK, 8),

        // Standard with index then slow down
        std::make_tuple(std::vector<uint16_t>({12, 100, 12, 12, 40, 70, 100, 130, 150, 200}), PSE_POSITION_STATE_OK, 8),

        // Standard with index then early index (backwards rotation)
        std::make_tuple(std::vector<uint16_t>({12, 100, 12, 12, 100, 12, 12, 12, 12, 12}), PSE_POSITION_STATE_UNKNOWN, 0),
    
        // Standard with index, early index then resynchronize
        std::make_tuple(std::vector<uint16_t>({12, 100, 12, 12, 100, 12, 12, 12, 12, 100, 12, 12}), PSE_POSITION_STATE_OK, 2)
    )
);
