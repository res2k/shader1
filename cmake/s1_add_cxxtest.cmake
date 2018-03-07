# This is CXXTEST_ADD_TEST, modified to support generator expressions
macro(S1_CXXTEST_ADD_TEST _cxxtest_testname _cxxtest_outfname)
    set(_cxxtest_real_outfname ${CMAKE_CURRENT_BINARY_DIR}/${_cxxtest_outfname})

    add_custom_command(
        OUTPUT  ${_cxxtest_real_outfname}
        DEPENDS ${ARGN}
        COMMAND ${CXXTEST_TESTGEN_INTERPRETER}
        ${CXXTEST_TESTGEN_EXECUTABLE} ${CXXTEST_TESTGEN_ARGS} -o ${_cxxtest_real_outfname} ${ARGN}
    )

    set_source_files_properties(${_cxxtest_real_outfname} PROPERTIES GENERATED true)
    add_executable(${_cxxtest_testname} ${_cxxtest_real_outfname} ${ARGN})

    add_test(NAME ${_cxxtest_testname} COMMAND ${_cxxtest_testname})
endmacro()

FUNCTION(s1_add_cxxtest TARGET)
  CMAKE_PARSE_ARGUMENTS(S1_ADD_CXXTEST
    ""
    ""
    "SOURCES;DEPENDS"
    ${ARGN})
  S1_GET_DEPENDENCIES(add_sources add_libs add_targets ${S1_ADD_CXXTEST_DEPENDS})
  S1_CXXTEST_ADD_TEST(${TARGET} ${TARGET}.cpp
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
  foreach(dep ${S1_ADD_CXXTEST_DEPENDS})
    target_compile_features(${TARGET} PRIVATE $<TARGET_PROPERTY:${dep},INTERFACE_COMPILE_FEATURES>)
  endforeach()
  set_property(TARGET ${TARGET} PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(${TARGET})
ENDFUNCTION()
