if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
  # Collect Boost.Preprocessor includes, concatenate to one big header, replacing BOOST_PP with _S1BOOSTPP
  add_custom_command(OUTPUT preprocessor.h
                     COMMAND ${CMAKE_SOURCE_DIR}/build/generate_preprocessor_h.sh ${CMAKE_CURRENT_SOURCE_DIR}/genObject/Object.h.template -I${CMAKE_CURRENT_SOURCE_DIR} -I${Boost_INCLUDE_DIR}
                              | sed -e 's/BOOST_PP/_S1BOOSTPP/g' -e 's/BOOST_PREPROCESSOR/_S1BOOSTPP/g' > preprocessor.h
                     DEPENDS genObject/Object.h.template)

  # Generate Object.h from Object.h.template:
  #  Replace BOOST_PP with _S1BOOSTPP
  #  Replace #includes with header generated in first step
  add_custom_command(OUTPUT Object.h
                     COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/genObject/Object.h.template | grep -v "^#include" | sed -e "s-//#include-#include-" | sed -e 's/BOOST_PP/_S1BOOSTPP/g' > Object.h
                     DEPENDS genObject/Object.h.template preprocessor.h)

  add_custom_target(generate_Object DEPENDS Object.h)
else()
  # Use pregenerated headers
  add_custom_command(OUTPUT preprocessor.h
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/pregenerated/preprocessor.h preprocessor.h
                     DEPENDS pregenerated/preprocessor.h)
  add_custom_command(OUTPUT Object.h
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/pregenerated/Object.h Object.h
                     DEPENDS pregenerated/Object.h)
  add_custom_target(generate_Object DEPENDS preprocessor.h Object.h
                    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/generate_Object")
endif()

add_custom_target(generate_ResultCode)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ResultCode_components.txt)
file(STRINGS ResultCode_components.txt RESULTCODE_COMPONENTS)
foreach(comp ${RESULTCODE_COMPONENTS})
  add_custom_command(OUTPUT ResultCode_defs_${comp}.h
                    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                        ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode_${comp}.xml
                        -r components
                        -t ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode_defs_comp.h.tpl > ResultCode_defs_${comp}.h
                    DEPENDS ResultCode_${comp}.xml ResultCode_defs_comp.h.tpl)
  add_custom_target(generate_ResultCode_${comp} DEPENDS ResultCode_defs_${comp}.h
                    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/generate_ResultCode_${comp}")
  add_dependencies(generate_ResultCode generate_ResultCode_${comp})
  list(APPEND RESULTCODE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode_${comp}.xml)
endforeach(comp)

add_custom_command(OUTPUT ResultCode_defs.h
                  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                      ${RESULTCODE_FILES}
                      -r components
                      -t ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode_defs.h.tpl > ResultCode_defs.h
                  DEPENDS ${RESULTCODE_FILES} ResultCode_defs.h.tpl)
add_custom_target(generate_ResultCode_defs DEPENDS ResultCode_defs.h
                  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/generate_ResultCode_defs")
add_dependencies(generate_ResultCode generate_ResultCode_defs)
