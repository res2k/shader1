/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


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
#include "intermediate/SequenceVisitor.h"
#include "semantics/FunctionFormalParameter.h"

#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    semantics::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters1 (const semantics::TypePtr& type)
    {
      semantics::FunctionFormalParameters params;

      semantics::FunctionFormalParameter fp;
      fp.type = type;
      fp.identifier = uc::String ("a");
      fp.dir = semantics::FunctionFormalParameter::dirIn;
      params.push_back (fp);

      return params;
    }

    semantics::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters2 (const semantics::TypePtr& type)
    {
      return MakeFormalParameters2 (type, type);
    }

    semantics::FunctionFormalParameters
    IntermediateGeneratorSemanticsHandler::MakeFormalParameters2 (const semantics::TypePtr& typeA,
                                                                  const semantics::TypePtr& typeB)
    {
      semantics::FunctionFormalParameters params;

      semantics::FunctionFormalParameter fp;
      fp.type = typeA;
      fp.identifier = uc::String ("a");
      fp.dir = semantics::FunctionFormalParameter::dirIn;
      params.push_back (fp);

      fp.type = typeB;
      fp.identifier = uc::String ("b");
      params.push_back (fp);

      return params;
    }

    void IntermediateGeneratorSemanticsHandler::SetupBuiltins (const ScopeImplPtr& scope)
    {
      // TODO: Check if less functions can be used; e.g. instead of all the overloads for vector types, let casts do the work

      semantics::TypePtr vecTypeInt[5];
      semantics::TypePtr vecTypeUInt[5];
      semantics::TypePtr vecTypeFloat[5];
      for (unsigned int c = 1; c < 5; c++)
      {
        vecTypeInt[c] = CreateVectorType (intType, c);
        vecTypeUInt[c] = CreateVectorType (uintType, c);
        vecTypeFloat[c] = CreateVectorType (floatType, c);
      }

      semantics::TypePtr matTypeInt[5][5];
      semantics::TypePtr matTypeUInt[5][5];
      semantics::TypePtr matTypeFloat[5][5];
      for (unsigned int c = 1; c < 5; c++)
      {
        for (unsigned int r = 1; r < 5; r++)
        {
          matTypeInt[r][c] = CreateMatrixType (intType, c, r);
          matTypeUInt[r][c] = CreateMatrixType (uintType, c, r);
          matTypeFloat[r][c] = CreateMatrixType (floatType, c, r);
        }
      }

      scope->AddBuiltinFunction (semantics::Builtin::Pow,
                                 floatType.get(),
                                 uc::String ("pow"),
                                 MakeFormalParameters2 (floatType));

      uc::String id_dot ("dot");
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (semantics::Builtin::VecDot,
                                   intType.get(),
                                   id_dot,
                                   MakeFormalParameters2 (vecTypeInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::VecDot,
                                   uintType.get(),
                                   id_dot,
                                   MakeFormalParameters2 (vecTypeUInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::VecDot,
                                   floatType.get(),
                                   id_dot,
                                   MakeFormalParameters2 (vecTypeFloat[c]));
      }
      uc::String id_cross ("cross");
      scope->AddBuiltinFunction (semantics::Builtin::VecCross,
                                 vecTypeInt[3].get(),
                                 id_cross,
                                 MakeFormalParameters2 (vecTypeInt[3]));
      scope->AddBuiltinFunction (semantics::Builtin::VecCross,
                                 vecTypeUInt[3].get(),
                                 id_cross,
                                 MakeFormalParameters2 (vecTypeUInt[3]));
      scope->AddBuiltinFunction (semantics::Builtin::VecCross,
                                 vecTypeFloat[3].get(),
                                 id_cross,
                                 MakeFormalParameters2 (vecTypeFloat[3]));

      uc::String id_mul ("mul");
      for (unsigned int l = 1; l < 5; l++)
      {
        for (unsigned int m = 1; m < 5; m++)
        {
          for (unsigned int n = 1; n < 5; n++)
          {
            scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                       matTypeInt[l][n].get(),
                                       id_mul,
                                       MakeFormalParameters2 (matTypeInt[l][m], matTypeInt[m][n]));
            scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                       matTypeUInt[l][n].get(),
                                       id_mul,
                                       MakeFormalParameters2 (matTypeUInt[l][m], matTypeUInt[m][n]));
            scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                       matTypeFloat[l][n].get(),
                                       id_mul,
                                       MakeFormalParameters2 (matTypeFloat[l][m], matTypeFloat[m][n]));
          }
        }
      }

      for (unsigned int m = 1; m < 5; m++)
      {
        for (unsigned int n = 1; n < 5; n++)
        {
          scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                     vecTypeInt[n].get(),
                                     id_mul,
                                     MakeFormalParameters2 (vecTypeInt[m], matTypeInt[m][n]));
          scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                     vecTypeUInt[n].get(),
                                     id_mul,
                                     MakeFormalParameters2 (vecTypeUInt[m], matTypeUInt[m][n]));
          scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                     vecTypeFloat[n].get(),
                                     id_mul,
                                     MakeFormalParameters2 (vecTypeFloat[m], matTypeFloat[m][n]));
        }
      }

      for (unsigned int l = 1; l < 5; l++)
      {
        for (unsigned int m = 1; m < 5; m++)
        {
          scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                     vecTypeInt[l].get(),
                                     id_mul,
                                     MakeFormalParameters2 (matTypeInt[l][m], vecTypeInt[m]));
          scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                     vecTypeUInt[l].get(),
                                     id_mul,
                                     MakeFormalParameters2 (matTypeUInt[l][m], vecTypeUInt[m]));
          scope->AddBuiltinFunction (semantics::Builtin::MatrixLinAlgMul,
                                     vecTypeFloat[l].get(),
                                     id_mul,
                                     MakeFormalParameters2 (matTypeFloat[l][m], vecTypeFloat[m]));
        }
      }

      uc::String id_normalize ("normalize");
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (semantics::Builtin::VecNormalize,
                                   vecTypeFloat[c].get(),
                                   id_normalize,
                                   MakeFormalParameters1 (vecTypeInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::VecNormalize,
                                   vecTypeFloat[c].get(),
                                   id_normalize,
                                   MakeFormalParameters1 (vecTypeUInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::VecNormalize,
                                   vecTypeFloat[c].get(),
                                   id_normalize,
                                   MakeFormalParameters1 (vecTypeFloat[c]));
      }

      uc::String id_length ("length");
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (semantics::Builtin::VecLength,
                                   floatType.get(),
                                   id_length,
                                   MakeFormalParameters1 (vecTypeInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::VecLength,
                                   floatType.get(),
                                   id_length,
                                   MakeFormalParameters1 (vecTypeUInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::VecLength,
                                   floatType.get(),
                                   id_length,
                                   MakeFormalParameters1 (vecTypeFloat[c]));
      }

      uc::String id_tex1D ("tex1D");
      auto sampler_1D = CreateSamplerType (semantics::SamplerType::_1D);
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex1D,
                                 vecTypeFloat[4].get(),
                                 id_tex1D,
                                 MakeFormalParameters2 (sampler_1D, vecTypeInt[1]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex1D,
                                 vecTypeFloat[4].get(),
                                 id_tex1D,
                                 MakeFormalParameters2 (sampler_1D, vecTypeUInt[1]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex1D,
                                 vecTypeFloat[4].get(),
                                 id_tex1D,
                                 MakeFormalParameters2 (sampler_1D, vecTypeFloat[1]));

      uc::String id_tex2D ("tex2D");
      auto sampler_2D = CreateSamplerType (semantics::SamplerType::_2D);
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex2D,
                                 vecTypeFloat[4].get(),
                                 id_tex2D,
                                 MakeFormalParameters2 (sampler_2D, vecTypeInt[2]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex2D,
                                 vecTypeFloat[4].get(),
                                 id_tex2D,
                                 MakeFormalParameters2 (sampler_2D, vecTypeUInt[2]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex2D,
                                 vecTypeFloat[4].get(),
                                 id_tex2D,
                                 MakeFormalParameters2 (sampler_2D, vecTypeFloat[2]));

      uc::String id_tex3D ("tex3D");
      auto sampler_3D = CreateSamplerType (semantics::SamplerType::_3D);
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex3D,
                                 vecTypeFloat[4].get(),
                                 id_tex3D,
                                 MakeFormalParameters2 (sampler_3D, vecTypeInt[3]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex3D,
                                 vecTypeFloat[4].get(),
                                 id_tex3D,
                                 MakeFormalParameters2 (sampler_3D, vecTypeUInt[3]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTex3D,
                                 vecTypeFloat[4].get(),
                                 id_tex3D,
                                 MakeFormalParameters2 (sampler_3D, vecTypeFloat[3]));

      uc::String id_texCUBE ("texCUBE");
      auto sampler_CUBE = CreateSamplerType (semantics::SamplerType::CUBE);
      scope->AddBuiltinFunction (semantics::Builtin::SampleTexCUBE,
                                 vecTypeFloat[4].get(),
                                 id_texCUBE,
                                 MakeFormalParameters2 (sampler_CUBE, vecTypeInt[3]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTexCUBE,
                                 vecTypeFloat[4].get(),
                                 id_texCUBE,
                                 MakeFormalParameters2 (sampler_CUBE, vecTypeUInt[3]));
      scope->AddBuiltinFunction (semantics::Builtin::SampleTexCUBE,
                                 vecTypeFloat[4].get(),
                                 id_texCUBE,
                                 MakeFormalParameters2 (sampler_CUBE, vecTypeFloat[3]));

      uc::String id_min ("min");
      scope->AddBuiltinFunction (semantics::Builtin::Min,
                                 intType.get(),
                                 id_min,
                                 MakeFormalParameters2 (intType));
      scope->AddBuiltinFunction (semantics::Builtin::Min,
                                 uintType.get(),
                                 id_min,
                                 MakeFormalParameters2 (uintType));
      scope->AddBuiltinFunction (semantics::Builtin::Min,
                                 floatType.get(),
                                 id_min,
                                 MakeFormalParameters2 (floatType));
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (semantics::Builtin::Min,
                                   vecTypeInt[c].get(),
                                   id_min,
                                   MakeFormalParameters2 (vecTypeInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::Min,
                                   vecTypeUInt[c].get(),
                                   id_min,
                                   MakeFormalParameters2 (vecTypeUInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::Min,
                                   vecTypeFloat[c].get(),
                                   id_min,
                                   MakeFormalParameters2 (vecTypeFloat[c]));
      }

      uc::String id_max ("max");
      scope->AddBuiltinFunction (semantics::Builtin::Max,
                                 intType.get(),
                                 id_max,
                                 MakeFormalParameters2 (intType));
      scope->AddBuiltinFunction (semantics::Builtin::Max,
                                 uintType.get(),
                                 id_max,
                                 MakeFormalParameters2 (uintType));
      scope->AddBuiltinFunction (semantics::Builtin::Max,
                                 floatType.get(),
                                 id_max,
                                 MakeFormalParameters2 (floatType));
      for (unsigned int c = 1; c < 5; c++)
      {
        scope->AddBuiltinFunction (semantics::Builtin::Max,
                                   vecTypeInt[c].get(),
                                   id_max,
                                   MakeFormalParameters2 (vecTypeInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::Max,
                                   vecTypeUInt[c].get(),
                                   id_max,
                                   MakeFormalParameters2 (vecTypeUInt[c]));
        scope->AddBuiltinFunction (semantics::Builtin::Max,
                                   vecTypeFloat[c].get(),
                                   id_max,
                                   MakeFormalParameters2 (vecTypeFloat[c]));
      }
    }
  } // namespace intermediate
} // namespace s1
