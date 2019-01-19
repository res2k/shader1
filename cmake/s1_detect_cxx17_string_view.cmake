# Check whether std::string_view is available
function(s1_detect_string_view VAR)
  # Use cached result
  if(DEFINED "${VAR}")
    return()
  endif()

  message(STATUS "Performing Test ${VAR}")
  try_compile(${VAR}
    ${CMAKE_BINARY_DIR}
    SOURCES ${CMAKE_SOURCE_DIR}/config-tests/string_view.cpp
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)
  set(${VAR} "${${VAR}}" CACHE INTERNAL "Test ${VAR}")
  set(${VAR} "${${VAR}}" PARENT_SCOPE)
  message(STATUS "Performing Test ${VAR} - ${${VAR}}")
endfunction()
