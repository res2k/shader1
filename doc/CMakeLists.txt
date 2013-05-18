add_custom_target(docs)

find_package(Doxygen)
find_package(PythonInterp)
if(DOXYGEN_FOUND AND PYTHONINTERP_FOUND)
  add_custom_target(merge_headers
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include/s1 headers-merged/s1
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/include/s1/Object.h headers-merged/s1/
    COMMENT "Merging headers" VERBATIM
  )
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pubapi-Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile @ONLY)
  add_custom_target(doc_pubapi
    "MERGED_HEADERS=${CMAKE_CURRENT_BINARY_DIR}/headers-merged" ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  add_dependencies(doc_pubapi generate_Object merge_headers)
  add_dependencies(docs doc_pubapi)
endif()
