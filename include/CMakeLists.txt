add_subdirectory(s1)

if(NOT S1_PREBUILT)
  if(UNIX)
    # Collect Boost.Preprocessor includes, concatenate to one big header, replacing BOOST_PP with _S1BOOSTPP
    add_custom_command(OUTPUT s1/preprocessor.h
                      COMMAND ${CMAKE_SOURCE_DIR}/build/generate_preprocessor_h.sh ${CMAKE_CURRENT_SOURCE_DIR}/s1_genObject/Object.h.template -I${CMAKE_CURRENT_SOURCE_DIR} -I${Boost_INCLUDE_DIR}
                                | sed -e 's/BOOST_PP/_S1BOOSTPP/g' -e 's/BOOST_PREPROCESSOR/_S1BOOSTPP/g' > s1/preprocessor.h
                      DEPENDS s1_genObject/Object.h.template)

    # Generate Object.h from Object.h.template:
    #  Replace BOOST_PP with _S1BOOSTPP
    #  Replace #includes with header generated in first step
    add_custom_command(OUTPUT s1/Object.h
                      COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/s1_genObject/Object.h.template | grep -v "^#include" | sed -e "s-//#include-#include-" | sed -e 's/BOOST_PP/_S1BOOSTPP/g' > s1/Object.h
                      DEPENDS s1_genObject/Object.h.template s1/preprocessor.h)

    add_custom_target(generate_Object DEPENDS s1/Object.h)
  else()
    # Use pregenerated headers
    add_custom_command(OUTPUT s1/preprocessor.h
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/s1_pregenerated/preprocessor.h s1/preprocessor.h
                      DEPENDS s1_pregenerated/preprocessor.h)
    add_custom_command(OUTPUT s1/Object.h
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/s1_pregenerated/Object.h s1/Object.h
                      DEPENDS s1_pregenerated/Object.h)
    add_custom_target(generate_Object DEPENDS s1/preprocessor.h s1/Object.h
                      COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/generate_Object")
  endif()

  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/s1/Object.h"
            "${CMAKE_CURRENT_BINARY_DIR}/s1/preprocessor.h"
            "${CMAKE_CURRENT_BINARY_DIR}/s1/s1config.h"
          DESTINATION include/s1)

  add_library(s1_headers INTERFACE)
  target_include_directories(s1_headers INTERFACE
                             "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                             "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
                             $<INSTALL_INTERFACE:include>)
  add_custom_target(s1_headers_deps)
  add_dependencies(s1_headers_deps generate_Object generate_ResultCode)
  add_dependencies(s1_headers s1_headers_deps)
endif()

if(CXXTEST_FOUND AND NOT S1_PREBUILT)
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    # Disable some warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4800")
  endif()

  file(GLOB api_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  add_library(api_testlib STATIC ${api_testlib_sources})
  set_property(TARGET api_testlib PROPERTY COMPILE_DEFINITIONS S1_BUILD)
  set_property(TARGET api_testlib PROPERTY EXCLUDE_FROM_ALL YES)
  target_compile_features(api_testlib PUBLIC $<TARGET_PROPERTY:outcome::hl,INTERFACE_COMPILE_FEATURES>)
  target_include_directories(api_testlib PRIVATE ${CMAKE_SOURCE_DIR}/outcome/include)
  target_link_libraries(api_testlib s1_headers_internal)

  set(api_test_sources
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiByteStream.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiLibrary.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiLibraryObject.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiObject.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiResultCode.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiString.h)
  set(api_test_sources_11
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiByteStream_c++11.h)
  foreach(try_std default 98 11 14 17)
    # Provide ability to skip test with certain standards,
    # if necessary
    if(NOT S1_SKIP_TEST_${try_std})
      list(APPEND TEST_STD ${try_std})
    endif()
  endforeach()
  foreach(std ${TEST_STD})
    if(api_test_sources_${std})
      list(APPEND api_test_sources ${api_test_sources_${std}})
    endif()
    S1_CXXTEST_ADD_TEST(test_Api_${std} test_Api_${std}.cpp
                        ${api_test_sources})
    target_link_libraries(test_Api_${std} api_testlib s1)
    if(NOT ${std} STREQUAL "default")
      set_target_properties(test_Api_${std} PROPERTIES CXX_STANDARD ${std})
    endif()
    set_property(TARGET test_Api_${std} PROPERTY EXCLUDE_FROM_ALL YES)
    # Make sure S1 lib is found
    set_tests_properties(test_Api_${std} PROPERTIES ENVIRONMENT "PATH=$<TARGET_FILE_DIR:s1>;$ENV{PATH}")
    AutoBuildTest(test_Api_${std})
  endforeach()
endif()

if(NOT S1_PREBUILT)
  add_subdirectory(internal)

  add_library(s1_headers_internal INTERFACE)
  target_include_directories(s1_headers_internal INTERFACE
                             "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/internal>"
                             "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/internal>")
  target_link_libraries(s1_headers_internal INTERFACE s1_headers)
  add_custom_target(s1_headers_internal_deps)
  add_dependencies(s1_headers_internal_deps s1_headers generate_Diagnostics_fwd)
  add_dependencies(s1_headers_internal s1_headers_internal_deps)

  install(TARGETS s1_headers s1_headers_internal EXPORT s1_headers)

  install(DIRECTORY s1/ DESTINATION include/s1
          FILES_MATCHING PATTERN "*.h")
endif()
