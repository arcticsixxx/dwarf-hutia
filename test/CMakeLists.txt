# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(dwarf-hutiaTests LANGUAGES CXX)

# ---- Tests ----

add_executable(dwarf-hutia_test source/dwarf-hutia_test.cpp)
# target_link_libraries(dwarf-hutia_test PRIVATE dwarf-hutia_lib)
target_compile_features(dwarf-hutia_test PRIVATE cxx_std_20)

add_test(NAME dwarf-hutia_test COMMAND dwarf-hutia_test)

# ---- End-of-file commands ----

add_folders(Test)
