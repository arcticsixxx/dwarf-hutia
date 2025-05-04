cmake_minimum_required(VERSION 3.14)

add_library(
    core_lib OBJECT

    keyvaluestore.h keyvaluestore.cpp
    cli.h cli.cpp
    istorage.h
    server.h server.cpp
    keyvalueservice.h keyvalueservice.cpp
    keyvalueclient.h keyvalueclient.cpp
    keyvaluestore.h keyvaluestore.cpp
    persistancemanager.h persistancemanager.cpp

    utils/variant.h
    utils/protohelpers.h
    node.h node.cpp
    replicaservice.h
    replicaservice.cpp
)

target_link_libraries(core_lib PRIVATE
    kvstore_grpc_lib
    nlohmann_json::nlohmann_json
)

target_include_directories(
  core_lib ${warning_guard}
  PUBLIC
  "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source/core>"
)

target_compile_features(core_lib PUBLIC cxx_std_20)

