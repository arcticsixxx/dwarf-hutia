cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    dwarf-hutia
    VERSION 0.1.0
    DESCRIPTION "redis clone "
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

set(ZLIB_ROOT "/opt/homebrew/opt/zlib")
set(ZLIB_INCLUDE_DIR "${ZLIB_ROOT}/include")
set(ZLIB_LIBRARY "${ZLIB_ROOT}/lib/libz.dylib")

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)


# set(PROTO_FILE proto/example.proto)

# Generate protobuf and gRPC files
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})
# grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_FILE})
include_directories(
    ${PROTOBUF_INCLUDE_DIRS}
    ${GRPC_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

add_library(
    dwarf-hutia_lib OBJECT
    source/lib.cpp
    source/lib.hpp
   source/example.grpc.pb.cc
   source/example.pb.h
   source/example.grpc.pb.h
   source/example.pb.cc
)

target_link_libraries(dwarf-hutia_lib PRIVATE
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)

target_include_directories(
    dwarf-hutia_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(dwarf-hutia_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_subdirectory(source/core)

add_executable(client
    source/client.cpp
   source/example.grpc.pb.cc
   source/example.pb.h
   source/example.grpc.pb.h
   source/example.pb.cc
)

target_link_libraries(client
    PRIVATE
    gRPC::grpc++
    protobuf::libprotobuf
)

add_executable(dwarf-hutia_exe source/main.cpp)
add_executable(dwarf-hutia::exe ALIAS dwarf-hutia_exe)

set_property(TARGET dwarf-hutia_exe PROPERTY OUTPUT_NAME dwarf-hutia)

target_compile_features(dwarf-hutia_exe PRIVATE cxx_std_20)

target_link_libraries(dwarf-hutia_exe PRIVATE dwarf-hutia_lib core_lib)

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
