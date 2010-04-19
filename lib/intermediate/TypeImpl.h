#ifndef __INTERMEDIATE_TYPEIMPL_H__
#define __INTERMEDIATE_TYPEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    struct IntermediateGeneratorSemanticsHandler::TypeImpl : public Type
    {
      enum Class
      {
	Base, Sampler, Array, Vector, Matrix
      };
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
      
      /// Returns whether this type is losslessly assignable to \a to.
      bool CompatibleLossless (const TypeImpl& to) const;
      bool CompatibleLossy (const TypeImpl& to) const;
      bool IsEqual (const TypeImpl& other) const;
      bool IsPrecisionHigherEqual (const TypeImpl& other) const;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TYPEIMPL_H__
