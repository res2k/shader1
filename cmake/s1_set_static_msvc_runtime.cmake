function(s1_set_static_msvc_runtime)
  # Use static VC runtime when building an S1 DLL
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
     if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
     endif()
     set(${flag_var} "${${flag_var}}" PARENT_SCOPE)
  endforeach(flag_var)
  IF((MSVC_VERSION GREATER 1800) AND S1_UCRT_DYNAMIC)
    foreach(flag_var
            CMAKE_SHARED_LINKER_FLAGS_RELEASE
            CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
            CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)
      SET(${flag_var} "${${flag_var}} /nodefaultlib:libucrt.lib ucrt.lib" PARENT_SCOPE)
    endforeach()
    foreach(flag_var
            CMAKE_SHARED_LINKER_FLAGS_DEBUG)
      SET(${flag_var} "${${flag_var}} /nodefaultlib:libucrtd.lib ucrtd.lib" PARENT_SCOPE)
    endforeach()
  ENDIF()
endfunction()
