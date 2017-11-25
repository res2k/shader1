add_subdirectory(s1)

if(CXXTEST_FOUND)
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    # Disable some warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4800")
  endif()

  file(GLOB api_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  add_library(api_testlib STATIC ${api_testlib_sources})
  add_dependencies(api_testlib generate_Object)
  set_property(TARGET api_testlib PROPERTY COMPILE_DEFINITIONS S1_BUILD)
  set_property(TARGET api_testlib PROPERTY EXCLUDE_FROM_ALL YES)
  target_compile_features(api_testlib PUBLIC $<TARGET_PROPERTY:outcome::hl,INTERFACE_COMPILE_FEATURES>)
  target_include_directories(api_testlib PRIVATE ${CMAKE_SOURCE_DIR}/outcome/include)

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
    CXXTEST_ADD_TEST(test_Api_${std} test_Api_${std}.cpp
                    ${api_test_sources})
    target_link_libraries(test_Api_${std} api_testlib s1)
    if(NOT ${std} STREQUAL "default")
      set_target_properties(test_Api_${std} PROPERTIES CXX_STANDARD ${std})
    endif()
    set_property(TARGET test_Api_${std} PROPERTY EXCLUDE_FROM_ALL YES)
    # Make sure S1 lib is found
    set_property(TARGET test_Api_${std} PROPERTY ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    AutoBuildTest(test_Api_${std})
  endforeach()
endif()
