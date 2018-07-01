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

BOOST_AUTO_TEST_SUITE(ParserCommonTypeCompat)

BOOST_AUTO_TEST_CASE(CompatBase)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeVoid;
  BOOST_CHECK_NO_THROW ((typeVoid = semanticsHandler.CreateType (s1::semantics::BaseType::Void)));
  s1::semantics::TypePtr typeBool;
  BOOST_CHECK_NO_THROW ((typeBool = semanticsHandler.CreateType (s1::semantics::BaseType::Bool)));
  s1::semantics::TypePtr typeInt;
  BOOST_CHECK_NO_THROW ((typeInt = semanticsHandler.CreateType (s1::semantics::BaseType::Int)));
  s1::semantics::TypePtr typeUInt;
  BOOST_CHECK_NO_THROW ((typeUInt = semanticsHandler.CreateType (s1::semantics::BaseType::UInt)));
  s1::semantics::TypePtr typeFloat;
  BOOST_CHECK_NO_THROW ((typeFloat = semanticsHandler.CreateType (s1::semantics::BaseType::Float)));

  // Void - not even compatible to itself
  {
    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossless (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossy (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeVoid->IsPrecisionHigherEqual (*typeVoid), false);

    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossless (*typeBool), false);
    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossy (*typeBool), false);
    BOOST_CHECK_EQUAL (typeVoid->IsPrecisionHigherEqual (*typeBool), false);

    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossless (*typeInt), false);
    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossy (*typeInt), false);
    BOOST_CHECK_EQUAL (typeVoid->IsPrecisionHigherEqual (*typeInt), false);

    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossless (*typeUInt), false);
    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossy (*typeUInt), false);
    BOOST_CHECK_EQUAL (typeVoid->IsPrecisionHigherEqual (*typeUInt), false);

    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossless (*typeFloat), false);
    BOOST_CHECK_EQUAL (typeVoid->CompatibleLossy (*typeFloat), false);
    BOOST_CHECK_EQUAL (typeVoid->IsPrecisionHigherEqual (*typeFloat), false);
  }
  // Bool - only compatible to itself
  {
    BOOST_CHECK_EQUAL (typeBool->CompatibleLossless (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeBool->CompatibleLossy (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeBool->IsPrecisionHigherEqual (*typeVoid), false);

    BOOST_CHECK_EQUAL (typeBool->CompatibleLossless (*typeBool), true);
    BOOST_CHECK_EQUAL (typeBool->CompatibleLossy (*typeBool), true);
    BOOST_CHECK_EQUAL (typeBool->IsPrecisionHigherEqual (*typeBool), true);

    BOOST_CHECK_EQUAL (typeBool->CompatibleLossless (*typeInt), false);
    BOOST_CHECK_EQUAL (typeBool->CompatibleLossy (*typeInt), false);
    BOOST_CHECK_EQUAL (typeBool->IsPrecisionHigherEqual (*typeInt), false);

    BOOST_CHECK_EQUAL (typeBool->CompatibleLossless (*typeUInt), false);
    BOOST_CHECK_EQUAL (typeBool->CompatibleLossy (*typeUInt), false);
    BOOST_CHECK_EQUAL (typeBool->IsPrecisionHigherEqual (*typeUInt), false);

    BOOST_CHECK_EQUAL (typeBool->CompatibleLossless (*typeFloat), false);
    BOOST_CHECK_EQUAL (typeBool->CompatibleLossy (*typeFloat), false);
    BOOST_CHECK_EQUAL (typeBool->IsPrecisionHigherEqual (*typeFloat), false);
  }
  // Int - compatible to other numeric types
  {
    BOOST_CHECK_EQUAL (typeInt->CompatibleLossless (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeInt->CompatibleLossy (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeInt->IsPrecisionHigherEqual (*typeVoid), false);

    BOOST_CHECK_EQUAL (typeInt->CompatibleLossless (*typeBool), false);
    BOOST_CHECK_EQUAL (typeInt->CompatibleLossy (*typeBool), false);
    BOOST_CHECK_EQUAL (typeInt->IsPrecisionHigherEqual (*typeBool), false);

    BOOST_CHECK_EQUAL (typeInt->CompatibleLossless (*typeInt), true);
    BOOST_CHECK_EQUAL (typeInt->CompatibleLossy (*typeInt), true);
    BOOST_CHECK_EQUAL (typeInt->IsPrecisionHigherEqual (*typeInt), true);

    BOOST_CHECK_EQUAL (typeInt->CompatibleLossless (*typeUInt), true);
    BOOST_CHECK_EQUAL (typeInt->CompatibleLossy (*typeUInt), true);
    BOOST_CHECK_EQUAL (typeInt->IsPrecisionHigherEqual (*typeUInt), true);

    BOOST_CHECK_EQUAL (typeInt->CompatibleLossless (*typeFloat), true);
    BOOST_CHECK_EQUAL (typeInt->CompatibleLossy (*typeFloat), true);
    // int is lower prec than float
    BOOST_CHECK_EQUAL (typeInt->IsPrecisionHigherEqual (*typeFloat), false);
  }
  // UInt - compatible to other numeric types
  {
    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossless (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossy (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeUInt->IsPrecisionHigherEqual (*typeVoid), false);

    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossless (*typeBool), false);
    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossy (*typeBool), false);
    BOOST_CHECK_EQUAL (typeUInt->IsPrecisionHigherEqual (*typeBool), false);

    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossless (*typeInt), true);
    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossy (*typeInt), true);
    // uint, despite "lossless" assignable to, is lower prec than int!
    BOOST_CHECK_EQUAL (typeUInt->IsPrecisionHigherEqual (*typeInt), false);

    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossless (*typeUInt), true);
    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossy (*typeUInt), true);
    BOOST_CHECK_EQUAL (typeUInt->IsPrecisionHigherEqual (*typeUInt), true);

    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossless (*typeFloat), true);
    BOOST_CHECK_EQUAL (typeUInt->CompatibleLossy (*typeFloat), true);
    // uint is lower prec than float
    BOOST_CHECK_EQUAL (typeUInt->IsPrecisionHigherEqual (*typeFloat), false);
  }
  // Float - compatible to other numeric types
  {
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossless (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossy (*typeVoid), false);
    BOOST_CHECK_EQUAL (typeFloat->IsPrecisionHigherEqual (*typeVoid), false);
    
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossless (*typeBool), false);
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossy (*typeBool), false);
    BOOST_CHECK_EQUAL (typeFloat->IsPrecisionHigherEqual (*typeBool), false);

    // float -> int not lossless
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossless (*typeInt), false);
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossy (*typeInt), true);
    // float is higher prec than int
    BOOST_CHECK_EQUAL (typeFloat->IsPrecisionHigherEqual (*typeInt), true);

    // float -> uint not lossless
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossless (*typeUInt), false);
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossy (*typeUInt), true);
    // float is higher prec than uint
    BOOST_CHECK_EQUAL (typeFloat->IsPrecisionHigherEqual (*typeUInt), true);

    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossless (*typeFloat), true);
    BOOST_CHECK_EQUAL (typeFloat->CompatibleLossy (*typeFloat), true);
    BOOST_CHECK_EQUAL (typeFloat->IsPrecisionHigherEqual (*typeFloat), true);
  }
}

BOOST_AUTO_TEST_CASE(HigherPrecision)
{
  using namespace s1::parser;

  TestSemanticsHandler semanticsHandler;

  s1::semantics::TypePtr typeVoid;
  BOOST_CHECK_NO_THROW ((typeVoid = semanticsHandler.CreateType (s1::semantics::BaseType::Void)));
  s1::semantics::TypePtr typeBool;
  BOOST_CHECK_NO_THROW ((typeBool = semanticsHandler.CreateType (s1::semantics::BaseType::Bool)));
  s1::semantics::TypePtr typeInt;
  BOOST_CHECK_NO_THROW ((typeInt = semanticsHandler.CreateType (s1::semantics::BaseType::Int)));
  s1::semantics::TypePtr typeUInt;
  BOOST_CHECK_NO_THROW ((typeUInt = semanticsHandler.CreateType (s1::semantics::BaseType::UInt)));
  s1::semantics::TypePtr typeFloat;
  BOOST_CHECK_NO_THROW ((typeFloat = semanticsHandler.CreateType (s1::semantics::BaseType::Float)));

  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeVoid.get(), typeBool.get()), nullptr);
  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeVoid.get(), typeInt.get()), nullptr);
  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeVoid.get(), typeUInt.get()), nullptr);
  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeVoid.get(), typeFloat.get()), nullptr);

  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeBool.get(), typeInt.get()), nullptr);
  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeBool.get(), typeUInt.get()), nullptr);
  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeBool.get(), typeFloat.get()), nullptr);

  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeInt.get(), typeUInt.get()), typeInt.get());
  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeInt.get(), typeFloat.get()), typeFloat.get());

  BOOST_CHECK_EQUAL (s1::semantics::Type::GetHigherPrecisionType (typeUInt.get(), typeFloat.get()), typeFloat.get());
}

BOOST_AUTO_TEST_SUITE_END()
