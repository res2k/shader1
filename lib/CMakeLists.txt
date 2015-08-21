add_definitions(-DS1_BUILD)
if(MSVC)
  # Disable some warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4800")
endif()

if(MSVC AND S1_STATIC_CRT)
  # Use static VC runtime when building an S1 DLL
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
     if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
     endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
  IF((MSVC_VERSION GREATER 1800) AND S1_UCRT_DYNAMIC)
    foreach(flag_var
            CMAKE_SHARED_LINKER_FLAGS_RELEASE
            CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
            CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)
      SET(${flag_var} "${${flag_var}} /nodefaultlib:libucrt.lib ucrt.lib")
    endforeach(flag_var)
    foreach(flag_var
            CMAKE_SHARED_LINKER_FLAGS_DEBUG)
      SET(${flag_var} "${${flag_var}} /nodefaultlib:libucrtd.lib ucrtd.lib")
    endforeach(flag_var)
  ENDIF()
endif()
if(MSVC AND NOT BUILD_SHARED_LIBS)
  # Static lib merging doesn't merge debug info, so manually set up a single
  # PDB. For convenience, place it in the same dir the lib is put
  set(s1_outputname "$<TARGET_PROPERTY:s1,OUTPUT_NAME>")
  set(s1_logicalname "$<TARGET_PROPERTY:s1,NAME>")
  set(s1_actualoutput "$<$<BOOL:${s1_outputname}>:${s1_outputname}>$<$<NOT:$<BOOL:${s1_outputname}>>:${s1_logicalname}>")
  set(S1LIB_PDB "/Fd${CMAKE_BINARY_DIR}/\$(Configuration)/${s1_actualoutput}.pdb" ${CXX_PDB_SYNC})
endif()

# Build Boost libs we need manually (the build flags we need are kinda wacky).
set(WANT_BOOST_LIBS system)
find_package(Boost ${BOOST_VERSION} COMPONENTS ${WANT_BOOST_LIBS})
foreach(lib_name ${WANT_BOOST_LIBS})
  string(TOUPPER "${lib_name}" lib_upper)
  file(GLOB_RECURSE lib_sources ${BOOST_ROOT}/libs/${lib_name}/src/*.cpp)
  if(lib_sources)
    s1_add_library(s1boost_${lib_name} STATIC
                   SOURCES ${lib_sources})
    target_compile_definitions(s1boost_${lib_name} PUBLIC "-DBOOST_${lib_upper}_STATIC_LINK=1")
  elseif(${Boost_${lib_upper}_FOUND})
    # Add an interface library if we don't have sources
    add_library(s1boost_${lib_name} INTERFACE)
    target_link_libraries(s1boost_${lib_name} INTERFACE ${Boost_${lib_upper}_LIBRARY})
    target_compile_definitions(s1boost_${lib_name} INTERFACE "-DBOOST_${lib_upper}_STATIC_LINK=1")
  endif()
endforeach()

add_subdirectory(base)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(intermediate)
add_subdirectory(codegen)
add_subdirectory(splitter)
add_subdirectory(optimize)
add_subdirectory(compiler)
add_subdirectory(API)
