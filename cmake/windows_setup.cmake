if (WIN32)
    message(STATUS "Configuring Windows-specific defines")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64-bit Windows
        add_compile_definitions(
            WIN32_LEAN_AND_MEAN
            NOMINMAX
            _WIN32_WINNT=0x0601
        )
        add_compile_options(-m64)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        # 32-bit Windows
        add_compile_definitions(
            WIN32_LEAN_AND_MEAN
            NOMINMAX
            _WIN32_WINNT=0x0601
        )
        add_compile_options(-m32)
    else()
        message(FATAL_ERROR "Unsupported architecture detected!")
    endif()
endif()
