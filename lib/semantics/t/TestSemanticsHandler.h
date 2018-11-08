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

#ifndef __TESTSEMANTICSHANDLER_H__
#define __TESTSEMANTICSHANDLER_H__

#include "base/intrusive_ptr.h"
#include "semantics/Attribute.h"
#include "semantics/Block.h"
#include "semantics/CommonHandler.h"
#include "semantics/Expression.h"
#include "semantics/Function.h"
#include "semantics/Name.h"
#include "semantics/Scope.h"
#include "semantics/Type.h"

#include <vector>

class TestSemanticsHandler : public s1::semantics::CommonHandler
{
public:
  class TestScope : public s1::semantics::Scope
  {
    friend class TestSemanticsHandler;
    
    TestSemanticsHandler* handler;
    boost::intrusive_ptr<TestScope> parent;
  public:
    TestScope (TestSemanticsHandler* handler,
	             TestScope* parent, s1::semantics::ScopeLevel level);
    
    s1::semantics::FunctionPtr AddFunction (s1::semantics::TypePtr returnType,
      const s1::uc::String& identifier,
      const s1::semantics::FunctionFormalParameters& params);
  };
  
  s1::semantics::ScopePtr CreateScope (s1::semantics::ScopePtr parentScope, s1::semantics::ScopeLevel scopeLevel)
  {
    return s1::semantics::ScopePtr (new TestScope (this,
      s1::get_static_ptr<TestScope> (parentScope),
      scopeLevel));
  }
  
  struct TestExpressionBase : public s1::semantics::Expression
  {
    virtual const std::string& GetExprStringImpl() = 0;
    virtual s1::semantics::TypePtr GetValueType() = 0;

    static std::string GetExprString (s1::semantics::ExpressionPtr expr);
    std::string GetExprString ();
  };
  
  static std::string GetExprString (TestExpressionBase* expr)
  {
    if (expr)
    {
      return expr->GetExprStringImpl ();
    }
    else
    {
      return "<INVALID-EXPR>";
    }
  }

  static std::string GetExprString (s1::semantics::ExpressionPtr expr)
  {
    return GetExprString (static_cast<TestExpressionBase*> (expr.get ()));
  }

  struct TestExpressionConst : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    TestExpressionConst (const std::string& str,
			 const s1::semantics::TypePtr& valueType) : str (str), valueType (valueType) { }

    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionVar : public TestExpressionBase
  {
    std::string identifier;
    s1::semantics::TypePtr valueType;
    
    TestExpressionVar (const s1::uc::String& identifier,
		       const s1::semantics::TypePtr& valueType) : valueType (valueType)
    {
      identifier.toUTF8String (this->identifier);
    }
    
    const std::string& GetExprStringImpl() override { return identifier; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionOp : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    TestExpressionOp (const char* op, s1::semantics::ExpressionPtr left, s1::semantics::ExpressionPtr right)
    {
      str = "(";
      str.append (GetExprString (left));
      str.append (" ");
      str.append (op);
      str.append (" ");
      str.append (GetExprString (right));
      str.append (")");
      
      if (left) valueType = static_cast<TestExpressionBase*> (left.get())->GetValueType();
    }
    
    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionUnary : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    TestExpressionUnary (const char* op, s1::semantics::ExpressionPtr right)
    {
      str = op;
      str.append (GetExprString (right));
      if (right) valueType = static_cast<TestExpressionBase*> (right.get())->GetValueType();
    }
    
    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionTernary : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    TestExpressionTernary (s1::semantics::ExpressionPtr condition, s1::semantics::ExpressionPtr ifExpr, s1::semantics::ExpressionPtr thenExpr)
    {
      str = "(";
      str.append (GetExprString (condition));
      str.append (" ? ");
      str.append (GetExprString (ifExpr));
      str.append (" : ");
      str.append (GetExprString (thenExpr));
      str.append (")");

      if (ifExpr) valueType = static_cast<TestExpressionBase*> (ifExpr.get())->GetValueType();
    }
    
    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionAttr : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    TestExpressionAttr (s1::semantics::ExpressionPtr base, const s1::uc::String& attr,
			TestSemanticsHandler& handler)
    {
      std::string attrStr;
      attr.toUTF8String (attrStr);
      
      str = GetExprString (base);
      str.append (".");
      str.append (attrStr);
      
      s1::semantics::Attribute attrInfo = s1::semantics::Attribute::Identify (attr);
      s1::semantics::TypePtr baseType = static_cast<TestExpressionBase*> (base.get())->GetValueType();
      if (baseType)
        valueType = s1::semantics::Attribute::GetType (&handler, baseType.get(), attrInfo);
    }
    
    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionArray : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    TestExpressionArray (s1::semantics::ExpressionPtr base, s1::semantics::ExpressionPtr index)
    {
      str = GetExprString(base);
      str.append ("[");
      str.append (GetExprString (index));
      str.append ("]");
      
      s1::semantics::TypePtr baseType;
      if (base) baseType = static_cast<TestExpressionBase*> (base.get ())->GetValueType ();
      if (baseType)
      {
        if (baseType->GetTypeClass() == s1::semantics::Type::Array)
          valueType = baseType->GetArrayBase();
      }
    }
    
    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionFunction : public TestExpressionBase
  {
    std::string str;
    s1::semantics::TypePtr valueType;
    
    void ParamsToStr (const ExpressionVector& params)
    {
      bool first = true;
      for (ExpressionVector::const_iterator exprIt = params.begin(); exprIt != params.end(); ++exprIt)
      {
	if (!first)
	  str.append (", ");
	else
	  first = false;
	
        str.append (GetExprString (*exprIt));
      }
    }
    
    TestExpressionFunction (s1::semantics::NameFunction* name, const ExpressionVector& params)
    {
      {
        name->GetIdentifier().toUTF8String (str);
      }
      
      str.append (" (");
      ParamsToStr (params);
      str.append (")");
      
      // TODO: Resolve overload?
    }
    
    TestExpressionFunction (s1::semantics::Type* type, const ExpressionVector& params)
    {
      {
        type->ToString().toUTF8String (str);
      }
      
      str.append (" (");
      ParamsToStr (params);
      str.append (")");
      
      valueType = type;
    }
    
    const std::string& GetExprStringImpl() override { return str; }
    s1::semantics::TypePtr GetValueType() { return valueType; }
  };
  
  s1::semantics::ExpressionPtr CreateConstBoolExpression (bool value)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionConst (value ? "true" : "false",
						   CreateType (s1::semantics::BaseType::Bool)));
  }
  
  s1::semantics::ExpressionPtr CreateConstNumericExpression (const s1::uc::String& valueStr)
  {
    std::string str;
    valueStr.toUTF8String (str);
    return s1::semantics::ExpressionPtr (new TestExpressionConst (str,
      CreateType (s1::semantics::Type::DetectNumericType (valueStr))));
  }
  
  s1::semantics::ExpressionPtr CreateVariableExpression (s1::semantics::NameVariable* name)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionVar (name->GetIdentifier(), name->GetValueType()));
  }
  
  s1::semantics::ExpressionPtr CreateAttributeAccess (s1::semantics::ExpressionPtr expr,
                                                      const s1::uc::String& attr)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionAttr (expr, attr, *this));
  }
      
  s1::semantics::ExpressionPtr CreateArrayElementAccess (s1::semantics::ExpressionPtr arrayExpr,
                                                         s1::semantics::ExpressionPtr elementExpr)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionArray (arrayExpr, elementExpr));
  }
  
  s1::semantics::ExpressionPtr CreateAssignExpression (s1::semantics::ExpressionPtr target,
                                                       s1::semantics::ExpressionPtr value)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionOp ("=",
      target, value));
  }

  s1::semantics::ExpressionPtr CreateArithmeticExpression (ArithmeticOp op,
                                                           s1::semantics::ExpressionPtr operand1,
                                                           s1::semantics::ExpressionPtr operand2)
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
    return s1::semantics::ExpressionPtr (new TestExpressionOp (opStr,
      operand1, operand2));
  }
  
  s1::semantics::ExpressionPtr CreateUnaryExpression (UnaryOp op,
                                                      s1::semantics::ExpressionPtr operand)
  {
    const char* opStr = 0;
    switch (op)
    {
    case Neg:	opStr = "-"; break;
    case Inv:	opStr = "~"; break;
    case Not:	opStr = "!"; break;
    }
    return s1::semantics::ExpressionPtr (new TestExpressionUnary (opStr, operand));
  }

  s1::semantics::ExpressionPtr CreateTernaryExpression (s1::semantics::ExpressionPtr condition,
                                                        s1::semantics::ExpressionPtr ifExpr,
                                                        s1::semantics::ExpressionPtr thenExpr)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionTernary (condition, ifExpr, thenExpr));
  }

  s1::semantics::ExpressionPtr CreateComparisonExpression (CompareOp op,
                                                           s1::semantics::ExpressionPtr operand1,
                                                           s1::semantics::ExpressionPtr operand2)
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
    return s1::semantics::ExpressionPtr (new TestExpressionOp (opStr,
      operand1, operand2));
  }

  s1::semantics::ExpressionPtr CreateLogicExpression (LogicOp op,
                                                      s1::semantics::ExpressionPtr operand1,
                                                      s1::semantics::ExpressionPtr operand2)
  {
    const char* opStr = 0;
    switch (op)
    {
    case And:	opStr = "&&"; break;
    case Or:	opStr = "||"; break;
    }
    return s1::semantics::ExpressionPtr (new TestExpressionOp (opStr,
      operand1, operand2));
  }
  
  s1::semantics::ExpressionPtr CreateFunctionCallExpression (s1::semantics::NamePtr funcName,
                                                             const ExpressionVector& params)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionFunction (
      s1::get_static_ptr<s1::semantics::NameFunction> (funcName), params));
  }
  
  s1::semantics::ExpressionPtr CreateTypeConstructorExpression (s1::semantics::TypePtr type,
                                                                const ExpressionVector& params)
  {
    return s1::semantics::ExpressionPtr (new TestExpressionFunction (type.get(), params));
  }
    
  class TestBlock : public s1::semantics::Block
  {
    typedef std::vector<std::string> StrVector;
    StrVector blockCommands;

    void Finish() override {}
    
    struct Command
    {
      StrVector commandStrings;
      
      void PushStrings (const StrVector& other)
      {
	commandStrings.insert (commandStrings.end(),
				other.begin(), other.end());
      }
    };
    
    class CommandExpression : public Command
    {
    public:
      CommandExpression (TestExpressionBase* expr)
      {
	this->commandStrings.push_back (GetExprString (expr) + ";");
      }
    };

    class CommandReturn : public Command
    {
    public:
      CommandReturn (TestExpressionBase* expr)
      {
	std::string returnStr ("return");
	if (expr)
	{
	  returnStr.append (" ");
	  returnStr.append (GetExprString (expr));
	}
	returnStr.append (";");
	this->commandStrings.push_back (returnStr);
      }
    };

    class CommandBlock : public Command
    {
    public:
      CommandBlock (const TestBlock* block)
      {
	this->commandStrings.push_back ("{");
	PushStrings (block->blockCommands);
	this->commandStrings.push_back ("}");
      }
    };

    class CommandIf : public Command
    {
    public:
      CommandIf (TestExpressionBase* branchCondition,
		 const TestBlock* ifBlock,
		 const TestBlock* elseBlock)
      {
	std::string condStr ("if (");
	condStr.append (GetExprString (branchCondition));
	condStr.append (")");
	this->commandStrings.push_back (condStr);
	this->commandStrings.push_back ("{");
	PushStrings (ifBlock->blockCommands);
	this->commandStrings.push_back ("}");
	if (elseBlock)
	{
	  this->commandStrings.push_back ("else");
	  this->commandStrings.push_back ("{");
	  PushStrings (elseBlock->blockCommands);
	  this->commandStrings.push_back ("}");
	}
      }
    };

    class CommandWhile : public Command
    {
    public:
      CommandWhile (TestExpressionBase* loopCondition,
		    const TestBlock* block)
      {
	std::string condStr ("while (");
	condStr.append (GetExprString (loopCondition));
	condStr.append (")");
	this->commandStrings.push_back (condStr);
	this->commandStrings.push_back ("{");
	PushStrings (block->blockCommands);
	this->commandStrings.push_back ("}");
      }
    };

    class CommandFor : public Command
    {
    public:
      CommandFor (TestExpressionBase* initExpr,
		  TestExpressionBase* loopCond,
		  TestExpressionBase* tailExpr,
		  const TestBlock* block)
      {
	std::string condStr ("for (");
	condStr.append (GetExprString (initExpr));
	condStr.append ("; ");
	condStr.append (GetExprString (loopCond));
	condStr.append ("; ");
	condStr.append (GetExprString (tailExpr));
	condStr.append (")");
	this->commandStrings.push_back (condStr);
	this->commandStrings.push_back ("{");
	PushStrings (block->blockCommands);
	this->commandStrings.push_back ("}");
      }
    };

    void AddCommands (const Command& cmd)
    {
      const StrVector& cmdStrings = cmd.commandStrings;
      for (StrVector::const_iterator str = cmdStrings.begin(); str != cmdStrings.end(); ++str)
      {
	blockCommands.push_back (std::string ("  ") + *str);
      }
    }
    
    s1::semantics::ScopePtr blockScope;
  public:
    TestBlock (s1::semantics::ScopePtr blockScope) : blockScope (blockScope) {}
    
    std::string GetBlockString()
    {
      std::string s;
      for (StrVector::const_iterator str = blockCommands.begin(); str != blockCommands.end(); ++str)
      {
	s.append (*str);
	s.append ("\n");
      }
      return s;
    }
    
    s1::semantics::ScopePtr GetInnerScope() { return blockScope; }
    
    void AddExpressionCommand (s1::semantics::ExpressionPtr expr)
    {
      CommandExpression cmd (static_cast<TestExpressionBase*> (expr.get()));
      AddCommands (cmd);
    }

    void AddReturnCommand (s1::semantics::ExpressionPtr returnValue)
    {
      CommandReturn cmd (static_cast<TestExpressionBase*> (returnValue.get()));
      AddCommands (cmd);
    }

    void AddBranching (s1::semantics::ExpressionPtr branchCondition, s1::semantics::BlockPtr ifBlock,
                       s1::semantics::BlockPtr elseBlock)
    {
      CommandIf cmd (static_cast<TestExpressionBase*> (branchCondition.get()),
		     static_cast<TestBlock*> (ifBlock.get()),
		     static_cast<TestBlock*> (elseBlock.get()));
      AddCommands (cmd);
    }

    void AddWhileLoop (s1::semantics::ExpressionPtr loopCond, s1::semantics::BlockPtr loopBlock)
    {
      CommandWhile cmd (static_cast<TestExpressionBase*> (loopCond.get()),
			static_cast<TestBlock*> (loopBlock.get()));
      AddCommands (cmd);
    }

    void AddForLoop (s1::semantics::ExpressionPtr initExpr,
                     s1::semantics::ExpressionPtr loopCond,
                     s1::semantics::ExpressionPtr tailExpr,
                     s1::semantics::BlockPtr loopBlock)
    {
      CommandFor cmd (static_cast<TestExpressionBase*> (initExpr.get()),
		      static_cast<TestExpressionBase*> (loopCond.get()),
		      static_cast<TestExpressionBase*> (tailExpr.get()),
		      static_cast<TestBlock*> (loopBlock.get()));
      AddCommands (cmd);
    }

    void AddNestedBlock (s1::semantics::BlockPtr block)
    {
      CommandBlock cmd (static_cast<TestBlock*> (block.get()));
      AddCommands (cmd);
    }
  };
  
  s1::semantics::BlockPtr CreateBlock (s1::semantics::ScopePtr parentScope)
  {
    s1::semantics::ScopePtr blockScope = CreateScope (parentScope, s1::semantics::ScopeLevel::Function);
    return s1::semantics::BlockPtr (new TestBlock (blockScope));
  }
};

inline std::string TestSemanticsHandler::TestExpressionBase::GetExprString (s1::semantics::ExpressionPtr expr)
{
  return TestSemanticsHandler::GetExprString (expr);
}

inline std::string TestSemanticsHandler::TestExpressionBase::GetExprString()
{
  return TestSemanticsHandler::GetExprString (this);
}

#endif // __TESTSEMANTICSHANDLER_H__
