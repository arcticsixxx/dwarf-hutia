cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    dwarf-hutia
    VERSION 0.1.0
    DESCRIPTION "Redis clown."
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
    GEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated"
    TARGET_NAME "proto_gen"
)

add_library(
    kvstore_grpc_lib OBJECT
    ${GENERATED_PROTO_SRCS}
    ${GENERATED_PROTO_HDRS}
)

add_dependencies(kvstore_grpc_lib proto_gen)

target_link_libraries(kvstore_grpc_lib PRIVATE
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)

# PUBLIC: Headers needed by both the library and its users
# PRIVATE: Headers only needed by the library itself
# INTERFACE: Only adds to dependents' include paths (for header-only libraries)
# BUILD_INTERFACE specifies this path should only be included when:
# -The library is being built directly
# -Or when it's being used by another target in the same build system (same CMake project)
target_include_directories(
    kvstore_grpc_lib ${warning_guard}
    SYSTEM PUBLIC
   "\$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated>"
    ${PROTOBUF_INCLUDE_DIRS}
)

# ---- Third party libraries ----

set(NLOHMANN_JSON_SYSTEM_INCLUDE "")
add_subdirectory(third_party/njson)

# ---- Declare library ----

add_subdirectory(source)

# ---- Declare executable ----


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
