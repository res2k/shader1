# Supply a version script to hide all symbols but our own
CxxCompilerFlagString (CXX_LINK_HIDE_OTHER_SYMBOLS "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/ld-symbols.map")

add_custom_command(OUTPUT ResultCode.cpp
                   COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                      ${CMAKE_SOURCE_DIR}/include/s1/ResultCode.xml
                      ${CMAKE_CURRENT_SOURCE_DIR}/ResultCode.cpp.tpl > ResultCode.cpp
                   DEPENDS ${CMAKE_SOURCE_DIR}/include/s1/ResultCode.xml ResultCode.cpp.tpl)

set(s1_sources
    Backend.cpp
    BackendOptions.cpp
    CompiledProgram.cpp
    Library.cpp
    LibraryObject.cpp
    Object.cpp
    Options.cpp
    Program.cpp
    ResultCode.cpp
    ResultCodeHelper.cpp
    ResultCodeHelper.h
    ../../include/s1/Backend.h
    ../../include/s1/BackendOptions.h
    ../../include/s1/CompiledProgram.h
    ../../include/s1/Library.h
    ../../include/s1/LibraryObject.h
    ../../include/s1/Options.h
    ../../include/s1/Program.h
    ../../include/s1/ResultCode.h
    ../../include/s1/genObject/Object.h.template
    )
set_source_files_properties(${CMAKE_BINARY_DIR}/include/s1/Object.h PROPERTIES GENERATED YES)
include_directories(.)
if(BUILD_SHARED_LIBS)
  s1_add_library(s1 SHARED
                 SOURCES ${s1_sources}
                 DEPENDS generate_Object generate_ResultCode
                 LINK base compiler)
  set_property(TARGET s1 APPEND PROPERTY LINK_FLAGS "${CXX_STATIC_LIBSTDCXX_FLAG} ${CXX_LINK_HIDE_OTHER_SYMBOLS}")
else()
  s1_add_library(s1 STATIC 
                 SOURCES ${s1_sources}
                 DEPENDS generate_Object generate_ResultCode
                 LINK base compiler)
endif()
set(_ARCH "")
if(CMAKE_GENERATOR_PLATFORM)
  set(_ARCH "${CMAKE_GENERATOR_PLATFORM}")
elseif (MSVC AND (${CMAKE_GENERATOR} MATCHES "Win64"))
  set(_ARCH "x64")
endif()
if(_ARCH)
  set_target_properties(s1 PROPERTIES OUTPUT_NAME "s1-${_ARCH}")
endif()
set_target_properties(s1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set_target_properties(s1 PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
if(MSVC)
  set_target_properties(s1 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif()

