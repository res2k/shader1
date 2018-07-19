set(generator_sources
    ArithmeticExpressionImpl.h
    ArrayElementExpressionImpl.h
    AssignmentExpressionImpl.h
    AttributeExpressionImpl.h
    BinaryExpressionImpl.h
    BlockImpl.h
    BoolExpressionImpl.h
    ComparisonExpressionImpl.h
    ExpressionImpl.h
    FunctionCallExpressionImpl.h
    FunctionImpl.h
    LogicExpressionImpl.h
    NumericExpressionImpl.h
    ScopeImpl.h
    TernaryExpressionImpl.h
    TypeConstructorExpressionImpl.h
    UnaryExpressionImpl.h
    VariableExpressionImpl.h
    ArithmeticExpressionImpl.cpp
    ArrayElementExpressionImpl.cpp
    AssignmentExpressionImpl.cpp
    AttributeExpressionImpl.cpp
    BinaryExpressionImpl.cpp
    BlockImpl.cpp
    BoolExpressionImpl.cpp
    ComparisonExpressionImpl.cpp
    ExpressionImpl.cpp
    FunctionCallExpressionImpl.cpp
    LogicExpressionImpl.cpp
    NumericExpressionImpl.cpp
    ScopeImpl.cpp
    TernaryExpressionImpl.cpp
    TypeConstructorExpressionImpl.cpp
    UnaryExpressionImpl.cpp
    VariableExpressionImpl.cpp
   )
s1_add_library_internal(intermediate_generator
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${generator_sources}
                        DEPENDS generate_Diagnostics_intermediate generate_Diagnostics_parser
                        LINK s1_headers_internal)
