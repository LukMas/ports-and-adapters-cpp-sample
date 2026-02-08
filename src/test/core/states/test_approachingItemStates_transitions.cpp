//
// Created by luca on 30/01/26.
//

#include <gtest/gtest.h>

#include "Kiosk.h"
#include "gmock/gmock-function-mocker.h"

// mock versions of the classes not relevant for this tests
class MockArm : public IArmPort
{
public:
    MOCK_METHOD(Coordinate, getCurrentPosition, (), (const override));
    MOCK_METHOD(bool, hasReachedTarget, (), (const override));
    MOCK_METHOD(void, setDestination, (const Coordinate& destination), (override));
};

class MockView : public IViewPort
{
    MOCK_METHOD(void, notifyMessage, (std::string status), (override));
    MOCK_METHOD(void, notifyError, (std::string message), (override));
};


class MockQueue : public ICommandQueue
{
    MOCK_METHOD(void, push, (KioskCommand cmd), (override));
    MOCK_METHOD(KioskCommand, pop, (), (override));
};


class ApproachingItemStatesTest : public ::testing::Test
{
public:
    MockQueue queue;
    MockArm mock_arm;
    MockView mock_view;
    std::unique_ptr<Kiosk> kiosk;

protected:
    void SetUp() override
    {
        kiosk = std::make_unique<Kiosk>(queue, mock_view, mock_arm, 5, 5);
    }
};


// testing
TEST_F(ApproachingItemStatesTest, ApproachingItemtoSecuringItem)
{
    IKioskState& currentState = ApproachingItemState::getInstance();
    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::SECURE_ITEM));
        // TODO: change to the correct state later
        EXPECT_TRUE(typeid(nextState) == typeid(IdleState));
    }
}

TEST_F(ApproachingItemStatesTest, InitializeToInitialize)
{
    IKioskState& currentState = ApproachingItemState::getInstance();

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::IDLE));
        EXPECT_TRUE(typeid(nextState) == typeid(ApproachingItemState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::READY));
        EXPECT_TRUE(typeid(nextState) == typeid(ApproachingItemState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::START));
        EXPECT_TRUE(typeid(nextState) == typeid(ApproachingItemState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::STOP));
        EXPECT_TRUE(typeid(nextState) == typeid(ApproachingItemState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::SECURE_ITEM));
        EXPECT_TRUE(typeid(nextState) == typeid(ApproachingItemState));
    }
}
