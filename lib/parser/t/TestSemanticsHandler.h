#ifndef __TESTSYNTAXHANDLER_H__
#define __TESTSYNTAXHANDLER_H__

#include "parser/ParserSemanticsHandler.h"

class TestSemanticsHandler : public s1::ParserSemanticsHandler
{
public:
  struct TestType : public Type
  {
    enum Class
    {
      Base, Sampler, Array, Vector, Matrix
    };
    Class typeClass;
    BaseType base;
    SamplerType sampler;
    TypePtr avmBase;
    unsigned int vectorDim;
    unsigned int matrixCols;
    unsigned int matrixRows;
    
    TestType (BaseType base) : typeClass (Base), base (base) {}
    TestType (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
    TestType (TypePtr aBase) : typeClass (Array), avmBase (aBase) {}
    TestType (TypePtr vBase, unsigned int d)
     : typeClass (Vector), avmBase (vBase), vectorDim (d) {}
    TestType (TypePtr mBase, unsigned int c, unsigned int r)
     : typeClass (Matrix), avmBase (mBase), matrixCols (c), matrixRows (r) {}
  };
  
  TypePtr CreateType (BaseType type)
  {
    return TypePtr (new TestType (type));
  }
  
  TypePtr CreateSamplerType (SamplerType dim)
  {
    return TypePtr (new TestType (dim));
  }
  
  TypePtr CreateArrayType (TypePtr baseType)
  {
    return TypePtr (new TestType (baseType));
  }
  
  TypePtr CreateVectorType (TypePtr baseType,
			    unsigned int components)
  {
    return TypePtr (new TestType (baseType, components));
  }
  
  TypePtr CreateMatrixType (TypePtr baseType,
			    unsigned int columns,
			    unsigned int rows)
  {
    return TypePtr (new TestType (baseType, columns, rows));
  }
  
};

#endif // __TESTSYNTAXHANDLER_H__
