# We have unusual requirements for Boost (requiring it to be built with
# different build flags in different places), so we wrap it's usage.

# List of all the Boost libs we use anywhere in the source tree
set(_S1_BOOST_USED_LIBS filesystem iostreams program_options system)

macro(s1_find_boost VERSION)
  if(ARGN)
    message(FATAL_ERROR "Too many arguments to s1_find_boost")
  endif()
  # Fail if Boost is not found
  find_package(Boost ${VERSION} REQUIRED)
  # But it's okay if we don't have the libs
  find_package(Boost ${VERSION} COMPONENTS ${_S1_BOOST_USED_LIBS})
endmacro()

# Dependendencies
set(BOOST_FILESYSTEM_BOOSTDEP system)
set(BOOST_THREAD_BOOSTDEP system)

# Sources for boost_iostreams
set(BOOST_IOSTREAMS_SOURCES
    ${BOOST_ROOT}/libs/iostreams/src/file_descriptor.cpp
    ${BOOST_ROOT}/libs/iostreams/src/mapped_file.cpp)

function(s1_get_boost_link_libs VAR)
  set(link_libs)
  foreach(lib_name ${ARGN})
    string(TOUPPER "${lib_name}" lib_upper)
    if(BOOST_${lib_upper}_BOOSTDEP)
      s1_get_boost_link_libs(lib_deps ${BOOST_${lib_upper}_BOOSTDEP})
      list(APPEND link_libs ${lib_deps})
    endif()
    set(target_suffix "")
    if(MSVC)
      # Use different Boost libs for different runtime libs options
      string(REGEX MATCH "/(M.)" runtime_arg "${CMAKE_CXX_FLAGS_RELEASE}")
      # runtime_arg contains the initial slash, CMAKE_MATCH_1 doesn't.
      set(target_suffix "-${CMAKE_MATCH_1}")
    endif()
    if(S1_REQUIRED_CXX_FEATURES)
      set(target_suffix "${target_suffix}-11")
    endif()
    set(private_target_name "boost_${lib_name}_s1${target_suffix}")
    if(NOT TARGET ${private_target_name})
      if(BOOST_${lib_upper}_SOURCES)
        set(lib_sources ${BOOST_${lib_upper}_SOURCES})
      else()
        file(GLOB_RECURSE lib_sources ${BOOST_ROOT}/libs/${lib_name}/src/*.cpp)
      endif()
      if(lib_sources)
        s1_add_library(${private_target_name} STATIC
                       SOURCES ${lib_sources})
        target_compile_definitions(${private_target_name} PUBLIC "-DBOOST_${lib_upper}_STATIC_LINK=1" ${BOOST_${lib_upper}_DEFS})
        # Allow specification of source-specific compile definitions
        foreach(src ${lib_sources})
          get_filename_component(source_name ${src} NAME_WE)
          string(TOUPPER "${source_name}" source_name_upper)
          set_property(SOURCE "${src}" PROPERTY COMPILE_DEFINITIONS_DEBUG "${BOOST_${lib_upper}_${source_name_upper}_COMPILE_DEFS}")
        endforeach()
        if(BOOST_${lib_upper}_DEPENDS)
          target_link_libraries(${private_target_name} PUBLIC ${BOOST_${lib_upper}_DEPENDS})
        endif()
      elseif(${Boost_${lib_upper}_FOUND})
        # Add an interface library if we don't have sources
        add_library(${private_target_name} INTERFACE)
        target_link_libraries(${private_target_name} INTERFACE ${Boost_${lib_upper}_LIBRARY} ${BOOST_${lib_upper}_DEPENDS})
        target_compile_definitions(${private_target_name} INTERFACE "-DBOOST_${lib_upper}_STATIC_LINK=1")
      endif()
    endif()
    list(APPEND link_libs ${private_target_name})
  endforeach()
  list(REVERSE link_libs)
  list(REMOVE_DUPLICATES link_libs)
  list(REVERSE link_libs)
  set(${VAR} ${link_libs} PARENT_SCOPE)
endfunction()
