#ifndef __TESTSEMANTICSHANDLER_H__
#define __TESTSEMANTICSHANDLER_H__

#include "parser/CommonSemanticsHandler.h"


enum
{
  /**
   * Resolve undeclared identifiers without throwing
   * (for expression tests)
   */
  testoptIdentifiersSloppy = 1
};

template<int Options>
class TestSemanticsHandlerTemplated : public s1::parser::CommonSemanticsHandler
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
  
  struct TestExpressionBase : public Expression
  {
    virtual const std::string& GetExprString() = 0;
  };
  
  struct TestExpressionConst : public TestExpressionBase
  {
    std::string str;
    
    TestExpressionConst (const std::string& str) : str (str) { }
    
    const std::string& GetExprString() { return str; }
  };
  
  struct TestExpressionVar : public TestExpressionBase
  {
    std::string identifier;
    
    TestExpressionVar (const UnicodeString& identifier)
    {
      StringByteSink<std::string> utfSink (&(this->identifier));
      identifier.toUTF8 (utfSink);
    }
    
    const std::string& GetExprString() { return identifier; }
  };
  
  struct TestExpressionOp : public TestExpressionBase
  {
    std::string str;
    
    TestExpressionOp (const char* op, ExpressionPtr left, ExpressionPtr right)
    {
      str = "(";
      str.append (static_cast<TestExpressionBase*> (left.get())->GetExprString());
      str.append (" ");
      str.append (op);
      str.append (" ");
      str.append (static_cast<TestExpressionBase*> (right.get())->GetExprString());
      str.append (")");
    }
    
    const std::string& GetExprString() { return str; }
  };
  
  struct TestExpressionUnary : public TestExpressionBase
  {
    std::string str;
    
    TestExpressionUnary (const char* op, ExpressionPtr right)
    {
      str = op;
      str.append (static_cast<TestExpressionBase*> (right.get())->GetExprString());
    }
    
    const std::string& GetExprString() { return str; }
  };
  
  struct TestExpressionTernary : public TestExpressionBase
  {
    std::string str;
    
    TestExpressionTernary (ExpressionPtr condition, ExpressionPtr ifExpr, ExpressionPtr thenExpr)
    {
      str = "(";
      str.append (static_cast<TestExpressionBase*> (condition.get())->GetExprString());
      str.append (" ? ");
      str.append (static_cast<TestExpressionBase*> (ifExpr.get())->GetExprString());
      str.append (" : ");
      str.append (static_cast<TestExpressionBase*> (thenExpr.get())->GetExprString());
      str.append (")");
    }
    
    const std::string& GetExprString() { return str; }
  };
  
  struct TestExpressionAttr : public TestExpressionBase
  {
    std::string str;
    
    TestExpressionAttr (ExpressionPtr base, const UnicodeString& attr)
    {
      std::string attrStr;
      StringByteSink<std::string> utfSink (&attrStr);
      attr.toUTF8 (utfSink);
      
      str = static_cast<TestExpressionBase*> (base.get())->GetExprString();
      str.append (".");
      str.append (attrStr);
    }
    
    const std::string& GetExprString() { return str; }
  };
  
  struct TestExpressionArray : public TestExpressionBase
  {
    std::string str;
    
    TestExpressionArray (ExpressionPtr base, ExpressionPtr index)
    {
      str = static_cast<TestExpressionBase*> (base.get())->GetExprString();
      str.append ("[");
      str.append (static_cast<TestExpressionBase*> (index.get())->GetExprString());
      str.append ("]");
    }
    
    const std::string& GetExprString() { return str; }
  };
  
  ExpressionPtr CreateConstBoolExpression (bool value)
  {
    return ExpressionPtr (new TestExpressionConst (value ? "true" : "false"));
  }
  
  ExpressionPtr CreateConstNumericExpression (const UnicodeString& valueStr)
  {
    std::string str;
    StringByteSink<std::string> utfSink (&str);
    valueStr.toUTF8 (utfSink);
    return ExpressionPtr (new TestExpressionConst (str));
  }
  
  ExpressionPtr CreateVariableExpression (NamePtr name)
  {
    return ExpressionPtr (new TestExpressionVar (
      static_cast<CommonName*> (name.get())->identifier));
  }
  
  ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
				       const UnicodeString& attr)
  {
    return ExpressionPtr (new TestExpressionAttr (expr, attr));
  }
      
  ExpressionPtr CreateArrayElementAccess (ExpressionPtr arrayExpr,
					  ExpressionPtr elementExpr)
  {
    return ExpressionPtr (new TestExpressionArray (arrayExpr, elementExpr));
  }
  
  ExpressionPtr CreateAssignExpression (ExpressionPtr target,
				        ExpressionPtr value)
  {
    return ExpressionPtr (new TestExpressionOp ("=",
      target, value));
  }
						
  ExpressionPtr CreateArithmeticExpression (ArithmeticOp op,
					    ExpressionPtr operand1,
					    ExpressionPtr operand2)
  {
    const char* opStr = 0;
    switch (op)
    {
    case Add:	opStr = "+"; break;
    case Sub:	opStr = "-"; break;
    case Mul:	opStr = "*"; break;
    case Div:	opStr = "/"; break;
    case Mod:	opStr = "%"; break;
    }
    return ExpressionPtr (new TestExpressionOp (opStr,
      operand1, operand2));
  }
  
  ExpressionPtr CreateUnaryExpression (UnaryOp op,
				       ExpressionPtr operand)
  {
    const char* opStr = 0;
    switch (op)
    {
    case Neg:	opStr = "-"; break;
    case Inv:	opStr = "~"; break;
    case Not:	opStr = "!"; break;
    }
    return ExpressionPtr (new TestExpressionUnary (opStr, operand));
  }
						
  ExpressionPtr CreateTernaryExpression (ExpressionPtr condition,
					 ExpressionPtr ifExpr,
					 ExpressionPtr thenExpr)
  {
    return ExpressionPtr (new TestExpressionTernary (condition, ifExpr, thenExpr));
  }
						  
  ExpressionPtr CreateComparisonExpression (CompareOp op,
					    ExpressionPtr operand1,
					    ExpressionPtr operand2)
  {
    const char* opStr = 0;
    switch (op)
    {
    case Equals:	opStr = "=="; break;
    case NotEquals:	opStr = "!="; break;
    case Smaller:	opStr = "<"; break;
    case SmallerEqual:	opStr = "<="; break;
    case Larger:	opStr = ">"; break;
    case LargerEqual:	opStr = ">="; break;
    }
    return ExpressionPtr (new TestExpressionOp (opStr,
      operand1, operand2));
  }

  ExpressionPtr CreateLogicExpression (LogicOp op,
				       ExpressionPtr operand1,
				       ExpressionPtr operand2)
  {
    const char* opStr = 0;
    switch (op)
    {
    case And:	opStr = "&&"; break;
    case Or:	opStr = "||"; break;
    }
    return ExpressionPtr (new TestExpressionOp (opStr,
      operand1, operand2));
  }
  
  class TestScope : public CommonScope
  {
  public:
    TestScope (TestScope* parent, ScopeLevel level) : CommonScope (parent, level) {}
    
    NamePtr ResolveIdentifier (const UnicodeString& identifier)
    {
      if (Options & testoptIdentifiersSloppy)
	return NamePtr (new CommonName (identifier, Name::Variable));
      return CommonScope::ResolveIdentifier (identifier);
    }
  };
  
  ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel)
  {
    return ScopePtr (new TestScope (static_cast<TestScope*> (parentScope.get()),
      scopeLevel));
  }
};

#endif // __TESTSEMANTICSHANDLER_H__
