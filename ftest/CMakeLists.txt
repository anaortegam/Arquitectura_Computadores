cmake_minimum_required(VERSION 3.26)# Executable for all unit tests with list of sources

add_executable(ftest
        funcional_test.cpp)
# Library dependencies
target_link_libraries (ftest
        PRIVATE
        sim
        GTest::gtest_main
        Microsoft.GSL::GSL)
# Discover all tests and add them to the test driver
include(GoogleTest)
gtest_discover_tests(ftest)
# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in sim
