# Generate diagnostics headers
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS Diagnostics_components.txt)
file(STRINGS Diagnostics_components.txt DIAGNOSTICS_COMPS)

add_custom_target(generate_Diagnostics)

set(DIAGNOSTICS_DEFS_TPL ${CMAKE_CURRENT_SOURCE_DIR}/diagnostics/Diagnostics_defs.h.tpl)
set(DIAGNOSTICS_FWD_TPL ${CMAKE_CURRENT_SOURCE_DIR}/diagnostics/Diagnostics_fwd.h.tpl)
foreach(comp ${DIAGNOSTICS_COMPS})
  add_custom_command(OUTPUT ${comp}/Diagnostics.h
    COMMAND ${CMAKE_COMMAND} -E make_directory ${comp}
    COMMAND ${CMAKE_COMMAND} -E env PYTHONHASHSEED=423
        ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
        ${CMAKE_CURRENT_SOURCE_DIR}/${comp}/Diagnostics.xml
        -r components
        -D namespace=${comp}
        -t ${DIAGNOSTICS_DEFS_TPL} > ${comp}/Diagnostics.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${comp}/Diagnostics.xml ${DIAGNOSTICS_DEFS_TPL})
  add_custom_command(OUTPUT ${comp}/Diagnostics_fwd.h
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/build/generate_codes_table.py
        -D namespace=${comp}
        -t ${DIAGNOSTICS_FWD_TPL} > ${comp}/Diagnostics_fwd.h
    DEPENDS ${DIAGNOSTICS_FWD_TPL})
  add_custom_target(generate_Diagnostics_${comp} DEPENDS ${comp}/Diagnostics.h ${comp}/Diagnostics_fwd.h
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/generate_Diagnostics_${comp}")
  add_dependencies(generate_Diagnostics generate_Diagnostics_${comp})
  list(APPEND DIAGNOSTICS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${comp}/Diagnostics.xml)
endforeach()
