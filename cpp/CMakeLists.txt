cmake_minimum_required(VERSION 3.14)
project(GildedRose)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

set(GOLDEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/golden" CACHE PATH "Golden master expected output directory")

# Library
add_library(gilded_rose src/GildedRose.cpp)
target_include_directories(gilded_rose PUBLIC include)

add_library(texttest_simulator src/TexttestSimulator.cpp)
target_link_libraries(texttest_simulator PUBLIC gilded_rose)
target_include_directories(texttest_simulator PUBLIC include)

# Texttest-compatible console fixture (optional manual / approval refresh)
add_executable(texttest_fixture src/TexttestFixture.cpp)
target_link_libraries(texttest_fixture PRIVATE texttest_simulator)

# Unit tests
enable_testing()
add_executable(gilded_rose_test test/GildedRoseTest.cpp)
target_link_libraries(gilded_rose_test gilded_rose GTest::gtest_main)

# Golden master regression test
add_executable(gilded_rose_golden_test test/GoldenMasterTest.cpp)
target_link_libraries(gilded_rose_golden_test PRIVATE texttest_simulator GTest::gtest_main)
target_compile_definitions(gilded_rose_golden_test PRIVATE GOLDEN_DIR="${GOLDEN_DIR}")

include(GoogleTest)
gtest_discover_tests(gilded_rose_test)
gtest_discover_tests(gilded_rose_golden_test)

# Regenerate approved output from the current implementation
add_custom_target(update-golden-master
    COMMAND ${CMAKE_COMMAND} -E env "UPDATE_GOLDEN=1" $<TARGET_FILE:gilded_rose_golden_test>
    DEPENDS gilded_rose_golden_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Writing ${GOLDEN_DIR}/texttest_31_days.expected.txt from current behavior"
)
