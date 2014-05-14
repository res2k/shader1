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

#ifndef __INTERMEDIATE_TYPEIMPL_H__
#define __INTERMEDIATE_TYPEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    struct IntermediateGeneratorSemanticsHandler::TypeImpl : public Type
    {
      Class typeClass;
      parser::SemanticsHandler::BaseType base;
      parser::SemanticsHandler::SamplerType sampler;
      parser::SemanticsHandler::TypePtr avmBase;
      unsigned int vectorDim;
      unsigned int matrixCols;
      unsigned int matrixRows;
      
      TypeImpl (parser::SemanticsHandler::BaseType base) : typeClass (Base), base (base) {}
      TypeImpl (parser::SemanticsHandler::SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
      TypeImpl (parser::SemanticsHandler::TypePtr aBase) : typeClass (Array), avmBase (aBase) {}
      TypeImpl (parser::SemanticsHandler::TypePtr vBase, unsigned int d)
	: typeClass (Vector), avmBase (vBase), vectorDim (d) {}
      TypeImpl (parser::SemanticsHandler::TypePtr mBase, unsigned int c, unsigned int r)
	: typeClass (Matrix), avmBase (mBase), matrixCols (c), matrixRows (r) {}
      
      Class GetTypeClass() const { return typeClass; }
      BaseType GetBaseType() const { return base; }
      SamplerType GetSamplerType() const { return sampler; }
      
      TypePtr GetArrayVectorMatrixBaseType() const { return avmBase; }
      
      unsigned int GetVectorTypeComponents() const { return vectorDim; }
      
      unsigned int GetMatrixTypeCols() const { return matrixCols; }
      unsigned int GetMatrixTypeRows() const { return matrixRows; }
      
      /// Returns whether this type is losslessly assignable to \a to.
      bool CompatibleLossless (const TypeImpl& to) const;
      bool CompatibleLossy (const TypeImpl& to) const;
      bool IsEqual (const TypeImpl& other) const;
      bool IsPrecisionHigherEqual (const TypeImpl& other) const;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TYPEIMPL_H__
