file(GLOB s1c_sources *.h *.cpp)
s1_get_boost_link_libs(_S1C_LINK_BOOST filesystem iostreams program_options)

# Detect function attribute syntax to use
set(funcattr_unused_candidates "[[maybe_unused]]" "__attribute__((unused))")
s1_detect_function_attribute(S1C_FUNCATTR_MAYBE_UNUSED_98
                             CXX_STANDARD 98
                             ${funcattr_unused_candidates})
s1_detect_function_attribute(S1C_FUNCATTR_MAYBE_UNUSED_11
                             CXX_STANDARD 11
                             ${funcattr_unused_candidates})

# Write configuration files
foreach(cxx_ver 98 11)
  set(S1C_FUNCATTR_MAYBE_UNUSED ${S1C_FUNCATTR_MAYBE_UNUSED_${cxx_ver}})
  set(dest_dir "${CMAKE_CURRENT_BINARY_DIR}/include_${cxx_ver}")
  file(MAKE_DIRECTORY ${dest_dir})
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/s1c-config.h.in"
                 "${dest_dir}/s1c-config.h")
endforeach()

# s1c built with C++98 to check API compatibility
ADD_EXECUTABLE(s1c_c++98 ${s1c_sources})
target_include_directories(s1c_c++98 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include_98")
target_link_libraries(s1c_c++98 s1 ${_S1C_LINK_BOOST})
# Require C++11 features if set globally
if(S1_REQUIRED_CXX_FEATURES)
  target_compile_features(s1c_c++98 PRIVATE ${S1_REQUIRED_CXX_FEATURES})
else()
  set_target_properties(s1c_c++98 PROPERTIES CXX_STANDARD 98)
endif()

# s1c variant we're 'publishing'. Allow it to utilize C++11 features
ADD_EXECUTABLE(s1c ${s1c_sources})
s1_strip_debug_symbols(s1c INSTALL_DESTINATION bin)
target_include_directories(s1c PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include_11")
target_link_libraries(s1c s1 ${_S1C_LINK_BOOST})
set_target_properties(s1c PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
# Require C++11 features if set globally
if(S1_REQUIRED_CXX_FEATURES)
  target_compile_features(s1c PRIVATE ${S1_REQUIRED_CXX_FEATURES})
else()
  set_target_properties(s1c PROPERTIES CXX_STANDARD 11)
endif()

install(TARGETS s1c
        EXPORT s1c
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
if(MSVC)
  install(FILES $<TARGET_PDB_FILE:s1c> DESTINATION bin OPTIONAL)
endif()
