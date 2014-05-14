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

#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    bool IntermediateGeneratorSemanticsHandler::TypeImpl::CompatibleLossless (const TypeImpl& to) const
    {
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
	if ((base == Void) || (to.base == Void))
	  // Void is not even compatible to itself
	  return false;
	if (((base == Int) || (base == UInt))
	    && ((to.base == Int) || (to.base == UInt)))
	  // int and unsigned int are assumed assignable without precision loss
	  return true;
	if (((base == Int) || (base == UInt))
	    && (to.base == Float))
	  // assignment from int or unsigned int to float is assumed assignable without precision loss
	  return true;
	// Otherwise, types must be equal
	return base == to.base;
      case Sampler:
	// Samplers have to be equal
	return sampler == to.sampler;
      case Array:
	// Array assignments are compatible when the contained members are
	return static_cast<TypeImpl*> (avmBase.get())->CompatibleLossless (
	  *(static_cast<TypeImpl*> (to.avmBase.get())));
      case Vector:
	// Vectors: base types must be compatible and both types have the same number of components
	return static_cast<TypeImpl*> (avmBase.get())->CompatibleLossless (
	  *(static_cast<TypeImpl*> (to.avmBase.get())))
	  && (vectorDim == to.vectorDim);
      case Matrix:
	// Matrices: base types must be compatible and both types have the same number of rows/cols
	return static_cast<TypeImpl*> (avmBase.get())->CompatibleLossless (
	  *(static_cast<TypeImpl*> (to.avmBase.get())))
	  && (matrixCols == to.matrixCols)
	  && (matrixRows == to.matrixRows);
      }
      assert (false);
      return false;
    }
    
    bool IntermediateGeneratorSemanticsHandler::TypeImpl::CompatibleLossy (const TypeImpl& to) const
    {
      // Lossless compatibility implies lossy compatibility ;)
      if (CompatibleLossless (to)) return true;
      
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
	if ((base == Float)
	    && ((to.base == Int) || (to.base == UInt)))
	  // assignment from float to int or unsigned int possible with precision loss
	  return true;
      default:
	break;
      }
      return false;
    }
    
    bool IntermediateGeneratorSemanticsHandler::TypeImpl::IsEqual (const TypeImpl& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
	return base == other.base;
      case Sampler:
	return sampler == other.sampler;
      case Array:
	return static_cast<TypeImpl*> (avmBase.get())->IsEqual (
	  *(static_cast<TypeImpl*> (other.avmBase.get())));
      case Vector:
	return static_cast<TypeImpl*> (avmBase.get())->IsEqual (
	  *(static_cast<TypeImpl*> (other.avmBase.get())))
	  && (vectorDim == other.vectorDim);
      case Matrix:
	return static_cast<TypeImpl*> (avmBase.get())->IsEqual (
	  *(static_cast<TypeImpl*> (other.avmBase.get())))
	  && (matrixCols == other.matrixCols)
	  && (matrixRows == other.matrixRows);
      }
      assert (false);
      return false;
    }

    bool IntermediateGeneratorSemanticsHandler::TypeImpl::IsPrecisionHigherEqual (const TypeImpl& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
	// Void is not even of equal precision to itself...
	if ((base == Void) || (other.base == Void))
	  return false;
	// Treat 'int' as higher precision as 'unsigned int' (b/c 'int' has a sign)
	if ((base == Int)
	    && ((other.base == Int) || (other.base == UInt)))
	  return true;
	// Float is considered higher precision than int or unsigned int (has fractions)
	if ((base == Float)
	    && ((other.base == Int) || (other.base == UInt)))
	  // assignment from int or unsigned int to float is assumed assignable without precision loss
	  return true;
	// Otherwise, types must be equal
	return base == other.base;
      case Sampler:
	// Samplers have to be equal
	return sampler == other.sampler;
      case Array:
	// Array type is higher/equal prec when the contained members are
	return static_cast<TypeImpl*> (avmBase.get())->IsPrecisionHigherEqual (
	  *(static_cast<TypeImpl*> (other.avmBase.get())));
      case Vector:
	// Vectors: base type is higher/equal prec if both types have the same number of components
	return static_cast<TypeImpl*> (avmBase.get())->IsPrecisionHigherEqual (
	  *(static_cast<TypeImpl*> (other.avmBase.get())))
	  && (vectorDim == other.vectorDim);
      case Matrix:
	// Matrices: base type is higher/equal prec if both types have the same number of rows/cols
	return static_cast<TypeImpl*> (avmBase.get())->IsPrecisionHigherEqual (
	  *(static_cast<TypeImpl*> (other.avmBase.get())))
	  && (matrixCols == other.matrixCols)
	  && (matrixRows == other.matrixRows);
      }
      assert (false);
      return false;
    }
    
  } // namespace intermediate
} // namespace s1
