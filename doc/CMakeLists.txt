add_custom_target(docs)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pubapi-Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile @ONLY)
  add_custom_target(doc_pubapi
    "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}" ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/pubapi-Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  add_dependencies(doc_pubapi generate_Object)
  add_dependencies(docs doc_pubapi)
endif(DOXYGEN_FOUND)
