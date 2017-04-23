set(S1_EXAMPLES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/examples/${CMAKE_BUILD_TYPE}")

# Target to build all examples
add_custom_target(build_examples DEPENDS s1)

function(setup_example TARGET)
  set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${S1_EXAMPLES_OUTPUT_DIR}")
  add_dependencies(build_examples ${TARGET})

  # Copy s1 lib to S1_EXAMPLES_OUTPUT_DIR
  if(BUILD_SHARED_LIBS)
    if(CMAKE_BUILD_TYPE)
      set(_S1_EXAMPLES_LIB "${CMAKE_BINARY_DIR}/examples/${CMAKE_BUILD_TYPE}/$<TARGET_FILE_NAME:s1>")
    else()
      set(_S1_EXAMPLES_LIB "${CMAKE_BINARY_DIR}/examples/$<CONFIG>/$<TARGET_FILE_NAME:s1>")
    endif()
    ### Ideally this would be a add_custom_command() with OUTPUT ${_S1_EXAMPLES_LIB}
    ### ...but generator expressions are not supported by CMake, so tie to build_examples for now...
    add_custom_command(TARGET ${TARGET}
                       POST_BUILD
                       COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:s1>" "${_S1_EXAMPLES_LIB}")
  endif()
endfunction()

add_subdirectory(qml_shader)
add_subdirectory(s1c_purec)
