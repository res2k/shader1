if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
  # Collect Boost.Preprocessor includes, concatenate to one big header, replacing BOOST_PP with _S1BOOSTPP
  add_custom_command(OUTPUT preprocessor.h
                     COMMAND ${CMAKE_SOURCE_DIR}/build/generate_preprocessor_h.sh ${CMAKE_CURRENT_SOURCE_DIR}/genObject/Object.h.template -I${CMAKE_CURRENT_SOURCE_DIR}
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

add_custom_command(OUTPUT ResultCode_defs.h
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                      ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode.xml
                      ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode_defs.h.tpl > ResultCode_defs.h
                   DEPENDS ResultCode.xml ResultCode_defs.h.tpl)
add_custom_target(generate_ResultCode DEPENDS ResultCode_defs.h
                  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/generate_ResultCode")
