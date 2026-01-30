//
// Created by luca on 30/01/26.
//
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char **argv) {
    // 1. Initialize GoogleMock (which also initializes GoogleTest)
    // This allows you to use command-line flags to filter tests
    ::testing::InitGoogleMock(&argc, argv);

    // 2. Run all tests defined in your test files
    return RUN_ALL_TESTS();
}