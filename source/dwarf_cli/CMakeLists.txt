cmake_minimum_required(VERSION 3.14)

add_executable(dwarf_cli
    main.cpp
    Application.h
    Application.cpp
)

set_property(TARGET dwarf_cli PROPERTY OUTPUT_NAME dwarf_cli)

target_compile_features(dwarf_cli PRIVATE cxx_std_20)

target_link_libraries(dwarf_cli PRIVATE
    core_lib
)
