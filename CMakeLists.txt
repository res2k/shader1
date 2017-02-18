PROJECT(s1)
cmake_minimum_required(VERSION 3.1.0)
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
  set(CACHE_${cached_var} ${CMAKE_${cached_var}})
endforeach()

if(NOT CMAKE_BUILD_TYPE AND NOT MSVC)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(PythonInterp 3 REQUIRED)
find_package(CxxTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(s1_add_library)
include(s1_add_cxxtest)
include(s1_boost)

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
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-fvisibility-inlines-hidden -fvisibility=hidden")
  # Put data into own sections (so the linker discard if unused ones)
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-fdata-sections")
  # Put functions into own sections (again, so unused ones can be discarded by the linker)
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "-ffunction-sections")
endif()

CxxCompilerFlagString (CXX_AS_NEEDED_FLAG "-Wl,--as-needed")
CxxCompilerFlagString (CXX_NO_UNDEFINED_FLAG "-Wl,--no-undefined")
set(CACHE_EXE_LINKER_FLAGS "${CACHE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")
set(CACHE_SHARED_LINKER_FLAGS "${CACHE_SHARED_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG} ${CXX_NO_UNDEFINED_FLAG}")

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

CHECK_TYPE_SIZE(wchar_t CXX_SIZEOF_WCHAR_T BUILTIN_TYPES_ONLY LANGUAGE CXX)
CHECK_SYMBOL_EXISTS(mbrtoc16 uchar.h HAVE_UCHAR_H_MBRTOC16)

# Build shared libs by default
set(S1_BUILD_SHARED ON CACHE BOOL "Whether to build Shader1 as a shared library")
set(BUILD_SHARED_LIBS ${S1_BUILD_SHARED})

# Prefer a static MSVC runtime.
SET(S1_STATIC_CRT ${S1_BUILD_SHARED} CACHE BOOL "Whether to link the MSVC runtime statically")
# However, if we're on a version based off the universal, always use a dynamic UCRT component
SET(S1_UCRT_DYNAMIC ON CACHE BOOL "Whether to always use a dynamic MSVC universal runtime")

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
  # Check for omission of unused COMDATs
  CxxCompilerFlagString (CXX_ZCINLINE " /Zc:inline")
  
  SET(EXTRA_RELEASE_FLAGS "${CXX_GW}${CXX_ZCINLINE}")
  set(CACHE_CXX_FLAGS_RELWITHDEBINFO "${CACHE_CXX_FLAGS_RELWITHDEBINFO}${EXTRA_RELEASE_FLAGS}")
  set(CACHE_CXX_FLAGS_RELEASE "${CACHE_CXX_FLAGS_RELEASE}${EXTRA_RELEASE_FLAGS}")
  set(CACHE_CXX_FLAGS_MINSIZEREL "${CACHE_CXX_FLAGS_MINSIZEREL}${EXTRA_RELEASE_FLAGS}")
  
  # Check for extended optimized build debug info
  CxxCompilerFlagString (CXX_ZO " /Zo")
  set(CACHE_CXX_FLAGS_RELWITHDEBINFO "${CACHE_CXX_FLAGS_RELWITHDEBINFO}${CXX_ZO}")
  # New option in VC 12. Needed when more than one project writes to a PDB
  CxxCompilerFlagString (CXX_PDB_SYNC "/FS")
  # Interpret source files as UTF-8
  CxxCompilerFlagStringAppend (CACHE_CXX_FLAGS "/utf-8")
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
    # Set CMAKE_AR/_NM_/_RANLIB to gcc-* variants
    foreach(tool ar nm ranlib)
      string(TOUPPER "${tool}" tool_u)
      find_program(GCC_${tool_u} gcc-${tool})
      if(GCC_${tool_u})
        set(CMAKE_${tool_u} "${GCC_${tool_u}}" CACHE FILEPATH "Path to ${tool}")
      endif()
    endforeach()
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
        CACHE INTERNAL "The CMAKE_${cached_var} variable was augmented with our own settings.")
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

# All C++11 features we require
set(ALL_REQUIRED_CXX_FEATURES cxx_nullptr cxx_range_for cxx_rvalue_references
    cxx_variadic_templates cxx_static_assert cxx_override cxx_auto_type
    cxx_enum_forward_declarations cxx_strong_enums cxx_alignas)
# Actually we also require cxx_nonstatic_member_init ... but CMake doesn't grant
# it to MSVC 2013 since it's only partially supported there. However, it's still
# sufficient for us.
if(NOT S1_BUILD_SHARED)
  # For static builds, require C++11 features globally (to avoid conflicts
  # between lib and apps due to differing build flags)
  set(S1_REQUIRED_CXX_FEATURES ${ALL_REQUIRED_CXX_FEATURES})
endif()

# S1_PREBUILT: Points to a build tree with an already built libs1, which is then
# used to build s1.
# Useful if the current compiler can't build libs1 itself (due to required
# C++ features not being supported) but s1c should be built nevertheless (e.g.
# to check include files compatibility)
if(NOT S1_PREBUILT)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/internal)
  include_directories(include/internal)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
endif()
include_directories(include)

if(NOT S1_PREBUILT)
  add_subdirectory(include)
  set(S1_INC "${CMAKE_SOURCE_DIR}/include/internal")
  set(S1_INC_PUBLIC "${CMAKE_SOURCE_DIR}/include/s1")
  add_subdirectory(lib)
else()
  include("${S1_PREBUILT}/libs1.cmake")
  include_directories("${S1_PREBUILT}/include")
endif()
add_subdirectory(apps)

add_subdirectory(doc)

if(NOT S1_PREBUILT)
  export(TARGETS s1 FILE libs1.cmake)
  export(TARGETS s1c FILE s1c.cmake)
endif()
