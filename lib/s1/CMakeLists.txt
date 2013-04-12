set(s1_sources
    Library.cpp
    LibraryObject.cpp
    Object.cpp
    ../../include/s1/Library.h
    ../../include/s1/LibraryObject.h
    ${CMAKE_BINARY_DIR}/include/s1/Object.h
    )
set_source_files_properties(${CMAKE_BINARY_DIR}/include/s1/Object.h PROPERTIES GENERATED YES)
add_library(s1 ${s1_sources})
add_dependencies(s1 generate_Object)
target_link_libraries(s1 base compiler)
