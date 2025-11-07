cmake_minimum_required(VERSION 3.20)

project(lab2
  LANGUAGES C CXX
)

# Стандарты
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(lab2_lib STATIC
  twelve.cpp
)
target_include_directories(lab2_lib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(lab2_exe
  main.cpp
)
target_link_libraries(lab2_exe PRIVATE
  lab2_lib
)

# ===== Тесты =====
enable_testing()

add_executable(lab2_tests
  tests/test_lab2.cpp
)
target_link_libraries(lab2_tests PRIVATE
  lab2_lib
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lab2_tests
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DISCOVERY_TIMEOUT 60
)

# (не обязательно)
# add_test(NAME MyProjectTests COMMAND lab2_tests)
