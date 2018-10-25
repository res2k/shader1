set(optimize_sources
    CloningHelper.h
    CommonSequenceVisitor.h
    OptimizeSequenceConstantFolding.h
    OptimizeSequenceDeadCodeEliminate.h
    OptimizeSequenceInlineBlocks.h
    OptimizeSequenceStep.h
    ProgramOptimizations.h
    SequenceOptimizations.h
    CloningHelper.cpp
    ConstantFolding.cpp
    DeadCodeElimination.cpp
    Inliner.cpp
    Optimizer.cpp
    OptimizeSequenceConstantFolding.cpp
    OptimizeSequenceDeadCodeEliminate.cpp
    OptimizeSequenceInlineBlocks.cpp
    ProgramOptimizations.cpp
    SequenceOptimizations.cpp
    UnusedFunctionArgumentRemoval.cpp
    ${S1_INC}/optimize/ConstantFolding.h
    ${S1_INC}/optimize/DeadCodeElimination.h
    ${S1_INC}/optimize/Inliner.h
    ${S1_INC}/optimize/Optimizer.h
    ${S1_INC}/optimize/UnusedFunctionArgumentRemoval.h
    )
s1_add_library_internal(optimize
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${optimize_sources}
                        LINK intermediate)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(optimize PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

s1_add_boost_test(test_Optimize
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/ConstantFolding.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/DeadCodeElimination.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/InlineBlock.cpp
                  DEPENDS optimize)
set_property(TARGET test_Optimize
             APPEND
             PROPERTY INCLUDE_DIRECTORIES
             ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate
             ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate/generator)
