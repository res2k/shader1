# Strip debug symbols from a binary on Linux
function(s1_strip_debug_symbols TARGET)
  CMAKE_PARSE_ARGUMENTS(STRIP_DEBUG
    ""
    "INSTALL_DESTINATION"
    ""
    ${ARGN})
if(CMAKE_OBJCOPY)
    add_custom_command(TARGET ${TARGET}
                       POST_BUILD
                       COMMAND "${CMAKE_OBJCOPY}" --only-keep-debug "$<TARGET_FILE:${TARGET}>" "$<TARGET_FILE:${TARGET}>.dbg"
                       COMMAND "${CMAKE_OBJCOPY}" --strip-debug "$<TARGET_FILE:${TARGET}>"
                       COMMAND "${CMAKE_OBJCOPY}" "--add-gnu-debuglink=$<TARGET_FILE:${TARGET}>.dbg" "$<TARGET_FILE:${TARGET}>"
                       COMMENT "Stripping debug info from ${TARGET}")
    if(STRIP_DEBUG_INSTALL_DESTINATION)
      install(FILES "$<TARGET_FILE:${TARGET}>.dbg" DESTINATION ${STRIP_DEBUG_INSTALL_DESTINATION})
    endif()
  endif()
endfunction()
