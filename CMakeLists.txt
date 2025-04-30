cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    dwarf-hutia
    VERSION 0.1.0
    DESCRIPTION "redis clown"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
include(cmake/grpc-gen.cmake)

# ---- GRPC ----

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

set(PROTO_FILES "proto/kvstore.proto")

grpc_gen_cpp(
    PROTO_FILES ${PROTO_FILES}
    PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto"
    GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated"
    TARGET_NAME "proto_gen"
)

# ---- Declare library ----
include_directories(
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${PROTO_GEN_DIR}
)

add_library(
    dwarf-hutia_lib OBJECT
    source/lib.cpp
    source/lib.hpp
    ${GENERATED_PROTO_SRCS}
    ${GENERATED_PROTO_HDRS}
)

add_dependencies(dwarf-hutia_lib proto_gen)

target_link_libraries(dwarf-hutia_lib PRIVATE
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    Threads::Threads
)

target_include_directories(
    dwarf-hutia_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(dwarf-hutia_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_subdirectory(source/core)
add_subdirectory(source/dwarf_cli)

add_executable(dwarf-hutia_exe
    source/main.cpp
)

add_dependencies(core_lib proto_gen)
add_dependencies(dwarf-hutia_exe proto_gen)

add_executable(dwarf-hutia::exe ALIAS dwarf-hutia_exe)

set_property(TARGET dwarf-hutia_exe PROPERTY OUTPUT_NAME dwarf-hutia)

target_compile_features(dwarf-hutia_exe PRIVATE cxx_std_20)

target_link_libraries(dwarf-hutia_exe PRIVATE
    dwarf-hutia_lib
    Threads::Threads
    core_lib
    network_lib
)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT dwarf-hutia_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of dwarf-hutia"
  )
endif()

include(cmake/dev-mode.cmake)
