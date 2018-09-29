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
    typedef Superclass::Attribute Attribute;

    using Superclass::IdentifyAttribute;
    using Superclass::GetAttributeType;
  };
} // anonymous namespace

BOOST_AUTO_TEST_CASE(AttrTypeArray)
{
  using namespace s1::parser;

  typedef TestSemanticsHandler::TestType TestType;
  typedef boost::intrusive_ptr<TestType> TestTypePtr;

  TestSemanticsHandler semanticsHandler;

  TestTypePtr typeArray;
  BOOST_CHECK_NO_THROW ((typeArray = boost::static_pointer_cast<TestType> (
    semanticsHandler.CreateArrayType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int)))));

  TestSemanticsHandler::Attribute attrLength =
    TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("length"));
  TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeArray.get(), attrLength));
  BOOST_CHECK_EQUAL(typeAttr->base, s1::semantics::BaseType::UInt);
}

BOOST_AUTO_TEST_CASE(AttrTypeVector)
{
  using namespace s1::parser;

  typedef TestSemanticsHandler::TestType TestType;
  typedef boost::intrusive_ptr<TestType> TestTypePtr;

  TestSemanticsHandler semanticsHandler;

  TestTypePtr typeInt4;
  BOOST_CHECK_NO_THROW ((typeInt4 = boost::static_pointer_cast<TestType> (
    semanticsHandler.CreateVectorType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 4))));

  {
    TestSemanticsHandler::Attribute attrX =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("x"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4.get(), attrX));
    BOOST_CHECK_EQUAL(typeAttr->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrXY =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("xy"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4.get(), attrXY));
    BOOST_CHECK_EQUAL(typeAttr->typeClass, TestType::Vector);
    BOOST_CHECK_EQUAL(typeAttr->vectorDim, 2u);
    TestTypePtr typeBase = boost::static_pointer_cast<TestType> (typeAttr->avmBase);
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrGA =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("ga"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4.get(), attrGA));
    BOOST_CHECK_EQUAL(typeAttr->typeClass, TestType::Vector);
    BOOST_CHECK_EQUAL(typeAttr->vectorDim, 2u);
    TestTypePtr typeBase = boost::static_pointer_cast<TestType> (typeAttr->avmBase);
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrXB =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("xb"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4.get(), attrXB));
    BOOST_CHECK_EQUAL(typeAttr, TestTypePtr ());
  }
  {
    TestSemanticsHandler::Attribute attrRZ =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("rz"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4.get(), attrRZ));
    BOOST_CHECK_EQUAL(typeAttr, TestTypePtr ());
  }
}

BOOST_AUTO_TEST_CASE(AttrTypeMatrix)
{
  using namespace s1::parser;

  typedef TestSemanticsHandler::TestType TestType;
  typedef boost::intrusive_ptr<TestType> TestTypePtr;

  TestSemanticsHandler semanticsHandler;

  TestTypePtr typeInt4x3;
  BOOST_CHECK_NO_THROW ((typeInt4x3 = boost::static_pointer_cast<TestType> (
    semanticsHandler.CreateMatrixType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 4, 3))));

  {
    TestSemanticsHandler::Attribute attrCol =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("col"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4x3.get(), attrCol));
    BOOST_CHECK_EQUAL(typeAttr->typeClass, TestType::Array);
    TestTypePtr typeArrayBase = boost::static_pointer_cast<TestType> (typeAttr->avmBase);
    BOOST_CHECK_EQUAL(typeArrayBase->typeClass, TestType::Vector);
    BOOST_CHECK_EQUAL(typeArrayBase->vectorDim, 3u);
    TestTypePtr typeBase = boost::static_pointer_cast<TestType> (typeArrayBase->avmBase);
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrRow =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("row"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4x3.get(), attrRow));
    BOOST_CHECK_EQUAL(typeAttr->typeClass, TestType::Array);
    TestTypePtr typeArrayBase = boost::static_pointer_cast<TestType> (typeAttr->avmBase);
    BOOST_CHECK_EQUAL(typeArrayBase->typeClass, TestType::Vector);
    BOOST_CHECK_EQUAL(typeArrayBase->vectorDim, 4u);
    TestTypePtr typeBase = boost::static_pointer_cast<TestType> (typeArrayBase->avmBase);
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrTransp =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("transpose"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4x3.get(), attrTransp));
    BOOST_CHECK_EQUAL(typeAttr->typeClass, TestType::Matrix);
    BOOST_CHECK_EQUAL(typeAttr->matrixCols, 3u);
    BOOST_CHECK_EQUAL(typeAttr->matrixRows, 4u);
    TestTypePtr typeBase = boost::static_pointer_cast<TestType> (typeAttr->avmBase);
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
  {
    TestSemanticsHandler::Attribute attrInv =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("invert"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt4x3.get(), attrInv));
    BOOST_CHECK_EQUAL(typeAttr, TestTypePtr ());
  }
}

BOOST_AUTO_TEST_CASE(AttrTypeMatrix2)
{
  using namespace s1::parser;

  typedef TestSemanticsHandler::TestType TestType;
  typedef boost::intrusive_ptr<TestType> TestTypePtr;

  TestSemanticsHandler semanticsHandler;

  TestTypePtr typeInt3x3;
  BOOST_CHECK_NO_THROW ((typeInt3x3 = boost::static_pointer_cast<TestType> (
    semanticsHandler.CreateMatrixType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 3, 3))));

  {
    TestSemanticsHandler::Attribute attrInv =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("invert"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
    semanticsHandler.GetAttributeType (typeInt3x3.get(), attrInv));
    BOOST_CHECK_EQUAL(typeAttr->typeClass, TestType::Matrix);
    BOOST_CHECK_EQUAL(typeAttr->matrixCols, 3u);
    BOOST_CHECK_EQUAL(typeAttr->matrixRows, 3u);
    TestTypePtr typeBase = boost::static_pointer_cast<TestType> (typeAttr->avmBase);
    BOOST_CHECK_EQUAL(typeBase->base, s1::semantics::BaseType::Int);
  }
}

BOOST_AUTO_TEST_SUITE_END()
