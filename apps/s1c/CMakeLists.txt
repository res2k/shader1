file(GLOB s1c_sources *.h *.cpp)
ADD_EXECUTABLE(s1c ${s1c_sources})
target_link_libraries(s1c codegen)
