file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
  # Collect Boost.Preprocessor includes, concatenate to one big header, replacing BOOST_PP with _S1BOOSTPP
  add_custom_command(OUTPUT preprocessor.h
                     COMMAND ${CMAKE_SOURCE_DIR}/build/generate_preprocessor_h.sh ${CMAKE_CURRENT_SOURCE_DIR}/genObject/Object.h.template -I${CMAKE_CURRENT_SOURCE_DIR}
                              | sed -e 's/BOOST_PP/_S1BOOSTPP/g' -e 's/BOOST_PREPROCESSOR/_S1BOOSTPP/g' > preprocessor.h
                     DEPENDS genObject/Object.h.template)
  set_property(SOURCE preprocessor.h PROPERTY GENERATED YES)

  # Generate Object.h from Object.h.template:
  #  Replace BOOST_PP with _S1BOOSTPP
  #  Replace #includes with header generated in first step
  add_custom_command(OUTPUT Object.h
                     COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/genObject/Object.h.template | grep -v "^#include" | sed -e "s-//#include-#include-" | sed -e 's/BOOST_PP/_S1BOOSTPP/g' > Object.h
                     DEPENDS genObject/Object.h.template preprocessor.h)
  set_property(SOURCE Object.h PROPERTY GENERATED YES)

  add_custom_target(generate_Object DEPENDS Object.h)
else()
  # Use pregenerated headers
  file(INSTALL pregenerated/preprocessor.h pregenerated/Object.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  add_custom_target(generate_Object DEPENDS pregenerated/preprocessor.h pregenerated/Object.h)
endif()
