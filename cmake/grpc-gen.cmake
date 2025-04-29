function(grpc_gen_cpp)
    set(options)
    set(oneValueArgs PROTO_DIR GEN_DIR TARGET_NAME)
    set(multiValueArgs PROTO_FILES)
    cmake_parse_arguments(GRPC_GEN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT GRPC_GEN_PROTO_DIR)
        set(GRPC_GEN_PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
    endif()

    if(NOT GRPC_GEN_GEN_DIR)
        set(GRPC_GEN_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
    endif()

    if(NOT GRPC_GEN_TARGET_NAME)
        set(GRPC_GEN_TARGET_NAME "proto_gen")
    endif()

    file(MAKE_DIRECTORY ${GRPC_GEN_GEN_DIR})

    if(NOT Protobuf_PROTOC_EXECUTABLE)
        message(FATAL_ERROR "protoc compiler not found")
    endif()

    if(NOT gRPC_CPP_PLUGIN_EXECUTABLE)
        find_program(GRPC_CPP_PLUGIN
            NAMES grpc_cpp_plugin
            PATHS /usr/local/bin /opt/homebrew/bin
        )
        if(GRPC_CPP_PLUGIN)
            set(gRPC_CPP_PLUGIN_EXECUTABLE ${GRPC_CPP_PLUGIN})
        else()
            message(FATAL_ERROR "grpc_cpp_plugin not found")
        endif()
    endif()

    set(GENERATED_SRCS "")
    set(GENERATED_HDRS "")

    foreach(PROTO_FILE ${GRPC_GEN_PROTO_FILES})
        get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
        get_filename_component(PROTO_ABS ${PROTO_FILE} ABSOLUTE)

        set(PB_SRC "${GRPC_GEN_GEN_DIR}/${PROTO_NAME}.pb.cc")
        set(PB_HDR "${GRPC_GEN_GEN_DIR}/${PROTO_NAME}.pb.h")
        set(GRPC_SRC "${GRPC_GEN_GEN_DIR}/${PROTO_NAME}.grpc.pb.cc")
        set(GRPC_HDR "${GRPC_GEN_GEN_DIR}/${PROTO_NAME}.grpc.pb.h")

        add_custom_command(
            OUTPUT "${PB_SRC}" "${PB_HDR}" "${GRPC_SRC}" "${GRPC_HDR}"
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
            ARGS
                --cpp_out "${GRPC_GEN_GEN_DIR}"
                --grpc_out "${GRPC_GEN_GEN_DIR}"
                --plugin=protoc-gen-grpc=${gRPC_CPP_PLUGIN_EXECUTABLE}
                -I "${GRPC_GEN_PROTO_DIR}"
                -I "${CMAKE_CURRENT_SOURCE_DIR}"
                "${PROTO_ABS}"
            DEPENDS "${PROTO_ABS}"
            COMMENT "Generating Protobuf and gRPC code for ${PROTO_NAME}.proto"
            VERBATIM
        )

        list(APPEND GENERATED_SRCS "${PB_SRC}" "${GRPC_SRC}")
        list(APPEND GENERATED_HDRS "${PB_HDR}" "${GRPC_HDR}")
    endforeach()

    add_custom_target(${GRPC_GEN_TARGET_NAME} ALL DEPENDS ${GENERATED_SRCS} ${GENERATED_HDRS})

    set(GENERATED_PROTO_SRCS ${GENERATED_SRCS} PARENT_SCOPE)
    set(GENERATED_PROTO_HDRS ${GENERATED_HDRS} PARENT_SCOPE)
    set(PROTO_GEN_DIR ${GRPC_GEN_GEN_DIR} PARENT_SCOPE)
endfunction()