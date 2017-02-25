/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "Builtin.h"
#include "ScopeImpl.h"
#include "TypeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::Scope::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters1 (const TypePtr& type)
    {
      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameters params;

      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameter fp;
      fp.type = type;
      fp.identifier = uc::String ("a");
      fp.dir = IntermediateGeneratorSemanticsHandler::ScopeImpl::dirIn;
      params.push_back (fp);

      return params;
    }

    IntermediateGeneratorSemanticsHandler::Scope::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters2 (const TypePtr& type)
    {
      return MakeFormalParameters2 (type, type);
    }

    IntermediateGeneratorSemanticsHandler::Scope::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters2 (const TypePtr& typeA, const TypePtr& typeB)
    {
      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameters params;

      IntermediateGeneratorSemanticsHandler::ScopeImpl::FunctionFormalParameter fp;
      fp.type = typeA;
      fp.identifier = uc::String ("a");
      fp.dir = IntermediateGeneratorSemanticsHandler::ScopeImpl::dirIn;
      params.push_back (fp);

      fp.type = typeB;
      fp.identifier = uc::String ("b");
      params.push_back (fp);

      return params;
    }

    void IntermediateGeneratorSemanticsHandler::SetupBuiltins (const ScopeImplPtr& scope)
    {
      // TODO: Check if less functions can be used; e.g. instead of all the overloads for vector types, let casts do the work

      TypePtr vecTypeInt[5];
      TypePtr vecTypeUInt[5];
      TypePtr vecTypeFloat[5];
      for (unsigned int c = 1; c < 5; c++)
      {
        vecTypeInt[c] = CreateVectorType (intType, c);
        vecTypeUInt[c] = CreateVectorType (uintType, c);
        vecTypeFloat[c] = CreateVectorType (floatType, c);
      }

      TypePtr matTypeInt[5][5];
      TypePtr matTypeUInt[5][5];
      TypePtr matTypeFloat[5][5];
      for (unsigned int c = 1; c < 5; c++)
      {
        for (unsigned int r = 1; r < 5; r++)
        {
          matTypeInt[r][c] = CreateMatrixType (intType, c, r);
          matTypeUInt[r][c] = CreateMatrixType (uintType, c, r);
          matTypeFloat[r][c] = CreateMatrixType (floatType, c, r);
        }
      }

      scope->AddBuiltinFunction (new Builtin (intermediate::pow,
                                              floatType,
                                              uc::String ("pow"),
                                              MakeFormalParameters2 (floatType)));

      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (intermediate::dot,
                                                intType,
                                                uc::String ("dot"),
                                                MakeFormalParameters2 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::dot,
                                                uintType,
                                                uc::String ("dot"),
                                                MakeFormalParameters2 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::dot,
                                                floatType,
                                                uc::String ("dot"),
                                                MakeFormalParameters2 (vecTypeFloat[c])));
      }
      scope->AddBuiltinFunction (new Builtin (intermediate::cross,
                                              vecTypeInt[3],
                                              uc::String ("cross"),
                                              MakeFormalParameters2 (vecTypeInt[3])));
      scope->AddBuiltinFunction (new Builtin (intermediate::cross,
                                              vecTypeUInt[3],
                                              uc::String ("cross"),
                                              MakeFormalParameters2 (vecTypeUInt[3])));
      scope->AddBuiltinFunction (new Builtin (intermediate::cross,
                                              vecTypeFloat[3],
                                              uc::String ("cross"),
                                              MakeFormalParameters2 (vecTypeFloat[3])));

      for (unsigned int l = 1; l < 5; l++)
      {
        for (unsigned int m = 1; m < 5; m++)
        {
          for (unsigned int n = 1; n < 5; n++)
          {
            scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                    matTypeInt[l][n],
                                                    uc::String ("mul"),
                                                    MakeFormalParameters2 (matTypeInt[l][m], matTypeInt[m][n])));
            scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                    matTypeUInt[l][n],
                                                    uc::String ("mul"),
                                                    MakeFormalParameters2 (matTypeUInt[l][m], matTypeUInt[m][n])));
            scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                    matTypeFloat[l][n],
                                                    uc::String ("mul"),
                                                    MakeFormalParameters2 (matTypeFloat[l][m], matTypeFloat[m][n])));
          }
        }
      }

      for (unsigned int m = 1; m < 5; m++)
      {
        for (unsigned int n = 1; n < 5; n++)
        {
          scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                  vecTypeInt[n],
                                                  uc::String ("mul"),
                                                  MakeFormalParameters2 (vecTypeInt[m], matTypeInt[m][n])));
          scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                  vecTypeUInt[n],
                                                  uc::String ("mul"),
                                                  MakeFormalParameters2 (vecTypeUInt[m], matTypeUInt[m][n])));
          scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                  vecTypeFloat[n],
                                                  uc::String ("mul"),
                                                  MakeFormalParameters2 (vecTypeFloat[m], matTypeFloat[m][n])));
        }
      }

      for (unsigned int l = 1; l < 5; l++)
      {
        for (unsigned int m = 1; m < 5; m++)
        {
          scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                  vecTypeInt[l],
                                                  uc::String ("mul"),
                                                  MakeFormalParameters2 (matTypeInt[l][m], vecTypeInt[m])));
          scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                  vecTypeUInt[l],
                                                  uc::String ("mul"),
                                                  MakeFormalParameters2 (matTypeUInt[l][m], vecTypeUInt[m])));
          scope->AddBuiltinFunction (new Builtin (intermediate::mul,
                                                  vecTypeFloat[l],
                                                  uc::String ("mul"),
                                                  MakeFormalParameters2 (matTypeFloat[l][m], vecTypeFloat[m])));
        }
      }

      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (intermediate::normalize,
                                                vecTypeFloat[c],
                                                uc::String ("normalize"),
                                                MakeFormalParameters1 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::normalize,
                                                vecTypeFloat[c],
                                                uc::String ("normalize"),
                                                MakeFormalParameters1 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::normalize,
                                                vecTypeFloat[c],
                                                uc::String ("normalize"),
                                                MakeFormalParameters1 (vecTypeFloat[c])));
      }

      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (intermediate::length,
                                                floatType,
                                                uc::String ("length"),
                                                MakeFormalParameters1 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::length,
                                                floatType,
                                                uc::String ("length"),
                                                MakeFormalParameters1 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::length,
                                                floatType,
                                                uc::String ("length"),
                                                MakeFormalParameters1 (vecTypeFloat[c])));
      }

      scope->AddBuiltinFunction (new Builtin (intermediate::tex1D,
                                              vecTypeFloat[4],
                                              uc::String ("tex1D"),
                                              MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeInt[1])));
      scope->AddBuiltinFunction (new Builtin (intermediate::tex1D,
                                              vecTypeFloat[4],
                                              uc::String ("tex1D"),
                                              MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeUInt[1])));
      scope->AddBuiltinFunction (new Builtin (intermediate::tex1D,
                                              vecTypeFloat[4],
                                              uc::String ("tex1D"),
                                              MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeFloat[1])));

      scope->AddBuiltinFunction (new Builtin (intermediate::tex2D,
                                              vecTypeFloat[4],
                                              uc::String ("tex2D"),
                                              MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeInt[2])));
      scope->AddBuiltinFunction (new Builtin (intermediate::tex2D,
                                              vecTypeFloat[4],
                                              uc::String ("tex2D"),
                                              MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeUInt[2])));
      scope->AddBuiltinFunction (new Builtin (intermediate::tex2D,
                                              vecTypeFloat[4],
                                              uc::String ("tex2D"),
                                              MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeFloat[2])));

      scope->AddBuiltinFunction (new Builtin (intermediate::tex3D,
                                              vecTypeFloat[4],
                                              uc::String ("tex3D"),
                                              MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeInt[3])));
      scope->AddBuiltinFunction (new Builtin (intermediate::tex3D,
                                              vecTypeFloat[4],
                                              uc::String ("tex3D"),
                                              MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeUInt[3])));
      scope->AddBuiltinFunction (new Builtin (intermediate::tex3D,
                                              vecTypeFloat[4],
                                              uc::String ("tex3D"),
                                              MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeFloat[3])));

      scope->AddBuiltinFunction (new Builtin (intermediate::texCUBE,
                                              vecTypeFloat[4],
                                              uc::String ("texCUBE"),
                                              MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeInt[3])));
      scope->AddBuiltinFunction (new Builtin (intermediate::texCUBE,
                                              vecTypeFloat[4],
                                              uc::String ("texCUBE"),
                                              MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeUInt[3])));
      scope->AddBuiltinFunction (new Builtin (intermediate::texCUBE,
                                              vecTypeFloat[4],
                                              uc::String ("texCUBE"),
                                              MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeFloat[3])));

      scope->AddBuiltinFunction (new Builtin (intermediate::min,
                                              intType,
                                              uc::String ("min"),
                                              MakeFormalParameters2 (intType)));
      scope->AddBuiltinFunction (new Builtin (intermediate::min,
                                              uintType,
                                              uc::String ("min"),
                                              MakeFormalParameters2 (uintType)));
      scope->AddBuiltinFunction (new Builtin (intermediate::min,
                                              floatType,
                                              uc::String ("min"),
                                              MakeFormalParameters2 (floatType)));
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (intermediate::min,
                                                vecTypeInt[c],
                                                uc::String ("min"),
                                                MakeFormalParameters2 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::min,
                                                vecTypeUInt[c],
                                                uc::String ("min"),
                                                 MakeFormalParameters2 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::min,
                                                vecTypeFloat[c],
                                                uc::String ("min"),
                                                MakeFormalParameters2 (vecTypeFloat[c])));
      }

      scope->AddBuiltinFunction (new Builtin (intermediate::max,
                                              intType,
                                              uc::String ("max"),
                                              MakeFormalParameters2 (intType)));
      scope->AddBuiltinFunction (new Builtin (intermediate::max,
                                              uintType,
                                              uc::String ("max"),
                                              MakeFormalParameters2 (uintType)));
      scope->AddBuiltinFunction (new Builtin (intermediate::max,
                                              floatType,
                                              uc::String ("max"),
                                              MakeFormalParameters2 (floatType)));
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (intermediate::max,
                                                vecTypeInt[c],
                                                uc::String ("max"),
                                                MakeFormalParameters2 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::max,
                                                vecTypeUInt[c],
                                                uc::String ("max"),
                                                MakeFormalParameters2 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (intermediate::max,
                                                vecTypeFloat[c],
                                                uc::String ("max"),
                                                MakeFormalParameters2 (vecTypeFloat[c])));
      }
    }
  } // namespace intermediate
} // namespace s1
