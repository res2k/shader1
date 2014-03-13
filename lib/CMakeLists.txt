add_definitions(-DS1_BUILD)
if(MSVC)
  # Disable some warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4800")

  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
     if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
     endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif()

add_subdirectory(base)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(intermediate)
add_subdirectory(codegen)
add_subdirectory(splitter)
add_subdirectory(optimize)
add_subdirectory(compiler)
add_subdirectory(s1)
