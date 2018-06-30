/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "base/common.h"

#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"
#include "../../semantics/t/print_semantics_BaseType.h"

BOOST_AUTO_TEST_SUITE(ParserCommonAttrTypes)

namespace
{
  class TestSemanticsHandler : public ::TestSemanticsHandler
  {
  public:
    typedef ::TestSemanticsHandler Superclass;
    typedef s1::semantics::Attribute Attribute;

    using Superclass::GetAttributeType;
  };
} // anonymous namespace

BOOST_AUTO_TEST_CASE(AttrTypeArray)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeArray;
  BOOST_CHECK_NO_THROW ((typeArray = semanticsHandler.CreateArrayType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int))));

  TestSemanticsHandler::Attribute attrLength =
    TestSemanticsHandler::Attribute::Identify (s1::uc::String ("length"));
  auto typeAttr = semanticsHandler.GetAttributeType (typeArray.get(), attrLength);
  BOOST_CHECK_EQUAL(typeAttr->base, s1::semantics::BaseType::UInt);
}

BOOST_AUTO_TEST_CASE(AttrTypeVector)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeInt4;
  BOOST_CHECK_NO_THROW ((typeInt4 = semanticsHandler.CreateVectorType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 4)));

  {
    TestSemanticsHandler::Attribute attrX =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("x"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4.get(), attrX);
    BOOST_CHECK_EQUAL(typeAttr->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrXY =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("xy"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4.get(), attrXY);
    BOOST_CHECK_EQUAL(typeAttr->typeClass, s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeAttr->vectorDim, 2u);
    s1::semantics::TypePtr typeBase = typeAttr->avmBase;
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrGA =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("ga"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4.get(), attrGA);
    BOOST_CHECK_EQUAL(typeAttr->typeClass, s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeAttr->vectorDim, 2u);
    s1::semantics::TypePtr typeBase = typeAttr->avmBase;
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrXB =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("xb"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4.get(), attrXB);
    BOOST_CHECK_EQUAL(typeAttr, s1::semantics::TypePtr ());
  }
  {
    TestSemanticsHandler::Attribute attrRZ =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("rz"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4.get(), attrRZ);
    BOOST_CHECK_EQUAL(typeAttr, s1::semantics::TypePtr ());
  }
}

BOOST_AUTO_TEST_CASE(AttrTypeMatrix)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeInt4x3;
  BOOST_CHECK_NO_THROW ((typeInt4x3 = semanticsHandler.CreateMatrixType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 4, 3)));

  {
    TestSemanticsHandler::Attribute attrCol =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("col"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4x3.get(), attrCol);
    BOOST_CHECK_EQUAL(typeAttr->typeClass, s1::semantics::Type::Array);
    s1::semantics::TypePtr typeArrayBase = typeAttr->avmBase;
    BOOST_CHECK_EQUAL(typeArrayBase->typeClass, s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeArrayBase->vectorDim, 3u);
    s1::semantics::TypePtr typeBase = typeArrayBase->avmBase;
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrRow =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("row"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4x3.get(), attrRow);
    BOOST_CHECK_EQUAL(typeAttr->typeClass, s1::semantics::Type::Array);
    s1::semantics::TypePtr typeArrayBase = typeAttr->avmBase;
    BOOST_CHECK_EQUAL(typeArrayBase->typeClass, s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeArrayBase->vectorDim, 4u);
    s1::semantics::TypePtr typeBase = typeArrayBase->avmBase;
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrTransp =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("transpose"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4x3.get(), attrTransp);
    BOOST_CHECK_EQUAL(typeAttr->typeClass, s1::semantics::Type::Matrix);
    BOOST_CHECK_EQUAL(typeAttr->matrixCols, 3u);
    BOOST_CHECK_EQUAL(typeAttr->matrixRows, 4u);
    s1::semantics::TypePtr typeBase = typeAttr->avmBase;
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrInv =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("invert"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt4x3.get(), attrInv);
    BOOST_CHECK_EQUAL(typeAttr, s1::semantics::TypePtr ());
  }
}

BOOST_AUTO_TEST_CASE(AttrTypeMatrix2)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeInt3x3;
  BOOST_CHECK_NO_THROW ((typeInt3x3 = semanticsHandler.CreateMatrixType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 3, 3)));

  {
    TestSemanticsHandler::Attribute attrInv =
      TestSemanticsHandler::Attribute::Identify (s1::uc::String ("invert"));
    auto typeAttr = semanticsHandler.GetAttributeType (typeInt3x3.get(), attrInv);
    BOOST_CHECK_EQUAL(typeAttr->typeClass, s1::semantics::Type::Matrix);
    BOOST_CHECK_EQUAL(typeAttr->matrixCols, 3u);
    BOOST_CHECK_EQUAL(typeAttr->matrixRows, 3u);
    s1::semantics::TypePtr typeBase = typeAttr->avmBase;
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
}

BOOST_AUTO_TEST_SUITE_END()
