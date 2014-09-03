PROJECT(s1)
cmake_minimum_required(VERSION 2.8.12)

# CMake cache variables we will augment with out own detected settings
set(OVERRIDE_CACHE_VARS CXX_FLAGS
                        EXE_LINKER_FLAGS
                        SHARED_LINKER_FLAGS)
foreach(release_mode RELEASE RELWITHDEBINFO MINSIZEREL)
  list(APPEND OVERRIDE_CACHE_VARS
       CXX_FLAGS_${release_mode}
       C_FLAGS_${release_mode}
       EXE_LINKER_FLAGS_${release_mode}
       SHARED_LINKER_FLAGS_${release_mode}
       STATIC_LINKER_FLAGS_${release_mode})
endforeach()
foreach(cached_var ${OVERRIDE_CACHE_VARS})
  set(CACHE_${cached_var} ${CMAKE_${cached_var}})
endforeach()

if(NOT CMAKE_BUILD_TYPE AND NOT MSVC)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(CxxTest)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libutils.cmake)

if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
endif()

if(ICU_PREFIX)
  get_filename_component(ICU_PREFIX "${ICU_PREFIX}" ABSOLUTE)
  find_package_message(ICU_PREFIX "Using user-specified ICU_PREFIX: ${ICU_PREFIX}" ${ICU_PREFIX})
  set(ICU_FOUND ON)
  # FIXME: Those are the names for MSVC builds
  if(CMAKE_GENERATOR MATCHES "Win64")
	link_directories("${ICU_PREFIX}/lib64")
  else()
	link_directories("${ICU_PREFIX}/lib")
  endif()
  set(ICU_LIBRARIES icuin icuuc icudt)
  set(ICU_ICUIO_LIBRARIES icuio)
  set(ICU_INCLUDE_DIRS "${ICU_PREFIX}/include")
else()
  find_program(ICU_CONFIG icu-config)
  if(NOT ICU_CONFIG)
    message(FATAL_ERROR "ICU is required. Make sure 'icu-config' is in PATH or set the cmake var ICU_PREFIX.")
  endif()
  find_package_message(ICU_CONFIG "icu-config: ${ICU_CONFIG}" ${ICU_CONFIG})
  set(ICU_FOUND ON)
  execute_process(COMMAND ${ICU_CONFIG} --ldflags
		  OUTPUT_VARIABLE ICU_LIBRARIES)
  string(STRIP "${ICU_LIBRARIES}" ICU_LIBRARIES)
  execute_process(COMMAND ${ICU_CONFIG} --ldflags-icuio
		  OUTPUT_VARIABLE ICU_ICUIO_LIBRARIES)
  string(STRIP "${ICU_ICUIO_LIBRARIES}" ICU_ICUIO_LIBRARIES)
  execute_process(COMMAND ${ICU_CONFIG} --cppflags-searchpath
		  OUTPUT_VARIABLE ICU_INCLUDE_DIRS)
  string(REGEX REPLACE "^-I" "" ICU_INCLUDE_DIRS "${ICU_INCLUDE_DIRS}")
endif()
include_directories(${ICU_INCLUDE_DIRS})

find_package(Boost 1.53.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include(CheckCXXCompilerFlag)
function(CxxCompilerFlagString flagvar flagstr)
  CHECK_CXX_COMPILER_FLAG("${flagstr}" ${flagvar}_FOUND)
  if(${flagvar}_FOUND)
    set(${flagvar} "${flagstr}" PARENT_SCOPE)
  endif()
endfunction()
function(CxxCompilerFlagStringAppend flagvar flagstr)
  string(TOUPPER "${flagstr}" auto_flag_var)
  string(REGEX REPLACE "[^A-Z0-9]" "_" auto_flag_var "${auto_flag_var}")
  CxxCompilerFlagString (CXX_${auto_flag_var} "${flagstr}")
  if(NOT "${CXX_${auto_flag_var}}" EQUAL "")
    set(${flagvar} "${${flagvar}} ${CXX_${auto_flag_var}}" PARENT_SCOPE)
  endif()
endfunction()

if(NOT MSVC)
  # Annoying on MSVC; lots of warnings from inside Boost
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-Wall")
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-Wno-unused-local-typedefs")
endif()
# gcc flags that improve size of binaries
if(CMAKE_COMPILER_IS_GNUCXX)
  # Default visibility
  CxxCompilerFlagStringAppend (CMAKE_CXX_FLAGS "-fvisibility-inlines-hidden -fvisibility=hidden")
  # Put data into own sections (so the linker discard if unused ones)
  CxxCompilerFlagStringAppend (CMAKE_CXX_FLAGS "-fdata-sections")
  # Put functions into own sections (again, so unused ones can be discarded by the linker)
  CxxCompilerFlagStringAppend (CMAKE_CXX_FLAGS "-ffunction-sections")
endif()

CxxCompilerFlagString (CXX_AS_NEEDED_FLAG "-Wl,--as-needed")
CxxCompilerFlagString (CXX_NO_UNDEFINED_FLAG "-Wl,--no-undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG} ${CXX_NO_UNDEFINED_FLAG}")

# Check if we can link the C++ standard library statically
set(S1_STATIC_LIBSTDCXX ON CACHE BOOL "Whether to statically link libstdc++")
if(S1_STATIC_LIBSTDCXX)
  CxxCompilerFlagString (CXX_STATIC_LIBSTDCXX_FLAG "-static-libstdc++")
  if(CXX_STATIC_LIBSTDCXX_FLAG)
    # The flag is supported, but it may not actually work.
    message(STATUS "Checking if ${CXX_STATIC_LIBSTDCXX_FLAG} works")
    try_compile(STATIC_LIBSTDCXX_WORKS
                ${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/config-tests/static-libstdcxx.cpp
                LINK_LIBRARIES ${CXX_STATIC_LIBSTDCXX_FLAG})
    message(STATUS "Checking if ${CXX_STATIC_LIBSTDCXX_FLAG} works - ${STATIC_LIBSTDCXX_WORKS}")
    if(NOT STATIC_LIBSTDCXX_WORKS)
      set(CXX_STATIC_LIBSTDCXX_FLAG "")
    endif()
  endif()
endif()

# Look for symbolic link options
CxxCompilerFlagString (CXX_SYMBOLIC_LINK_FLAG "-Wl,-Bsymbolic -Wl,-Bsymbolic-functions")
set(CACHE_SHARED_LINKER_FLAGS "${CACHE_SHARED_LINKER_FLAGS} ${CXX_SYMBOLIC_LINK_FLAG}")

# Look for section GC options
CxxCompilerFlagString (CXX_LINK_GC_SECTIONS "-Wl,--gc-sections")
set(CACHE_SHARED_LINKER_FLAGS "${CACHE_SHARED_LINKER_FLAGS} ${CXX_LINK_GC_SECTIONS}")

CHECK_CXX_SOURCE_COMPILES(
  "int main() { void* p = nullptr; return 0; }"
  CXX_HAS_NULLPTR)
if(NOT CXX_HAS_NULLPTR)
endif()

# Build shared libs by default
set(S1_BUILD_SHARED ON CACHE BOOL "Whether to build Shader1 as a shared library")
set(BUILD_SHARED_LIBS ${S1_BUILD_SHARED})

if(MSVC)
  # Automatically make use of 'secure' CRT funtions
  add_definitions("-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1")
  add_definitions("-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1")
  # Silence remaining annoyances
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")

  # More aggressive optimization flags for release configs
  string(REPLACE "/Ob1" "" CACHE_C_FLAGS_RELWITHDEBINFO "${CACHE_C_FLAGS_RELWITHDEBINFO}")
  string(REPLACE "/Ob1" "" CACHE_CXX_FLAGS_RELWITHDEBINFO "${CACHE_CXX_FLAGS_RELWITHDEBINFO}")
  string(REPLACE "/Ob2" "" CACHE_C_FLAGS_RELEASE "${CACHE_C_FLAGS_RELEASE}")
  string(REPLACE "/Ob2" "" CACHE_CXX_FLAGS_RELEASE "${CACHE_CXX_FLAGS_RELEASE}")
  string(REPLACE "/Ob1" "" CACHE_C_FLAGS_MINSIZEREL "${CACHE_C_FLAGS_MINSIZEREL}")
  string(REPLACE "/Ob1" "" CACHE_CXX_FLAGS_MINSIZEREL "${CACHE_CXX_FLAGS_MINSIZEREL}")
  # Re-enable certain optimizations
  string(REPLACE "/INCREMENTAL" "/INCREMENTAL:NO /opt:ref" CACHE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CACHE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
  string(REPLACE "/INCREMENTAL" "/INCREMENTAL:NO /opt:ref" CACHE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CACHE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}")

  # Check for data COMDATs (eq. to gcc's -fdata-sections)
  CxxCompilerFlagString (CXX_GW " /Gw")
  set(CACHE_CXX_FLAGS_RELWITHDEBINFO "${CACHE_CXX_FLAGS_RELWITHDEBINFO}${CXX_GW}")
  set(CACHE_CXX_FLAGS_RELEASE "${CACHE_CXX_FLAGS_RELEASE}${CXX_GW}")
  set(CACHE_CXX_FLAGS_MINSIZEREL "${CACHE_CXX_FLAGS_MINSIZEREL}${CXX_GW}")
  
  # Check for extended optimized build debug info
  CxxCompilerFlagString (CXX_ZO " /Zo")
  set(CACHE_CXX_FLAGS_RELWITHDEBINFO "${CACHE_CXX_FLAGS_RELWITHDEBINFO}${CXX_ZO}")
  # New option in VC 12. Needed when more than one project writes to a PDB
  CxxCompilerFlagString (CXX_PDB_SYNC " /FS")
endif()

# Build everything with -fPIC to allow linking into shared libs
if(UNIX)
  set(CACHE_CXX_FLAGS "${CACHE_CXX_FLAGS} -fPIC")
  set(WITH_PIC YES)
endif()

# LTO configuration
set(S1_BUILD_LTO ${MSVC} CACHE BOOL "Enable building with LTO/LTCG")
if(S1_BUILD_LTO)
  message(STATUS "LTO/LTCG is enabled")
  if(CMAKE_COMPILER_IS_GNUCXX)
    CxxCompilerFlagString (CXX_LTO_FLAG "-flto")
    CxxCompilerFlagString (CXX_NO_LTO_FLAG "-fno-lto")
    set(CACHE_CXX_FLAGS "${CACHE_CXX_FLAGS} ${CXX_LTO_FLAG}")
    # At least with gcc 4.8 -flto causes problems when linking the examples.
    # Must be explicitly disabled as CMake passes CMAKE_CXX_FLAGS to
    # the linker as well.
    set(CACHE_EXE_LINKER_FLAGS "${CACHE_EXE_LINKER_FLAGS} ${CXX_NO_LTO_FLAG}")
    # TODO: set CMAKE_AR/_NM_/_RANLIB to gcc-* variants
  elseif(MSVC)
    foreach(mode RELEASE MINSIZEREL RELWITHDEBINFO)
      set(CACHE_CXX_FLAGS_${mode} "${CACHE_CXX_FLAGS_${mode}} /GL")
      set(CACHE_EXE_LINKER_FLAGS_${mode} "${CACHE_EXE_LINKER_FLAGS_${mode}} /LTCG")
      set(CACHE_SHARED_LINKER_FLAGS_${mode} "${CACHE_SHARED_LINKER_FLAGS_${mode}} /LTCG")
      set(CACHE_STATIC_LINKER_FLAGS_${mode} "${CACHE_STATIC_LINKER_FLAGS_${mode}} /LTCG")
    endforeach()
  else()
    message(WARNING "LTO/LTCG is not supported for your compiler")
  endif()
endif()

# Update cache values with config check results
foreach(cached_var ${OVERRIDE_CACHE_VARS})
  if(NOT CACHE_SET_${cached_var})
    set_property(CACHE CMAKE_${cached_var}
                 PROPERTY VALUE ${CACHE_${cached_var}})
    set(CACHE_SET_${cached_var} 1
        CACHE INTERNAL "The CMAKE_${cached_var} variable was augmented with out own settings.")
  endif()
endforeach()

# Configuration settings only relevant for building
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/internal/base)
configure_file(include/internal/base/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/internal/base/config.h)
# Configuration settings exposed to clients
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/s1)
configure_file(include/s1/s1config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/s1/s1config.h)

add_custom_target(build_tests)
set_property(TARGET build_tests PROPERTY EXCLUDE_FROM_ALL YES)
function(AutoBuildTest test_name)
  if(NOT MSVC)
    add_test(test_build_${test_name} "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${test_name})
    set_tests_properties(${test_name} PROPERTIES DEPENDS test_build_${test_name})
  endif()
  add_dependencies(build_tests ${test_name})
endfunction()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/internal)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(include/internal)
include_directories(include)

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(apps)

add_subdirectory(doc)
