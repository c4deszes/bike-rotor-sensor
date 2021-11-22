#include "gtest/gtest.h"
#include <tuple>
#include <vector>

extern "C" {
    #include "app/init.h"
}

class TestSensor : public testing::Test {
protected:
    void SetUp() override {

    }
};

TEST_F(TestSensor, CallMain) {
    app_main();

}
