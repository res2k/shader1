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
add_library(s1_api STATIC ${s1_sources})
add_dependencies(s1_api generate_Object)
target_link_libraries(s1_api base compiler)
MERGE_LIBRARIES(s1 s1_api NOINSTALL)
set_target_properties(s1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set_target_properties(s1 PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")