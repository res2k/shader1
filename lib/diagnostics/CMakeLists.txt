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
  add_custom_command(OUTPUT DescriptionString_${comp}.cpp
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
        ${DIAGNOSTICS_XML}
        -r components
        -D namespace=${comp}
        -t ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionString_comp.cpp.tpl > DescriptionString_${comp}.cpp
    DEPENDS ${DIAGNOSTICS_XML} ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionString_comp.cpp.tpl)

  list(APPEND DIAGNOSTICS_FILES ${DIAGNOSTICS_XML})
  list(APPEND DIAGNOSTICS_GENERATED_SRC IdString_${comp}.cpp DescriptionString_${comp}.cpp)
endforeach()

add_custom_command(OUTPUT IdString.cpp
                   COMMAND ${CMAKE_COMMAND} -E env PYTHONHASHSEED=423
                      ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                      ${DIAGNOSTICS_FILES}
                      -r components
                      -t ${CMAKE_CURRENT_SOURCE_DIR}/IdString.cpp.tpl > IdString.cpp
                   DEPENDS ${DIAGNOSTICS_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/IdString.cpp.tpl)
add_custom_command(OUTPUT DescriptionString.cpp
                   COMMAND ${CMAKE_COMMAND} -E env PYTHONHASHSEED=423
                      ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
                      ${DIAGNOSTICS_FILES}
                      -r components
                      -t ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionString.cpp.tpl > DescriptionString.cpp
                   DEPENDS ${DIAGNOSTICS_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/DescriptionString.cpp.tpl)

set(diagnostics_sources
    DescriptionString.cpp
    IdString.cpp
    ${DIAGNOSTICS_GENERATED_SRC}
    ${S1_INC}/diagnostics/common.h
   )
s1_add_library_internal(diagnostics
                        SOURCES ${diagnostics_sources}
                        LINK base)
add_dependencies(diagnostics generate_Diagnostics)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(diagnostics PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()
