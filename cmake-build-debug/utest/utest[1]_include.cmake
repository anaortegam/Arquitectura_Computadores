if(EXISTS "/home/anamaria/CLionProjects/fluid_program/cmake-build-debug/utest/utest[1]_tests.cmake")
  include("/home/anamaria/CLionProjects/fluid_program/cmake-build-debug/utest/utest[1]_tests.cmake")
else()
  add_test(utest_NOT_BUILT utest_NOT_BUILT)
endif()
