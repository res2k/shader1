file(GLOB s1c_purec_sources *.h *.c)
ADD_EXECUTABLE(s1c_purec ${s1c_purec_sources})
target_link_libraries(s1c_purec s1)
setup_example(s1c_purec)
