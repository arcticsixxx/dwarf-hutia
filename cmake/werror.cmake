function(add_werror TARGET)
    set(CLANG_GCC_WARNING
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wcast-qual
        -Wformat=2
        -Wundef
        -Wshadow
        -Wcast-align
        -Wunused
        -Wnull-dereference
        -Wdouble-promotion
        -Wimplicit-fallthrough
        -Wextra-semi
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
    )

    set(APPLE_CLANG_WARNING
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wcast-qual
        -Wformat=2
        -Wundef
        -Werror=float-equal 
        -Wshadow 
        -Wcast-align 
        -Wunused
        -Wnull-dereference
        -Wdouble-promotion 
        -Wimplicit-fallthrough
        -Wextra-semi 
        -Woverloaded-virtual
        -Wnon-virtual-dtor 
        -Wold-style-cast
    )

    set(MSVC_WARNING
        /W4             
        /w14165        
        /w44242 /w44254 /w44263 /w34265 /w34287
        /w44296 /w44365 /w44388 /w44464
        /w14545 /w14546 /w14547 /w14549 /w14555
        /w34619 /w34640 /w24826 /w14905 /w14906 /w14928
        /w45038
    )

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")          
        target_compile_options(${TARGET} PRIVATE ${CLANG_GCC_WARNING})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")    
        if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            target_compile_options(${TARGET} PRIVATE ${APPLE_CLANG_WARNING})
        else()
            target_compile_options(${TARGET} PRIVATE ${CLANG_GCC_WARNING_FLAGS})
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")     
            target_compile_options(${TARGET} PRIVATE ${MSVC_WARNING})
    else()
        message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()

endfunction()