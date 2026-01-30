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
public:
    MOCK_METHOD(void, notifyError, (const std::string& message), (override));
    MOCK_METHOD(void, notifyMessage, (const std::string& status), (override));
};

// now I write the test base class, that uses the mocks to create a kiosk
class KioskCoordsTest : public ::testing::Test
{
public:
    CommandQueue queue;
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
TEST_F(KioskCoordsTest, ReturnsNullOnIvalidFormat)
{
    auto coordinates = kiosk->generateCoordinates("Z99");
    EXPECT_FALSE(coordinates.has_value());

    coordinates = kiosk->generateCoordinates("99");
    EXPECT_FALSE(coordinates.has_value());

    coordinates = kiosk->generateCoordinates("9");
    EXPECT_FALSE(coordinates.has_value());

    coordinates = kiosk->generateCoordinates("0Z");
    EXPECT_FALSE(coordinates.has_value());
}

TEST_F(KioskCoordsTest, ReturnsNullOnOutOfGridValues)
{
    auto coordinates = kiosk->generateCoordinates("Z9");
    EXPECT_FALSE(coordinates.has_value());

    coordinates = kiosk->generateCoordinates("F5");
    EXPECT_FALSE(coordinates.has_value());

    coordinates = kiosk->generateCoordinates("A6");
    EXPECT_FALSE(coordinates.has_value());
}

TEST_F(KioskCoordsTest, ReturnsnCoordinatesOnClosestCell)
{
    auto coordinates = kiosk->generateCoordinates("A1");
    EXPECT_EQ(coordinates->x, 1);
    EXPECT_EQ(coordinates->y, 1);
}

TEST_F(KioskCoordsTest, ReturnsnCoordinatesOnFurtherCell)
{
    auto coordinates = kiosk->generateCoordinates("E5");
    EXPECT_EQ(coordinates->x, 5);
    EXPECT_EQ(coordinates->y, 5);
}


TEST_F(KioskCoordsTest, ReturnsnCoordinatesOnAnyMiddleCell)
{
    auto coordinates = kiosk->generateCoordinates("C2");
    EXPECT_EQ(coordinates->x, 3);
    EXPECT_EQ(coordinates->y, 2);
}
