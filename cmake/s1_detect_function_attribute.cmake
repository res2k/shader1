# Detect which variant of a C++ function attribute is supported
function(s1_detect_function_attribute VAR)
  # Use cached result
  if(DEFINED "${VAR}")
    return()
  endif()

  CMAKE_PARSE_ARGUMENTS(dfa
    ""
    "CXX_STANDARD"
    ""
    ${ARGN})
  message(STATUS "Performing Test ${VAR}")
  set(try_compile_flags)
  if(${dfa_CXX_STANDARD})
    list(APPEND try_compile_flags CXX_STANDARD ${dfa_CXX_STANDARD})
  endif()
  foreach(attr_variant ${dfa_UNPARSED_ARGUMENTS})
    string(MAKE_C_IDENTIFIER "${attr_variant}" attr_variant_c)
    set(test_src_name "src_${VAR}_${attr_variant_c}.cxx")
    set(SOURCE "${attr_variant} void unused() {}\n\nint main() { return 0; }")
    file(WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/${test_src_name}" "${SOURCE}\n")
    try_compile(try_compile_result
      ${CMAKE_BINARY_DIR}
      ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/${test_src_name}
      ${try_compile_flags}
      OUTPUT_VARIABLE OUTPUT)
    if(${try_compile_result})
      set(${VAR} "${attr_variant}" CACHE INTERNAL "Test ${VAR}")
      set(${VAR} "${attr_variant}" PARENT_SCOPE)
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "s1_detect_function_attribute(${VAR}) for ${attr_variant} succeeded with the following output:\n"
        "${OUTPUT}\n"
        "Source file was:\n${SOURCE}\n")
      message(STATUS "Performing Test ${VAR} - ${attr_variant}")
      return()
    else()
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
        "s1_detect_function_attribute(${VAR}) for ${attr_variant} failed with the following output:\n"
        "${OUTPUT}\n"
        "Source file was:\n${SOURCE}\n")
    endif()
  endforeach()
  set(${VAR} "" CACHE INTERNAL "Test ${VAR}")
  set(${VAR} "" PARENT_SCOPE)
  message(STATUS "Performing Test ${VAR} - failed")
endfunction()
