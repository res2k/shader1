/*
    Shader1
    Copyright (c) 2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Macros for common AST test cases
 */
#ifndef PARSER_T_ASTMACROS_H_
#define PARSER_T_ASTMACROS_H_

#include <boost/lexical_cast.hpp>
#include <boost/preprocessor/cat.hpp>

// Check if an ast::Type is a well-known type
#define AST_TEST_TYPE_IS_WELL_KNOWN(TYPE, TOKEN)                                    \
  do {                                                                              \
    const auto& typeWellKnown = boost::get<ast::Type::WellKnownType> ((TYPE).value);\
    TS_ASSERT_EQUALS(typeWellKnown.size(), 1u);                                     \
    TS_ASSERT_EQUALS(typeWellKnown[0].typeOrID, s1::lexer::TOKEN);                  \
  } while (0)

// Check if an ast::Type is an unsigned well-known type
#define AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN(TYPE, TOKEN)                           \
  do {                                                                              \
    const auto& typeWellKnown = boost::get<ast::Type::WellKnownType> ((TYPE).value);\
    TS_ASSERT_EQUALS(typeWellKnown.size(), 2u);                                     \
    TS_ASSERT_EQUALS(typeWellKnown[0].typeOrID, s1::lexer::kwUnsigned);             \
    TS_ASSERT_EQUALS(typeWellKnown[1].typeOrID, s1::lexer::TOKEN);                  \
  } while (0)

// Check if an ast::Type is a vector of a well-known type
#define AST_TEST_TYPE_IS_WELL_KNOWN_VEC(TYPE, TOKEN, DIM)                           \
  do {                                                                              \
    const auto& typeWellKnown = boost::get<ast::Type::WellKnownType> ((TYPE).value);\
    TS_ASSERT_EQUALS(typeWellKnown.size(), 1u);                                     \
    TS_ASSERT_EQUALS(typeWellKnown[0].typeOrID,                                     \
                     s1::lexer::TOKEN | s1::lexer::VecFlag);                        \
    TS_ASSERT_EQUALS(typeWellKnown[0].dimension1, DIM);                             \
  } while (0)

// Check if an ast::Type is a vector of an unsigned well-known type
#define AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN_VEC(TYPE, TOKEN, DIM)                  \
  do {                                                                              \
    const auto& typeWellKnown = boost::get<ast::Type::WellKnownType> ((TYPE).value);\
    TS_ASSERT_EQUALS(typeWellKnown.size(), 2u);                                     \
    TS_ASSERT_EQUALS(typeWellKnown[0].typeOrID, s1::lexer::kwUnsigned);             \
    TS_ASSERT_EQUALS(typeWellKnown[1].typeOrID,                                     \
                     s1::lexer::TOKEN | s1::lexer::VecFlag);                        \
    TS_ASSERT_EQUALS(typeWellKnown[1].dimension1, DIM);                             \
  } while (0)

// Check if an ast::Type is a matrix of a well-known type
#define AST_TEST_TYPE_IS_WELL_KNOWN_MAT(TYPE, TOKEN, DIM1, DIM2)                    \
  do {                                                                              \
    const auto& typeWellKnown = boost::get<ast::Type::WellKnownType> ((TYPE).value);\
    TS_ASSERT_EQUALS(typeWellKnown.size(), 1u);                                     \
    TS_ASSERT_EQUALS(typeWellKnown[0].typeOrID,                                     \
                     s1::lexer::TOKEN | s1::lexer::MatFlag);                        \
    TS_ASSERT_EQUALS(typeWellKnown[0].dimension1, DIM1);                            \
    TS_ASSERT_EQUALS(typeWellKnown[0].dimension2, DIM2);                            \
  } while (0)

// Check if an ast::Type is a matrix of an unsigned well-known type
#define AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN_MAT(TYPE, TOKEN, DIM1, DIM2)           \
  do {                                                                              \
    const auto& typeWellKnown = boost::get<ast::Type::WellKnownType> ((TYPE).value);\
    TS_ASSERT_EQUALS(typeWellKnown.size(), 2u);                                     \
    TS_ASSERT_EQUALS(typeWellKnown[0].typeOrID, s1::lexer::kwUnsigned);             \
    TS_ASSERT_EQUALS(typeWellKnown[1].typeOrID,                                     \
                     s1::lexer::TOKEN | s1::lexer::MatFlag);                        \
    TS_ASSERT_EQUALS(typeWellKnown[1].dimension1, DIM1);                            \
    TS_ASSERT_EQUALS(typeWellKnown[1].dimension2, DIM2);                            \
  } while (0)

// Check if an ast::Type is an alias
#define AST_TEST_TYPE_IS_ALIAS(TYPE, IDENTIFIER)                                    \
  do {                                                                              \
    const auto& typeIdentifier = boost::get<ast::Identifier> ((TYPE).value);        \
    TS_ASSERT_EQUALS(typeIdentifier.GetString(), IDENTIFIER);                       \
  } while (0)

// Check if an ast::ExprValue is an identifier
#define AST_TEST_EXPRVALUE_IS_IDENTIFIER(EXPR_VALUE, TEST_IDENTIFIER)         \
  do {                                                                        \
    TS_ASSERT_EQUALS ((EXPR_VALUE).value.typeOrID, s1::lexer::Identifier);    \
    TS_ASSERT_EQUALS ((EXPR_VALUE).value.tokenString, TEST_IDENTIFIER);       \
  } while (0)

// Check if an ast::ExprValue is a numeric
#define AST_TEST_EXPRVALUE_IS_NUMERIC(EXPR_VALUE, TYPE, TEST_VALUE)           \
  do {                                                                        \
    TS_ASSERT_EQUALS ((EXPR_VALUE).value.typeOrID, s1::lexer::Numeric);       \
    std::string valueStr;                                                     \
    (EXPR_VALUE).value.tokenString.toUTF8String (valueStr);                   \
    TS_ASSERT_EQUALS (boost::lexical_cast<TYPE> (valueStr), TEST_VALUE);      \
  } while (0)

// Check if an ast::Expr is an identifier
#define AST_TEST_EXPR_IS_IDENTIFIER(EXPR, TEST_IDENTIFIER)                          \
  do {                                                                              \
    const auto& astExprValue = boost::get<ast::ExprValuePtr> ((EXPR).value);        \
    AST_TEST_EXPRVALUE_IS_IDENTIFIER(*astExprValue, TEST_IDENTIFIER);               \
  } while (0)

// Check if an ast::Expr is an integer numeric
#define AST_TEST_EXPR_IS_NUMERIC(EXPR, TYPE, TEST_VALUE)                            \
  do {                                                                              \
    const auto& astExprValue = boost::get<ast::ExprValuePtr> ((EXPR).value);        \
    AST_TEST_EXPRVALUE_IS_NUMERIC(*astExprValue, TYPE, TEST_VALUE);                 \
  } while (0)

#endif // PARSER_T_ASTMACROS_H_
