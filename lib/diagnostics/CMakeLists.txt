set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS Diagnostics_components.txt)
file(STRINGS ${CMAKE_SOURCE_DIR}/include/internal/Diagnostics_components.txt DIAGNOSTICS_COMPS)

foreach(comp ${DIAGNOSTICS_COMPS})
  set(DIAGNOSTICS_XML "${CMAKE_SOURCE_DIR}/include/internal/${comp}/Diagnostics.xml")
  add_custom_command(OUTPUT IdString_${comp}.cpp
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
        ${DIAGNOSTICS_XML}
        -r components
        -D namespace=${comp}
        -t ${CMAKE_CURRENT_SOURCE_DIR}/IdString_comp.cpp.tpl > IdString_${comp}.cpp
    DEPENDS ${DIAGNOSTICS_XML} ${CMAKE_CURRENT_SOURCE_DIR}/IdString_comp.cpp.tpl)

  list(APPEND DIAGNOSTICS_FILES ${DIAGNOSTICS_XML})
  list(APPEND DIAGNOSTICS_IDSTRING_SRC IdString_${comp}.cpp)
endforeach()

add_custom_command(OUTPUT IdString.cpp
                   COMMAND ${CMAKE_COMMAND} -E env PYTHONHASHSEED=423
                      ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                      ${DIAGNOSTICS_FILES}
                      -r components
                      -t ${CMAKE_CURRENT_SOURCE_DIR}/IdString.cpp.tpl > IdString.cpp
                   DEPENDS ${DIAGNOSTICS_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/IdString.cpp.tpl)

set(diagnostics_sources
    IdString.cpp
    ${DIAGNOSTICS_IDSTRING_SRC}
    ${S1_INC}/diagnostics/common.h
   )
s1_add_library(diagnostics
               SOURCES ${diagnostics_sources}
               LINK base)
add_dependencies(diagnostics generate_Diagnostics)
IF(S1LIB_PDB)
  target_compile_options(diagnostics PRIVATE ${S1LIB_PDB})
ENDIF()
