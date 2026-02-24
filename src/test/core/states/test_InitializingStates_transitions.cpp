//
// Created by luca on 30/01/26.
//

#include <gtest/gtest.h>

#include "Kiosk.h"
#include "gmock/gmock-function-mocker.h"

// mock versions of the classes not relevant for this tests
class MockLogger : public ILogger
{
public:
    MOCK_METHOD(void, log, (const std::string& message), (override));
};

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
    MOCK_METHOD(std::optional<KioskCommand>, pop, (), (override));
};


class InitializingStatesTest : public ::testing::Test
{
public:
    MockQueue queue;
    MockLogger mock_logger;
    MockArm mock_arm;
    MockView mock_view;
    std::unique_ptr<Kiosk> kiosk;

protected:
    void SetUp() override
    {
        kiosk = std::make_unique<Kiosk>(queue, mock_view, mock_arm, mock_logger, 5, 5);
    }
};


// testing
TEST_F(InitializingStatesTest, InitializeToIdle)
{
    IKioskState& currentState = InitializingState::getInstance();
    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::READY));
        EXPECT_TRUE(typeid(nextState) == typeid(IdleState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::IDLE));
        EXPECT_TRUE(typeid(nextState) == typeid(IdleState));
    }
}

TEST_F(InitializingStatesTest, InitializeToInitialize)
{
    IKioskState& currentState = InitializingState::getInstance();

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::MOVE_TO));
        EXPECT_TRUE(typeid(nextState) == typeid(InitializingState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::SECURE_ITEM));
        EXPECT_TRUE(typeid(nextState) == typeid(InitializingState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::START));
        EXPECT_TRUE(typeid(nextState) == typeid(InitializingState));
    }

    {
        IKioskState& nextState = currentState.handleCommand(*kiosk, KioskCommand(CommandType::STOP));
        EXPECT_TRUE(typeid(nextState) == typeid(InitializingState));
    }
}
