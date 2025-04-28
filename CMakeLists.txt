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

add_library(
    dwarf-hutia_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    dwarf-hutia_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(dwarf-hutia_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(dwarf-hutia_exe source/main.cpp)
add_executable(dwarf-hutia::exe ALIAS dwarf-hutia_exe)

set_property(TARGET dwarf-hutia_exe PROPERTY OUTPUT_NAME dwarf-hutia)

target_compile_features(dwarf-hutia_exe PRIVATE cxx_std_20)

target_link_libraries(dwarf-hutia_exe PRIVATE dwarf-hutia_lib)

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
