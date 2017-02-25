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
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"

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

      auto default_builtin_factory =
        [](intermediate::BuiltinFunction what)
        {
          return
            [what](RegisterPtr destination, const std::vector<RegisterPtr>& inParams) -> SequenceOpPtr
            {
              return new SequenceOpBuiltinCall (destination, what, inParams);
            };
      };

      auto pow_factory = default_builtin_factory (intermediate::pow);
      scope->AddBuiltinFunction (new Builtin (pow_factory,
                                              floatType,
                                              uc::String ("pow"),
                                              MakeFormalParameters2 (floatType)));

      uc::String id_dot ("dot");
      auto dot_factory = default_builtin_factory (intermediate::dot);
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (dot_factory,
                                                intType,
                                                id_dot,
                                                MakeFormalParameters2 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (dot_factory,
                                                uintType,
                                                id_dot,
                                                MakeFormalParameters2 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (dot_factory,
                                                floatType,
                                                id_dot,
                                                MakeFormalParameters2 (vecTypeFloat[c])));
      }
      uc::String id_cross ("cross");
      auto cross_factory = default_builtin_factory (intermediate::dot);
      scope->AddBuiltinFunction (new Builtin (cross_factory,
                                              vecTypeInt[3],
                                              id_cross,
                                              MakeFormalParameters2 (vecTypeInt[3])));
      scope->AddBuiltinFunction (new Builtin (cross_factory,
                                              vecTypeUInt[3],
                                              id_cross,
                                              MakeFormalParameters2 (vecTypeUInt[3])));
      scope->AddBuiltinFunction (new Builtin (cross_factory,
                                              vecTypeFloat[3],
                                              id_cross,
                                              MakeFormalParameters2 (vecTypeFloat[3])));

      uc::String id_mul ("mul");
      auto mul_factory = default_builtin_factory (intermediate::mul);
      for (unsigned int l = 1; l < 5; l++)
      {
        for (unsigned int m = 1; m < 5; m++)
        {
          for (unsigned int n = 1; n < 5; n++)
          {
            scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                    matTypeInt[l][n],
                                                    id_mul,
                                                    MakeFormalParameters2 (matTypeInt[l][m], matTypeInt[m][n])));
            scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                    matTypeUInt[l][n],
                                                    id_mul,
                                                    MakeFormalParameters2 (matTypeUInt[l][m], matTypeUInt[m][n])));
            scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                    matTypeFloat[l][n],
                                                    id_mul,
                                                    MakeFormalParameters2 (matTypeFloat[l][m], matTypeFloat[m][n])));
          }
        }
      }

      for (unsigned int m = 1; m < 5; m++)
      {
        for (unsigned int n = 1; n < 5; n++)
        {
          scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                  vecTypeInt[n],
                                                  id_mul,
                                                  MakeFormalParameters2 (vecTypeInt[m], matTypeInt[m][n])));
          scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                  vecTypeUInt[n],
                                                  id_mul,
                                                  MakeFormalParameters2 (vecTypeUInt[m], matTypeUInt[m][n])));
          scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                  vecTypeFloat[n],
                                                  id_mul,
                                                  MakeFormalParameters2 (vecTypeFloat[m], matTypeFloat[m][n])));
        }
      }

      for (unsigned int l = 1; l < 5; l++)
      {
        for (unsigned int m = 1; m < 5; m++)
        {
          scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                  vecTypeInt[l],
                                                  id_mul,
                                                  MakeFormalParameters2 (matTypeInt[l][m], vecTypeInt[m])));
          scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                  vecTypeUInt[l],
                                                  id_mul,
                                                  MakeFormalParameters2 (matTypeUInt[l][m], vecTypeUInt[m])));
          scope->AddBuiltinFunction (new Builtin (mul_factory,
                                                  vecTypeFloat[l],
                                                  id_mul,
                                                  MakeFormalParameters2 (matTypeFloat[l][m], vecTypeFloat[m])));
        }
      }

      uc::String id_normalize ("normalize");
      auto normalize_factory = default_builtin_factory (intermediate::normalize);
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (normalize_factory,
                                                vecTypeFloat[c],
                                                id_normalize,
                                                MakeFormalParameters1 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (normalize_factory,
                                                vecTypeFloat[c],
                                                id_normalize,
                                                MakeFormalParameters1 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (normalize_factory,
                                                vecTypeFloat[c],
                                                id_normalize,
                                                MakeFormalParameters1 (vecTypeFloat[c])));
      }

      uc::String id_length ("length");
      auto length_factory = default_builtin_factory (intermediate::length);
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (length_factory,
                                                floatType,
                                                id_length,
                                                MakeFormalParameters1 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (length_factory,
                                                floatType,
                                                id_length,
                                                MakeFormalParameters1 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (length_factory,
                                                floatType,
                                                id_length,
                                                MakeFormalParameters1 (vecTypeFloat[c])));
      }

      uc::String id_tex1D ("tex1D");
      auto tex1D_factory = default_builtin_factory (intermediate::tex1D);
      scope->AddBuiltinFunction (new Builtin (tex1D_factory,
                                              vecTypeFloat[4],
                                              id_tex1D,
                                              MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeInt[1])));
      scope->AddBuiltinFunction (new Builtin (tex1D_factory,
                                              vecTypeFloat[4],
                                              id_tex1D,
                                              MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeUInt[1])));
      scope->AddBuiltinFunction (new Builtin (tex1D_factory,
                                              vecTypeFloat[4],
                                              id_tex1D,
                                              MakeFormalParameters2 (CreateSamplerType (_1D), vecTypeFloat[1])));

      uc::String id_tex2D ("tex2D");
      auto tex2D_factory = default_builtin_factory (intermediate::tex2D);
      scope->AddBuiltinFunction (new Builtin (tex2D_factory,
                                              vecTypeFloat[4],
                                              id_tex2D,
                                              MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeInt[2])));
      scope->AddBuiltinFunction (new Builtin (tex2D_factory,
                                              vecTypeFloat[4],
                                              id_tex2D,
                                              MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeUInt[2])));
      scope->AddBuiltinFunction (new Builtin (tex2D_factory,
                                              vecTypeFloat[4],
                                              id_tex2D,
                                              MakeFormalParameters2 (CreateSamplerType (_2D), vecTypeFloat[2])));

      uc::String id_tex3D ("tex3D");
      auto tex3D_factory = default_builtin_factory (intermediate::tex3D);
      scope->AddBuiltinFunction (new Builtin (tex3D_factory,
                                              vecTypeFloat[4],
                                              id_tex3D,
                                              MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeInt[3])));
      scope->AddBuiltinFunction (new Builtin (tex3D_factory,
                                              vecTypeFloat[4],
                                              id_tex3D,
                                              MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeUInt[3])));
      scope->AddBuiltinFunction (new Builtin (tex3D_factory,
                                              vecTypeFloat[4],
                                              id_tex3D,
                                              MakeFormalParameters2 (CreateSamplerType (_3D), vecTypeFloat[3])));

      uc::String id_texCUBE ("texCUBE");
      auto texCUBE_factory = default_builtin_factory (intermediate::texCUBE);
      scope->AddBuiltinFunction (new Builtin (texCUBE_factory,
                                              vecTypeFloat[4],
                                              id_texCUBE,
                                              MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeInt[3])));
      scope->AddBuiltinFunction (new Builtin (texCUBE_factory,
                                              vecTypeFloat[4],
                                              id_texCUBE,
                                              MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeUInt[3])));
      scope->AddBuiltinFunction (new Builtin (texCUBE_factory,
                                              vecTypeFloat[4],
                                              id_texCUBE,
                                              MakeFormalParameters2 (CreateSamplerType (CUBE), vecTypeFloat[3])));

      uc::String id_min ("min");
      auto min_factory = default_builtin_factory (intermediate::min);
      scope->AddBuiltinFunction (new Builtin (min_factory,
                                              intType,
                                              id_min,
                                              MakeFormalParameters2 (intType)));
      scope->AddBuiltinFunction (new Builtin (min_factory,
                                              uintType,
                                              id_min,
                                              MakeFormalParameters2 (uintType)));
      scope->AddBuiltinFunction (new Builtin (min_factory,
                                              floatType,
                                              id_min,
                                              MakeFormalParameters2 (floatType)));
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (min_factory,
                                                vecTypeInt[c],
                                                id_min,
                                                MakeFormalParameters2 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (min_factory,
                                                vecTypeUInt[c],
                                                id_min,
                                                 MakeFormalParameters2 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (min_factory,
                                                vecTypeFloat[c],
                                                id_min,
                                                MakeFormalParameters2 (vecTypeFloat[c])));
      }

      uc::String id_max ("max");
      auto max_factory = default_builtin_factory (intermediate::max);
      scope->AddBuiltinFunction (new Builtin (max_factory,
                                              intType,
                                              id_max,
                                              MakeFormalParameters2 (intType)));
      scope->AddBuiltinFunction (new Builtin (max_factory,
                                              uintType,
                                              id_max,
                                              MakeFormalParameters2 (uintType)));
      scope->AddBuiltinFunction (new Builtin (max_factory,
                                              floatType,
                                              id_max,
                                              MakeFormalParameters2 (floatType)));
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (new Builtin (max_factory,
                                                vecTypeInt[c],
                                                id_max,
                                                MakeFormalParameters2 (vecTypeInt[c])));
        scope->AddBuiltinFunction (new Builtin (max_factory,
                                                vecTypeUInt[c],
                                                id_max,
                                                MakeFormalParameters2 (vecTypeUInt[c])));
        scope->AddBuiltinFunction (new Builtin (max_factory,
                                                vecTypeFloat[c],
                                                id_max,
                                                MakeFormalParameters2 (vecTypeFloat[c])));
      }
    }
  } // namespace intermediate
} // namespace s1
