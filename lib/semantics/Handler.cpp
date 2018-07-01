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

#include "base/common.h"
#include "base/intrusive_ptr.h"
#include "semantics/Handler.h"
#include "semantics/Type.h"

namespace s1
{
  namespace semantics
  {
    Handler::Handler()
    {
      invalidType = new Type (BaseType::Invalid);
      voidType = new Type (BaseType::Void);
      boolType = new Type (BaseType::Bool);
      intType = new Type (BaseType::Int);
      uintType = new Type (BaseType::UInt);
      floatType = new Type (BaseType::Float);
    }

    Handler::~Handler() {}

    TypePtr Handler::CreateType (BaseType type)
    {
      switch (type)
      {
      case BaseType::Invalid: return invalidType;
      case BaseType::Void: return voidType;
      case BaseType::Bool: return boolType;
      case BaseType::Int: return intType;
      case BaseType::UInt: return uintType;
      case BaseType::Float: return floatType;
      }
      S1_ASSERT_NOT_REACHED (TypePtr());
    }

    TypePtr Handler::CreateSamplerType (SamplerType dim)
    {
      return make_intrusive<Type> (dim);
    }

    TypePtr Handler::CreateArrayType (TypePtr baseType)
    {
      return make_intrusive<Type> (baseType);
    }

    TypePtr Handler::CreateVectorType (TypePtr baseType, unsigned int components)
    {
      S1_ASSERT(baseType->GetTypeClass() == Type::Base, nullptr);
      return make_intrusive<Type> (baseType->GetBaseType(), components);
    }

    TypePtr Handler::CreateMatrixType (TypePtr baseType,
                                       unsigned int columns,
                                       unsigned int rows)
    {
      S1_ASSERT(baseType->GetTypeClass() == Type::Base, nullptr);
      return make_intrusive<Type> (baseType->GetBaseType(), columns, rows);
    }

  } // namespace semantics
} // namespace s1
