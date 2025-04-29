cmake_minimum_required(VERSION 3.14)

add_library(
    core_lib OBJECT
    keyvaluestore.h
    keyvaluestore.cpp
    cli.h
    cli.cpp
    istorage.h
)

target_link_libraries(core_lib PRIVATE)

target_include_directories(
  core_lib ${warning_guard}
  PUBLIC
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/core>"
)

target_compile_features(core_lib PUBLIC cxx_std_20)

add_subdirectory(network)

