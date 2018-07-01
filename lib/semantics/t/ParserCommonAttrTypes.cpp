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

BOOST_AUTO_TEST_CASE(AttrTypeArray)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeArray;
  BOOST_CHECK_NO_THROW ((typeArray = semanticsHandler.CreateArrayType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int))));

  s1::semantics::Attribute attrLength =
    s1::semantics::Attribute::Identify (s1::uc::String ("length"));
  auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeArray.get(), attrLength);
  BOOST_CHECK_EQUAL(typeAttr->GetBaseType(), s1::semantics::BaseType::UInt);
}

BOOST_AUTO_TEST_CASE(AttrTypeVector)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeInt4;
  BOOST_CHECK_NO_THROW ((typeInt4 = semanticsHandler.CreateVectorType (
      semanticsHandler.CreateType (s1::semantics::BaseType::Int), 4)));

  {
    s1::semantics::Attribute attrX =
      s1::semantics::Attribute::Identify (s1::uc::String ("x"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4.get(), attrX);
    BOOST_CHECK_EQUAL(typeAttr->GetBaseType(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::Attribute attrXY =
      s1::semantics::Attribute::Identify (s1::uc::String ("xy"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4.get(), attrXY);
    BOOST_CHECK_EQUAL(typeAttr->GetTypeClass(), s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeAttr->GetVectorTypeComponents(), 2u);
    BOOST_CHECK_EQUAL(typeAttr->GetVMBase(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::Attribute attrGA =
      s1::semantics::Attribute::Identify (s1::uc::String ("ga"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4.get(), attrGA);
    BOOST_CHECK_EQUAL(typeAttr->GetTypeClass(), s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeAttr->GetVectorTypeComponents(), 2u);
    BOOST_CHECK_EQUAL(typeAttr->GetVMBase(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::Attribute attrXB =
      s1::semantics::Attribute::Identify (s1::uc::String ("xb"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4.get(), attrXB);
    BOOST_CHECK_EQUAL(typeAttr, s1::semantics::TypePtr ());
  }
  {
    s1::semantics::Attribute attrRZ =
      s1::semantics::Attribute::Identify (s1::uc::String ("rz"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4.get(), attrRZ);
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
    s1::semantics::Attribute attrCol =
      s1::semantics::Attribute::Identify (s1::uc::String ("col"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4x3.get(), attrCol);
    BOOST_CHECK_EQUAL(typeAttr->GetTypeClass(), s1::semantics::Type::Array);
    s1::semantics::TypePtr typeArrayBase = typeAttr->GetArrayBase();
    BOOST_CHECK_EQUAL(typeArrayBase->GetTypeClass(), s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeArrayBase->GetVectorTypeComponents(), 3u);
    BOOST_CHECK_EQUAL(typeArrayBase->GetVMBase(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::Attribute attrRow =
      s1::semantics::Attribute::Identify (s1::uc::String ("row"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4x3.get(), attrRow);
    BOOST_CHECK_EQUAL(typeAttr->GetTypeClass(), s1::semantics::Type::Array);
    s1::semantics::TypePtr typeArrayBase = typeAttr->GetArrayBase();
    BOOST_CHECK_EQUAL(typeArrayBase->GetTypeClass(), s1::semantics::Type::Vector);
    BOOST_CHECK_EQUAL(typeArrayBase->GetVectorTypeComponents(), 4u);
    BOOST_CHECK_EQUAL(typeArrayBase->GetVMBase(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::Attribute attrTransp =
      s1::semantics::Attribute::Identify (s1::uc::String ("transpose"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4x3.get(), attrTransp);
    BOOST_CHECK_EQUAL(typeAttr->GetTypeClass(), s1::semantics::Type::Matrix);
    BOOST_CHECK_EQUAL(typeAttr->GetMatrixTypeCols(), 3u);
    BOOST_CHECK_EQUAL(typeAttr->GetMatrixTypeRows(), 4u);
    BOOST_CHECK_EQUAL(typeAttr->GetVMBase(), s1::semantics::BaseType::Int);
  }
  {
    s1::semantics::Attribute attrInv =
      s1::semantics::Attribute::Identify (s1::uc::String ("invert"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt4x3.get(), attrInv);
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
    s1::semantics::Attribute attrInv =
      s1::semantics::Attribute::Identify (s1::uc::String ("invert"));
    auto typeAttr = s1::semantics::Attribute::GetType (&semanticsHandler, typeInt3x3.get(), attrInv);
    BOOST_CHECK_EQUAL(typeAttr->GetTypeClass(), s1::semantics::Type::Matrix);
    BOOST_CHECK_EQUAL(typeAttr->GetMatrixTypeCols(), 3u);
    BOOST_CHECK_EQUAL(typeAttr->GetMatrixTypeRows(), 3u);
    BOOST_CHECK_EQUAL(typeAttr->GetVMBase(), s1::semantics::BaseType::Int);
  }
}

BOOST_AUTO_TEST_SUITE_END()
