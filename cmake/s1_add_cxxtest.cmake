FUNCTION(s1_add_cxxtest TARGET)
  CMAKE_PARSE_ARGUMENTS(S1_ADD_CXXTEST
    ""
    ""
    "SOURCES;DEPENDS"
    ${ARGN})
  S1_GET_DEPENDENCIES(add_sources add_libs add_targets ${S1_ADD_CXXTEST_DEPENDS})
  CXXTEST_ADD_TEST(${TARGET} ${TARGET}.cpp
                   ${S1_ADD_CXXTEST_SOURCES})
  IF(add_sources)
    ADD_LIBRARY(${TARGET}_dep STATIC ${add_sources})
    set_property(TARGET ${TARGET}_dep PROPERTY EXCLUDE_FROM_ALL YES)
    target_link_libraries(${TARGET} ${TARGET}_dep)
  ENDIF()
  IF(add_libs)
    target_link_libraries(${TARGET} ${add_libs})
  ENDIF()
  if(add_targets)
    add_dependencies(${TARGET} ${add_targets})
  endif()
  set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 14)
  set_property(TARGET ${TARGET} PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(${TARGET})
ENDFUNCTION()
