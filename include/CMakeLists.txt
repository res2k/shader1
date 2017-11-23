add_subdirectory(s1)

if(CXXTEST_FOUND)
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    # Disable some warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4800")
  endif()

  include_directories(${CMAKE_SOURCE_DIR}/boost.outcome/include)

  file(GLOB api_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  add_library(api_testlib STATIC ${api_testlib_sources})
  add_dependencies(api_testlib generate_Object)
  set_property(TARGET api_testlib PROPERTY CXX_STANDARD 14)
  set_property(TARGET api_testlib PROPERTY COMPILE_DEFINITIONS S1_BUILD)
  set_property(TARGET api_testlib PROPERTY EXCLUDE_FROM_ALL YES)
  target_include_directories(api_testlib PRIVATE ${CMAKE_SOURCE_DIR}/outcome/include)

  set(api_test_sources
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiByteStream.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiLibrary.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiLibraryObject.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiObject.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiResultCode.h
       ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiString.h)
  CXXTEST_ADD_TEST(test_Api test_Api.cpp
                   ${api_test_sources})
  target_link_libraries(test_Api api_testlib s1)
  set_property(TARGET test_Api PROPERTY CXX_STANDARD 11)
  set_property(TARGET test_Api PROPERTY EXCLUDE_FROM_ALL YES)
  # Make sure S1 lib is found
  set_property(TARGET test_Api PROPERTY ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
  AutoBuildTest(test_Api)
endif()
