# We have unusual requirements for Boost (requiring it to be built with
# different build flags in different places), so we wrap it's usage.

# List of all the Boost libs we use anywhere in the source tree
set(_S1_BOOST_USED_LIBS filesystem iostreams program_options system unit_test_framework)

macro(s1_find_boost VERSION)
  if(ARGN)
    message(FATAL_ERROR "Too many arguments to s1_find_boost")
  endif()
  # Fail if Boost is not found
  find_package(Boost ${VERSION} REQUIRED)
  # But it's okay if we don't have the libs
  find_package(Boost ${VERSION} COMPONENTS ${_S1_BOOST_USED_LIBS})
endmacro()

# Dependendencies on Boost libs
set(BOOST_FILESYSTEM_BOOSTDEP system)
set(BOOST_THREAD_BOOSTDEP system)
# Set BOOST_xxx_DEPENDS for other dependencies

# Sources for boost_iostreams
set(iostreams_src "${BOOST_ROOT}/libs/iostreams/src")
if(IS_DIRECTORY ${iostreams_src})
  set(BOOST_IOSTREAMS_SOURCES
      ${iostreams_src}/file_descriptor.cpp
      ${iostreams_src}/mapped_file.cpp)
endif()
# Sources for boost_unit_test_framework
set(unit_test_framework_src "${BOOST_ROOT}/libs/test/src")
if(IS_DIRECTORY ${unit_test_framework_src})
  set(BOOST_UNIT_TEST_FRAMEWORK_SOURCES
      ${unit_test_framework_src}/compiler_log_formatter.cpp
      ${unit_test_framework_src}/debug.cpp
      ${unit_test_framework_src}/decorator.cpp
      ${unit_test_framework_src}/execution_monitor.cpp
      ${unit_test_framework_src}/framework.cpp
      ${unit_test_framework_src}/junit_log_formatter.cpp
      ${unit_test_framework_src}/plain_report_formatter.cpp
      ${unit_test_framework_src}/progress_monitor.cpp
      ${unit_test_framework_src}/results_collector.cpp
      ${unit_test_framework_src}/results_reporter.cpp
      ${unit_test_framework_src}/test_framework_init_observer.cpp
      ${unit_test_framework_src}/test_tools.cpp
      ${unit_test_framework_src}/test_tree.cpp
      ${unit_test_framework_src}/unit_test_log.cpp
      ${unit_test_framework_src}/unit_test_main.cpp
      ${unit_test_framework_src}/unit_test_monitor.cpp
      ${unit_test_framework_src}/unit_test_parameters.cpp
      ${unit_test_framework_src}/xml_log_formatter.cpp
      ${unit_test_framework_src}/xml_report_formatter.cpp
      )
endif()

function(_s1_get_boost_link_libs VAR TARGET_LINK)
  set(link_libs)
  foreach(lib_name ${ARGN})
    string(TOUPPER "${lib_name}" lib_upper)
    set(lib_deps)
    if(BOOST_${lib_upper}_BOOSTDEP)
      _s1_get_boost_link_libs(lib_deps ${TARGET_LINK} ${BOOST_${lib_upper}_BOOSTDEP})
    endif()
    set(target_suffix "")
    if(MSVC)
      # Use different Boost libs for different runtime libs options
      string(REGEX MATCH "/(M.)" runtime_arg "${CMAKE_CXX_FLAGS_RELEASE}")
      # runtime_arg contains the initial slash, CMAKE_MATCH_1 doesn't.
      set(target_suffix "-${CMAKE_MATCH_1}")
    endif()
    if(S1_REQUIRED_CXX_FEATURES)
      set(target_suffix "${target_suffix}-feat")
    endif()
    string(TOLOWER "${TARGET_LINK}" target_link_lower)
    set(private_target_name "boost_${lib_name}_s1${target_suffix}-${target_link_lower}")
    if(NOT TARGET ${private_target_name})
      if(BOOST_${lib_upper}_SOURCES)
        set(lib_sources ${BOOST_${lib_upper}_SOURCES})
      else()
        file(GLOB_RECURSE lib_sources ${BOOST_ROOT}/libs/${lib_name}/src/*.cpp)
      endif()
      if(lib_sources)
        s1_add_library_internal(${private_target_name} TARGET_LINK ${TARGET_LINK}
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
        set_property(TARGET ${private_target_name} PROPERTY EXCLUDE_FROM_ALL YES)
      elseif(${Boost_${lib_upper}_FOUND})
        # Add an interface library if we don't have sources
        add_library(${private_target_name} INTERFACE)
        target_link_libraries(${private_target_name} INTERFACE ${Boost_${lib_upper}_LIBRARY} ${BOOST_${lib_upper}_DEPENDS})
        target_compile_definitions(${private_target_name} INTERFACE "-DBOOST_${lib_upper}_STATIC_LINK=1")
      endif()
    endif()
    list(APPEND link_libs ${private_target_name} ${lib_deps})
  endforeach()
  list(REVERSE link_libs)
  list(REMOVE_DUPLICATES link_libs)
  list(REVERSE link_libs)
  set(${VAR} ${link_libs} PARENT_SCOPE)
endfunction()

function(s1_target_link_boost TARGET)
  get_target_property(TARGET_TYPE ${TARGET} TYPE)
  if (("${TARGET_TYPE}" STREQUAL "SHARED_LIBRARY") OR ("${TARGET_TYPE}" STREQUAL "EXECUTABLE"))
    set(TARGET_LINK SHARED)
  else()
    set(TARGET_LINK STATIC)
  endif()

  _s1_get_boost_link_libs(_LINK_BOOST ${TARGET_LINK} ${ARGN})
  target_link_libraries(${TARGET} ${_LINK_BOOST})
endfunction()
