add_definitions(-DS1_BUILD)
set(S1_REQUIRED_CXX_FEATURES
  ${ALL_REQUIRED_CXX_FEATURES}
  $<TARGET_PROPERTY:outcome::hl,INTERFACE_COMPILE_FEATURES>)

if(MSVC)
  # Disable some warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4355")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4800")
endif()

if(MSVC AND S1_STATIC_CRT AND BUILD_SHARED_LIBS)
  s1_set_static_msvc_runtime()
endif()
if(MSVC AND NOT BUILD_SHARED_LIBS)
  # Static lib merging doesn't merge debug info, so manually set up a single
  # PDB. For convenience, place it in the same dir the lib is put
  set(s1_outputname "$<TARGET_PROPERTY:s1,OUTPUT_NAME>")
  set(s1_logicalname "$<TARGET_PROPERTY:s1,NAME>")
  set(s1_actualoutput "$<$<BOOL:${s1_outputname}>:${s1_outputname}>$<$<NOT:$<BOOL:${s1_outputname}>>:${s1_logicalname}>")
  set(S1LIB_PDB_PATH "${CMAKE_BINARY_DIR}/$<CONFIG>/${s1_actualoutput}.pdb")
  set(S1LIB_PDB_CXX_FLAGS "/Fd${S1LIB_PDB_PATH}" ${CXX_PDB_SYNC})
endif()

include_directories(${CMAKE_SOURCE_DIR}/outcome/include)

if(BUILD_SHARED_LIBS)
  set(S1_TARGET_LINK SHARED)
else()
  set(S1_TARGET_LINK STATIC)
endif()

add_subdirectory(base)
add_subdirectory(diagnostics)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(intermediate)
add_subdirectory(codegen)
add_subdirectory(splitter)
add_subdirectory(optimize)
add_subdirectory(compiler)
add_subdirectory(API)
