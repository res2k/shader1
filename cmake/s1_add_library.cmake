function(S1_GET_DEPENDENCIES output_sources output_libs output_targets)
  set(dep_obj "")
  SET(dep_other "")
  set(dep_targets "")
  foreach(LIB ${ARGN})
    if(TARGET ${LIB})
      GET_TARGET_PROPERTY(LIBTYPE ${LIB} TYPE)
      IF("${LIBTYPE}" STREQUAL "OBJECT_LIBRARY")
        list(APPEND dep_obj $<TARGET_OBJECTS:${LIB}>)
        get_target_property(lib_depends_obj ${LIB} S1_LINK_LIBS)
        if(NOT "${lib_depends_obj}" STREQUAL "lib_depends_obj-NOTFOUND")
          S1_GET_DEPENDENCIES(extra_objs extra_deps extra_targets ${lib_depends_obj})
          list(APPEND dep_obj ${extra_objs})
          list(APPEND dep_other ${extra_deps})
          list(APPEND dep_targets ${extra_targets})
        endif()
      ELSE()
        LIST(APPEND dep_other ${LIB})
      ENDIF()
      IF(NOT "${LIBTYPE}" STREQUAL "INTERFACE_LIBRARY")
        get_target_property(lib_depends_targets ${LIB} S1_LIB_DEPENDS)
      ENDIF()
      if(NOT "${lib_depends_targets}" STREQUAL "lib_depends_targets-NOTFOUND")
        list(APPEND dep_targets ${lib_depends_targets})
      endif()
    else()
      list(APPEND dep_other ${LIB})
    endif()
  endforeach()
  list(REMOVE_DUPLICATES dep_obj)
  list(REMOVE_DUPLICATES dep_other)
  list(REMOVE_DUPLICATES dep_targets)
  set(${output_sources} ${dep_obj} PARENT_SCOPE)
  set(${output_libs} ${dep_other} PARENT_SCOPE)
  set(${output_targets} ${dep_targets} PARENT_SCOPE)
endfunction()

FUNCTION(s1_add_library TARGET)
  CMAKE_PARSE_ARGUMENTS(S1_LIB
    "STATIC;SHARED"
    ""
    "SOURCES;DEPENDS;LINK"
    ${ARGN})
  IF(S1_LIB_UNPARSED_ARGS)
    MESSAGE(FATAL_ERROR "Unexpected arguments to s1_add_library: ${S1_LIB_UNPARSED_ARGS}")
  ENDIF()
  IF(S1_LIB_STATIC AND S1_LIB_SHARED)
    MESSAGE(FATAL_ERROR "s1_add_library supports STATIC or SHARED or neither, but not both")
  ENDIF()
  IF(S1_LIB_STATIC OR S1_LIB_SHARED)
    S1_GET_DEPENDENCIES(DEPS_OBJ DEPS_LINK DEPS_TARGETS ${S1_LIB_LINK})
    if(S1_LIB_STATIC)
      ADD_LIBRARY(${TARGET} STATIC ${S1_LIB_SOURCES} ${DEPS_OBJ})
    else()
      ADD_LIBRARY(${TARGET} SHARED ${S1_LIB_SOURCES} ${DEPS_OBJ})
    endif()
    IF(DEPS_LINK)
      TARGET_LINK_LIBRARIES(${TARGET} PRIVATE ${DEPS_LINK})
    ENDIF()
    if(DEPS_TARGETS)
      ADD_DEPENDENCIES(${TARGET} ${DEPS_TARGETS})
    endif()
    if(S1_REQUIRED_CXX_FEATURES)
      target_compile_features(${TARGET} PRIVATE ${S1_REQUIRED_CXX_FEATURES})
    endif()
  ELSE()
    ADD_LIBRARY(${TARGET} OBJECT ${S1_LIB_SOURCES})
    # Save dependent libs and targets so S1_GET_DEPENDENCIES can pick them up
    SET_PROPERTY(TARGET ${TARGET} PROPERTY S1_LINK_LIBS ${S1_LIB_LINK})
    SET_PROPERTY(TARGET ${TARGET} PROPERTY S1_LIB_DEPENDS ${S1_LIB_DEPENDS})
    S1_GET_DEPENDENCIES(DEPS_OBJ DEPS_LINK DEPS_TARGETS ${TARGET})
    IF(DEPS_LINK)
      foreach(link_lib ${DEPS_LINK})
        # Forward include dirs & defines from lib dependencies
        target_compile_definitions(${TARGET} PRIVATE $<TARGET_PROPERTY:${link_lib},INTERFACE_COMPILE_DEFINITIONS>)
        target_include_directories(${TARGET} PRIVATE $<TARGET_PROPERTY:${link_lib},INTERFACE_INCLUDE_DIRECTORIES>)
      endforeach()
    ENDIF()
    if(DEPS_TARGETS)
      ADD_DEPENDENCIES(${TARGET} ${DEPS_TARGETS})
    endif()
    if(S1_REQUIRED_CXX_FEATURES)
      target_compile_features(${TARGET} PUBLIC ${S1_REQUIRED_CXX_FEATURES})
    endif()
  ENDIF()
  IF(S1_LIB_DEPENDS)
    ADD_DEPENDENCIES(${TARGET} ${S1_LIB_DEPENDS})
    SET_PROPERTY(TARGET ${TARGET} PROPERTY S1_LIB_DEPENDS ${S1_LIB_DEPENDS})
  ENDIF()
ENDFUNCTION()
