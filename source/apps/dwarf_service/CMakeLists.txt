cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0167 OLD)
find_package(Boost 1.88.0 COMPONENTS program_options REQUIRED)

add_executable(dwarf-hutia_exe
    main.cpp
    parser.h
    parser.cpp
)

add_executable(dwarf-hutia::exe ALIAS dwarf-hutia_exe)

set_property(TARGET dwarf-hutia_exe PROPERTY OUTPUT_NAME dwarf-hutia)

target_compile_features(dwarf-hutia_exe PRIVATE cxx_std_20)

target_link_libraries(dwarf-hutia_exe PRIVATE
    core_lib
    kvstore_grpc_lib
    Boost::program_options
)
