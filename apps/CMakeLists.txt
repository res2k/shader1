add_subdirectory(s1c)
# The pure C example doesn't build with a static libs1.
if(BUILD_SHARED_LIBS)
add_subdirectory(s1c_purec)
endif()
