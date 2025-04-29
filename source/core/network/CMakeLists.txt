cmake_minimum_required(VERSION 3.14)

add_library(
    network_lib OBJECT
    keyvalueclient.cpp
    keyvalueclient.h
    keyvalueservice.h keyvalueservice.cpp
    server.h server.cpp
)

target_link_libraries(network_lib PRIVATE)

target_include_directories(
  network_lib ${warning_guard}
  PUBLIC
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/core/network>"
) ## TODO includes

target_compile_features(network_lib PUBLIC cxx_std_20)

