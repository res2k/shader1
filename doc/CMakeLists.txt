add_custom_target(docs)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(merge_headers
    ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/treelink.py ${CMAKE_SOURCE_DIR}/include/s1 headers-merged/s1
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/treelink.py ${CMAKE_BINARY_DIR}/include/s1/Object.h headers-merged/s1/Object.h
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/treelink.py ${CMAKE_BINARY_DIR}/include/s1/ResultCode.inc headers-merged/s1/ResultCode.inc
    COMMENT "Merging headers" VERBATIM
  )
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pubapi-Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile @ONLY)
  add_custom_target(doc_pubapi
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  add_dependencies(doc_pubapi generate_Object generate_ResultCode merge_headers)
  add_dependencies(docs doc_pubapi)
endif()
