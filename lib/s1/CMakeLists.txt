set(s1_sources
    Library.cpp
    LibraryObject.cpp
    Object.cpp
    ../../include/s1/Library.h
    ../../include/s1/LibraryObject.h
    ../../include/s1/Object.h
    )
add_library(s1 ${s1_sources})
target_link_libraries(s1 base)
