file(GLOB s1c_sources *.h *.cpp)
ADD_EXECUTABLE(s1c ${s1c_sources})
target_link_libraries(s1c s1)
set_target_properties(s1c PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
