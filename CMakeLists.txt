PROJECT(s1)
cmake_minimum_required(VERSION 3.9)
if(POLICY CMP0060)
  cmake_policy(SET CMP0060 NEW)
endif()

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
  # Remove the settings we applied from the cached var
  string(REPLACE " " ";" ${cached_var}_AS_LIST "${COMPUTED_${cached_var}}")
  foreach(part ${${cached_var}_AS_LIST})
    # Note: Space after 'part' is important!
    string(REPLACE "${part} " "" CMAKE_${cached_var} "${CMAKE_${cached_var}}")
  endforeach()
  string(STRIP "${CMAKE_${cached_var}}" CMAKE_${cached_var})
endforeach()

if(NOT CMAKE_BUILD_TYPE AND NOT MSVC)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(PythonInterp 3 REQUIRED)
find_package(CxxTest)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(s1_add_library)
include(s1_add_cxxtest)
include(s1_boost)
include(s1_detect_stdlib)
include(s1_strip_debug)

if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
endif()

if(WIN32)
  add_definitions(-DUNICODE -D_UNICODE)
endif()

s1_find_boost(1.60)
include_directories(${Boost_INCLUDE_DIRS})
add_definitions("-DBOOST_ALL_NO_LIB")

include(CheckCXXCompilerFlag)
include(CheckCXXSymbolExists)
include(CheckSymbolExists)
include(CheckTypeSize)

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
    set(${flagvar} "${${flagvar}} ${CXX_${auto_flag_var}} " PARENT_SCOPE)
  endif()
endfunction()

function(FlagStringAppend flagvar flagstr)
  set(${flagvar} "${${flagvar}} ${flagstr} " PARENT_SCOPE)
endfunction()

if(NOT MSVC)
  # Annoying on MSVC; lots of warnings from inside Boost
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-Wall")
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-Wno-unused-local-typedefs")
endif()
# gcc flags that improve size of binaries
if(NOT MSVC)
  # Default visibility
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-fvisibility-inlines-hidden -fvisibility=hidden")
  # Put data into own sections (so the linker discard if unused ones)
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-fdata-sections")
  # Put functions into own sections (again, so unused ones can be discarded by the linker)
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-ffunction-sections")
  # Clang: Silence this warning
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-Qunused-arguments")
endif()

CxxCompilerFlagString (CXX_AS_NEEDED_FLAG "-Wl,--as-needed")
CxxCompilerFlagString (CXX_NO_AS_NEEDED_FLAG "-Wl,--no-as-needed")
CxxCompilerFlagString (CXX_NO_UNDEFINED_FLAG "-Wl,--no-undefined")
FlagStringAppend(CACHE_EXE_LINKER_FLAGS "${CXX_AS_NEEDED_FLAG}")
FlagStringAppend(CACHE_SHARED_LINKER_FLAGS "${CXX_AS_NEEDED_FLAG} ${CXX_NO_UNDEFINED_FLAG}")

if(NOT MSVC)
  # Force pthreads to be always linked; even if no function as such is used we
  # still need thread support to be present.
  set(CMAKE_THREAD_LIBS_INIT "${CXX_NO_AS_NEEDED_FLAG} ${CMAKE_THREAD_LIBS_INIT}")
endif()

# Use critical sections instead of std::mutex with MSVC
set(S1_PREFER_CRITICAL_SECTIONS ON CACHE BOOL "Whether to use critical sections instead of std::mutex with MSVC")

# Build shared libs by default
set(S1_BUILD_SHARED ON CACHE BOOL "Whether to build Shader1 as a shared library")
set(BUILD_SHARED_LIBS ${S1_BUILD_SHARED})

s1_detect_stdlib(CXX_STDLIB)
message(STATUS "Detected C++ standard library: ${CXX_STDLIB}")

function(VerifyStaticLinkFlag FLAG)
  if(${FLAG})
    # The flag is supported, but it may not actually work.
    message(STATUS "Checking if ${${FLAG}} works")
    try_compile(${FLAG}_WORKS
                ${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/config-tests/static-libstdcxx.cpp
                LINK_LIBRARIES ${${FLAG}})
    message(STATUS "Checking if ${${FLAG}} works - ${${FLAG}_WORKS}")
    if(NOT ${FLAG}_WORKS)
      set(${FLAG} "" PARENT_SCOPE)
    endif()
  endif()
endfunction()

# Prefer a static standard lib.
SET(S1_STATIC_STDLIB ${S1_BUILD_SHARED} CACHE BOOL "Whether to link the C++ standard library statically")
if(CXX_STDLIB STREQUAL "msvcrt")
  SET(S1_STATIC_CRT ${S1_STATIC_STDLIB})
elseif(CXX_STDLIB STREQUAL "libstdc++")
  if(S1_STATIC_STDLIB)
    CxxCompilerFlagString (CXX_STATIC_LIBSTDCXX_FLAG "-static-libstdc++")
    VerifyStaticLinkFlag (CXX_STATIC_LIBSTDCXX_FLAG)
  endif()
endif()

# Look for symbolic link options
CxxCompilerFlagString (CXX_SYMBOLIC_LINK_FLAG "-Wl,-Bsymbolic -Wl,-Bsymbolic-functions")
FlagStringAppend(CACHE_EXE_LINKER_FLAGS "${CXX_SYMBOLIC_LINK_FLAG}")
FlagStringAppend(CACHE_SHARED_LINKER_FLAGS "${CXX_SYMBOLIC_LINK_FLAG}")

# Look for section GC options
CxxCompilerFlagString (CXX_LINK_GC_SECTIONS "-Wl,--gc-sections")
FlagStringAppend(CACHE_EXE_LINKER_FLAGS "${CXX_LINK_GC_SECTIONS}")
FlagStringAppend(CACHE_SHARED_LINKER_FLAGS "${CXX_LINK_GC_SECTIONS}")

CHECK_TYPE_SIZE(wchar_t CXX_SIZEOF_WCHAR_T BUILTIN_TYPES_ONLY LANGUAGE CXX)
CHECK_SYMBOL_EXISTS(mbrtoc16 uchar.h HAVE_UCHAR_H_MBRTOC16)
CHECK_CXX_SYMBOL_EXISTS(_msize malloc.h HAVE__MSIZE)
CHECK_CXX_SYMBOL_EXISTS(malloc_usable_size malloc.h HAVE_MALLOC_USABLE_SIZE)

# If we're on a version based off the universal always prefer a dynamic UCRT component
SET(S1_UCRT_DYNAMIC ON CACHE BOOL "Whether to always use a dynamic MSVC universal runtime")

if(MSVC)
  # Automatically make use of 'secure' CRT funtions
  add_definitions("-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1")
  add_definitions("-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1")
  # Silence remaining annoyances
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")

  # Check for data COMDATs (eq. to gcc's -fdata-sections)
  CxxCompilerFlagString (CXX_GW " /Gw")
  # Check for omission of unused COMDATs
  CxxCompilerFlagString (CXX_ZCINLINE " /Zc:inline")

  # Apply more optimization flags
  SET(EXTRA_RELEASE_FLAGS "${CXX_GW}${CXX_ZCINLINE} ")
  SET(EXTRA_RELEASE_LINK_FLAGS "/opt:ref /opt:icf")
  foreach(release_mode RELEASE RELWITHDEBINFO MINSIZEREL)
    FlagStringAppend(CACHE_CXX_FLAGS_${release_mode} "${EXTRA_RELEASE_FLAGS}")
    FlagStringAppend(CACHE_EXE_LINKER_FLAGS_${release_mode} "${EXTRA_RELEASE_LINK_FLAGS}")
    FlagStringAppend(CACHE_SHARED_LINKER_FLAGS_${release_mode} "${EXTRA_RELEASE_LINK_FLAGS}")
  endforeach()
  
  # Check for extended optimized build debug info
  CxxCompilerFlagString (CXX_ZO " /Zo")
  FlagStringAppend(CACHE_CXX_FLAGS_RELWITHDEBINFO "${CXX_ZO} ")
  # New option in VC 12. Needed when more than one project writes to a PDB
  CxxCompilerFlagString (CXX_PDB_SYNC "/FS")
  # Interpret source files as UTF-8
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "/utf-8")
endif()

# Build everything with -fPIC to allow linking into shared libs
if(UNIX)
  FlagStringAppend(CACHE_CXX_FLAGS "-fPIC")
  set(WITH_PIC YES)
endif()

# LTO configuration
set(S1_BUILD_LTO ${MSVC} CACHE BOOL "Enable building with LTO/LTCG")
if(S1_BUILD_LTO)
  message(STATUS "LTO/LTCG is enabled")
  if(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    if(CMAKE_COMPILER_IS_GNUCXX)
      CxxCompilerFlagString (CXX_LTO_FLAG "-flto")
      CxxCompilerFlagString (CXX_NO_LTO_FLAG "-fno-lto")
      FlagStringAppend(CACHE_CXX_FLAGS "${CXX_LTO_FLAG}")
    else()
      FlagStringAppend(CACHE_CXX_FLAGS "-flto")
      FlagStringAppend(CACHE_EXE_LINKER_FLAGS "-flto")
      FlagStringAppend(CACHE_SHARED_LINKER_FLAGS "-flto")
    endif()
    # Set CMAKE_AR/_RANLIB to compiler-specific variants
    foreach(tool ar ranlib)
      string(TOUPPER "${tool}" tool_u)
      if(CMAKE_CXX_COMPILER_${tool_u})
        set(CMAKE_${tool_u} "${CMAKE_CXX_COMPILER_${tool_u}}" CACHE FILEPATH "Path to ${tool}")
        set(CMAKE_${tool_u} "${CMAKE_CXX_COMPILER_${tool_u}}")
      endif()
    endforeach()
  elseif(MSVC)
    foreach(mode RELEASE MINSIZEREL RELWITHDEBINFO)
      FlagStringAppend(CACHE_CXX_FLAGS_${mode} "/GL")
      FlagStringAppend(CACHE_EXE_LINKER_FLAGS_${mode} "/LTCG")
      FlagStringAppend(CACHE_SHARED_LINKER_FLAGS_${mode} "/LTCG")
      FlagStringAppend(CACHE_STATIC_LINKER_FLAGS_${mode} "/LTCG")
    endforeach()
  else()
    message(WARNING "LTO/LTCG is not supported for your compiler")
  endif()
endif()

# Update cache values with config check results
foreach(cached_var ${OVERRIDE_CACHE_VARS})
  # Save the values we computed for subsequent removal
  set(COMPUTED_${cached_var} ${CACHE_${cached_var}}
      CACHE INTERNAL "Our own settings for CMAKE_${cached_var}.")
  set(CMAKE_${cached_var} "${CACHE_${cached_var}} ${CMAKE_${cached_var}}")
endforeach()

# Remove some options from default build flags
### FIXME: Also removes user-set options :(
if(MSVC)
  # More aggressive optimization flags for release configs
  string(REPLACE "/Ob1" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
  string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  string(REPLACE "/Ob2" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
  string(REPLACE "/Ob2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "/Ob1" "" CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
  string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")

  string(REPLACE " /INCREMENTAL " "" CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO " ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ")
  string(REPLACE " /INCREMENTAL " "" CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO " ${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} ")
endif()

foreach(cached_var ${OVERRIDE_CACHE_VARS})
  set_property(CACHE CMAKE_${cached_var}
               PROPERTY VALUE ${CMAKE_${cached_var}})
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
    target_link_libraries(${test_name} ${CMAKE_THREAD_LIBS_INIT})
  endif()
  add_dependencies(build_tests ${test_name})
endfunction()

# All C++ features we require
set(ALL_REQUIRED_CXX_FEATURES
    cxx_std_14)
if(NOT S1_BUILD_SHARED)
  # For static builds, require C++ features globally (to avoid conflicts
  # between lib and apps due to differing build flags)
  set(S1_REQUIRED_CXX_FEATURES ${ALL_REQUIRED_CXX_FEATURES})
endif()

# S1_PREBUILT: Points to a build tree with an already built libs1, which is then
# used to build s1.
# Useful if the current compiler can't build libs1 itself (due to required
# C++ features not being supported) but s1c should be built nevertheless (e.g.
# to check include files compatibility)
if(NOT S1_PREBUILT)
  # Make outcome available
  add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/outcome"  # path to outcome source
    "${CMAKE_CURRENT_BINARY_DIR}/outcome"  # your choice of where to put binaries
    EXCLUDE_FROM_ALL                             # please only lazy build outcome on demand
  )

  add_subdirectory(include)
  set(S1_INC "${CMAKE_SOURCE_DIR}/include/internal")
  add_subdirectory(lib)

  add_subdirectory(doc)
else()
  find_package(s1 CONFIG HINTS "${S1_PREBUILT}")
endif()
add_subdirectory(apps)

if(NOT S1_PREBUILT)
  # Export targets so pointing S1_PREBUILT at build tree works as well
  export(TARGETS s1_headers FILE s1_headers.cmake)
  export(TARGETS s1 FILE libs1.cmake)
  export(TARGETS s1c FILE s1c.cmake)

  install(EXPORT s1 FILE libs1.cmake DESTINATION lib/cmake/s1)
  install(EXPORT s1_headers DESTINATION lib/cmake/s1)
  install(EXPORT s1c FILE s1c.cmake DESTINATION lib/cmake/s1)

  include(CMakePackageConfigHelpers)
  configure_package_config_file(s1-config.cmake.in
                                ${CMAKE_CURRENT_BINARY_DIR}/s1-config.cmake
                                INSTALL_DESTINATION lib/cmake/s1)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/s1-config.cmake
          DESTINATION lib/cmake/s1)
endif()
