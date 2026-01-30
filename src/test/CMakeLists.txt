# 1. Include the libraries from core
# (Assuming your src/main/core/CMakeLists.txt defines 'kiosk_logic_lib')

add_executable(kiosk_unit_tests
        test_main.cpp
        core/test_kiosk.cpp
)

# 2. Link against your logic and GTest
target_link_libraries(kiosk_unit_tests
        PRIVATE
        kiosk_logic_lib    # Your core logic
        kiosks_ports_adapters_lib
        utils_lib
        GTest::gtest_main
        GTest::gmock
)

# 3. Ensure headers are visible
target_include_directories(kiosk_unit_tests
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/main/core
        ${CMAKE_SOURCE_DIR}/src/main/utils
)

# This registers the tests with CLion's test runner
include(GoogleTest)
gtest_discover_tests(kiosk_unit_tests)