if(EXISTS "/home/anamaria/CLionProjects/fluid_program/cmake-build-debug/ftest/ftest[1]_tests.cmake")
  include("/home/anamaria/CLionProjects/fluid_program/cmake-build-debug/ftest/ftest[1]_tests.cmake")
else()
  add_test(ftest_NOT_BUILT ftest_NOT_BUILT)
endif()
