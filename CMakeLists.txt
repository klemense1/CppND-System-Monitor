cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

# Add the pugixml and GoogleTest library subdirectories
add_subdirectory(thirdparty/googletest)

# Add the testing executable
add_executable(test test/utest_linux_parser.cpp test/utest_process.cpp src/linux_parser.cpp src/process.cpp)

target_link_libraries(test 
    gtest_main 
)
