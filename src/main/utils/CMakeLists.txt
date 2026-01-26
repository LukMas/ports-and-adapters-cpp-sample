# Ensure headers in the same folder or 'include' folder are found
include_directories(.)

add_library(utils_lib INTERFACE) # INTERFACE because it's likely header-only
target_include_directories(utils_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})