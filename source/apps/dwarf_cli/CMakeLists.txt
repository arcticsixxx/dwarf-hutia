cmake_minimum_required(VERSION 3.14)

add_executable(dwarf_cli
    main.cpp
    application.h
    application.cpp
)

set_property(TARGET dwarf_cli PROPERTY OUTPUT_NAME dwarf_cli)

target_compile_features(dwarf_cli PRIVATE cxx_std_20)

target_link_libraries(dwarf_cli PRIVATE
    kvstore_grpc_lib
    core_lib
)
