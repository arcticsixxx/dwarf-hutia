# CMake generated Testfile for 
# Source directory: /Users/efimdenisov/Documents/dwarf-hutia/test
# Build directory: /Users/efimdenisov/Documents/dwarf-hutia/build/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(dwarf-hutia_test "/Users/efimdenisov/Documents/dwarf-hutia/build/test/Release/dwarf-hutia_test")
  set_tests_properties(dwarf-hutia_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/efimdenisov/Documents/dwarf-hutia/test/CMakeLists.txt;13;add_test;/Users/efimdenisov/Documents/dwarf-hutia/test/CMakeLists.txt;0;")
else()
  add_test(dwarf-hutia_test NOT_AVAILABLE)
endif()
