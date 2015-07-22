# Supply a version script to hide all symbols but our own
CxxCompilerFlagString (CXX_LINK_HIDE_OTHER_SYMBOLS "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/ld-symbols.map")

set(s1_sources
    Backend.cpp
    CompiledProgram.cpp
    Library.cpp
    LibraryObject.cpp
    Object.cpp
    Options.cpp
    Program.cpp
    ../../include/s1/Backend.h
    ../../include/s1/Library.h
    ../../include/s1/LibraryObject.h
    ../../include/s1/Options.h
    ../../include/s1/Program.h
    ../../include/s1/genObject/Object.h.template
    )
set_source_files_properties(${CMAKE_BINARY_DIR}/include/s1/Object.h PROPERTIES GENERATED YES)
if(BUILD_SHARED_LIBS)
  # Building shared: build lib straight away
  add_library(s1 SHARED ${s1_sources})
  add_dependencies(s1 generate_Object)
  target_link_libraries(s1 PRIVATE base compiler)
  set_property(TARGET s1 APPEND PROPERTY LINK_FLAGS "${CXX_STATIC_LIBSTDCXX_FLAG} ${CXX_LINK_HIDE_OTHER_SYMBOLS}")
else()
  # Building static lib: generate one big lib
  add_library(s1_api STATIC ${s1_sources})
  add_dependencies(s1_api generate_Object)
  target_link_libraries(s1_api base compiler)
  MERGE_LIBRARIES(s1 s1_api NOINSTALL)
endif()
if (MSVC AND (${CMAKE_GENERATOR} MATCHES  "Win64"))
  set_target_properties(s1 PROPERTIES OUTPUT_NAME "s1-x64")
endif()
set_target_properties(s1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set_target_properties(s1 PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
if(MSVC)
  set_target_properties(s1 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif()

