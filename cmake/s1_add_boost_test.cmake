FUNCTION(s1_add_boost_test TARGET)
  CMAKE_PARSE_ARGUMENTS(S1_ADD_BOOST_TEST
    ""
    ""
    "SOURCES;DEPENDS"
    ${ARGN})
  set(test_module_path "${CMAKE_CURRENT_BINARY_DIR}/_${TARGET}_module.cpp")
  file(WRITE "${test_module_path}.tmp" "#define BOOST_TEST_MODULE ${TARGET}\n#include <boost/test/unit_test.hpp>\n")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${test_module_path}.tmp" "${test_module_path}")
  S1_GET_DEPENDENCIES(add_sources add_libs add_targets ${S1_ADD_BOOST_TEST_DEPENDS})
  add_executable(${TARGET} ${S1_ADD_BOOST_TEST_SOURCES} ${add_sources} "${test_module_path}")
  IF(add_libs)
    target_link_libraries(${TARGET} ${add_libs})
  ENDIF()
  s1_target_link_boost(${TARGET} unit_test_framework)
  if(add_targets)
    add_dependencies(${TARGET} ${add_targets})
  endif()
  foreach(dep ${S1_ADD_BOOST_TEST_DEPENDS})
    target_compile_features(${TARGET} PRIVATE $<TARGET_PROPERTY:${dep},INTERFACE_COMPILE_FEATURES>)
  endforeach()
  set_property(TARGET ${TARGET} PROPERTY EXCLUDE_FROM_ALL YES)
  add_test(NAME ${TARGET} COMMAND ${TARGET})
  AutoBuildTest(${TARGET})
ENDFUNCTION()
