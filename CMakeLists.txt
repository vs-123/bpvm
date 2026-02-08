cmake_minimum_required(VERSION 3.10)

project(bpvm)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE i_see_you)
set(FETCHCONTENT_QUIET OFF)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(raylib)

set(SRCS "src/main.c")

add_executable(bpvm ${SRCS})
target_compile_options(bpvm PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(bpvm PRIVATE raylib)
