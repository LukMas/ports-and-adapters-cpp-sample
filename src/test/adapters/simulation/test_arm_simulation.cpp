//
// Created by luca on 30/01/26.
//

#include <gtest/gtest.h>

#include "Kiosk.h"
#include "gmock/gmock-function-mocker.h"
#include "simulation/SimulatedArm.h"


class MockQueue : public ICommandQueue
{
    MOCK_METHOD(void, push, (KioskCommand cmd), (override));
    MOCK_METHOD(std::optional<KioskCommand>, pop, (), (override));
};

// now I write the test base class, that uses the mocks to create a kiosk
class SimulatedArmTest : public ::testing::Test
{
public:
    MockQueue queue;

protected:
    void SetUp() override
    {
    }
};


// testing
TEST_F(SimulatedArmTest, MovementOnX_Axis)
{
    std::unique_ptr<SimulatedArm> simulated_arm = std::make_unique<SimulatedArm>();
    simulated_arm->setDestination(Coordinate(5.0f, 0.0f));

    EXPECT_EQ(static_cast<int>(std::round(simulated_arm->getCurrentPosition().x)), 0);

    while (!simulated_arm->hasReachedTarget())
    {
        simulated_arm->move();
    }

    EXPECT_EQ(static_cast<int>(std::round(simulated_arm->getCurrentPosition().x)), 5);
}


// testing
TEST_F(SimulatedArmTest, MovementOnY_Axis)
{
    std::unique_ptr<SimulatedArm> simulated_arm = std::make_unique<SimulatedArm>();
    simulated_arm->setDestination(Coordinate(0.0f, 5.0f));

    EXPECT_EQ(static_cast<int>(std::round(simulated_arm->getCurrentPosition().y)), 0);

    while (!simulated_arm->hasReachedTarget())
    {
        simulated_arm->move();
    }

    EXPECT_EQ(static_cast<int>(std::round(simulated_arm->getCurrentPosition().y)), 5);
}
