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
    MOCK_METHOD(KioskCommand, pop, (), (override));
};

// now I write the test base class, that uses the mocks to create a kiosk
class SimulatedArmTest : public ::testing::Test
{
public:
    MockQueue queue;
    std::unique_ptr<SimulatedArm> simulated_arm;

protected:
    void SetUp() override
    {
        simulated_arm = std::make_unique<SimulatedArm>();
    }
};


// testing
TEST_F(SimulatedArmTest, MovementOnX_Axis)
{
    simulated_arm->setDestination(Coordinate(5, 0));

    EXPECT_EQ(simulated_arm->getCurrentPosition().x, 0);

    simulated_arm->move();
    simulated_arm->move();
    simulated_arm->move();
    simulated_arm->move();
    simulated_arm->move();

    EXPECT_EQ(simulated_arm->getCurrentPosition().x, 5);
}


// testing
TEST_F(SimulatedArmTest, MovementOnY_Axis)
{
    simulated_arm->setDestination(Coordinate(0, 5));

    EXPECT_EQ(simulated_arm->getCurrentPosition().x, 0);

    simulated_arm->move();
    simulated_arm->move();
    simulated_arm->move();
    simulated_arm->move();
    simulated_arm->move();

    EXPECT_EQ(simulated_arm->getCurrentPosition().y, 5);
}
