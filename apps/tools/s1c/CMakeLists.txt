file(GLOB s1c_sources *.h *.cpp)
ADD_EXECUTABLE(s1c ${s1c_sources})
s1_get_boost_link_libs(_S1C_LINK_BOOST filesystem iostreams program_options)
target_link_libraries(s1c s1 ${_S1C_LINK_BOOST})
set_target_properties(s1c PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
# Require C++11 features if set globally
if(S1_REQUIRED_CXX_FEATURES)
  target_compile_features(s1c PRIVATE ${S1_REQUIRED_CXX_FEATURES})
endif()
