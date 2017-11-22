function(s1_detect_stdlib OUTPUT)
    if(MSVC)
        # MSVC: Only has one C++ runtime
        set(${OUTPUT} "msvcrt" PARENT_SCOPE)
    else()
        try_compile(DETECT_LIBCPP
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR}/config-tests/detect-libc++.cpp)
        try_compile(DETECT_LIBSTDCPP
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR}/config-tests/detect-libstdc++.cpp)
        if(DETECT_LIBCPP)
            set(${OUTPUT} "libc++" PARENT_SCOPE)
        elseif(DETECT_LIBSTDCPP)
            set(${OUTPUT} "libstdc++" PARENT_SCOPE)
        else()
            set(${OUTPUT} "unknown" PARENT_SCOPE)
        endif()
    endif()
endfunction()
