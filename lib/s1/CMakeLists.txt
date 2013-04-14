set(s1_sources
    Library.cpp
    LibraryObject.cpp
    Object.cpp
    Options.cpp
    ../../include/s1/Library.h
    ../../include/s1/LibraryObject.h
    ../../include/s1/Options.h
    ../../include/s1/genObject/Object.h.template
    )
set_source_files_properties(${CMAKE_BINARY_DIR}/include/s1/Object.h PROPERTIES GENERATED YES)
add_library(s1 ${s1_sources})
add_dependencies(s1 generate_Object)
target_link_libraries(s1 base compiler)
