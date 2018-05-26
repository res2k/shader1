add_custom_target(docs)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  file(STRINGS ${CMAKE_SOURCE_DIR}/include/s1/ResultCode_components.txt RESULTCODE_COMPONENTS)
  foreach(comp ${RESULTCODE_COMPONENTS})
    list(APPEND RESULTCODE_GENERATED ${CMAKE_BINARY_DIR}/include/s1/ResultCode_defs_${comp}.h)
  endforeach(comp)

  # Merge static+generated headers into a single directory + combine ResultCode headers
  add_custom_target(merge_headers
    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/treelink.py ${CMAKE_SOURCE_DIR}/include/s1 headers-merged/s1
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/treelink.py ${CMAKE_BINARY_DIR}/include/s1/Object.h headers-merged/s1/Object.h
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/cat.py ${CMAKE_BINARY_DIR}/include/s1/ResultCode_defs.h ${RESULTCODE_GENERATED} > headers-merged/s1/ResultCode_defs.h
    COMMENT "Merging headers" VERBATIM
  )
  add_dependencies(merge_headers generate_Object generate_ResultCode)

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pubapi-Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile @ONLY)
  add_custom_target(doc_pubapi
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  add_dependencies(doc_pubapi merge_headers)
  add_dependencies(docs doc_pubapi)

  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
          DESTINATION doc/shader1 OPTIONAL)
endif()
