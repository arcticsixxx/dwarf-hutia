install(
    TARGETS dwarf-hutia_exe
    RUNTIME COMPONENT dwarf-hutia_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
