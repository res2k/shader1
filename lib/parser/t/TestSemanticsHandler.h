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

#include "parser/CommonSemanticsHandler.h"

#include <vector>

class TestSemanticsHandler : public s1::parser::CommonSemanticsHandler
{
public:
  class TestScope : public Scope,
		    public boost::enable_shared_from_this<TestScope>
  {
    friend class TestSemanticsHandler;
    
    typedef boost::unordered_map<s1::uc::String, NamePtr> IdentifierMap;
    IdentifierMap identifiers;
    
    void CheckIdentifierUnique (const s1::uc::String& identifier);
    
    TestSemanticsHandler* handler;
    boost::shared_ptr<TestScope> parent;
    ScopeLevel level;

    class TestFunction : public Function
    {
      BlockPtr block;
    public:
      TestFunction (const BlockPtr& block) : block (block) {}
      BlockPtr GetBody() { return block; }
      void Finish() {}
    };
  public:
    TestScope (TestSemanticsHandler* handler,
	       const boost::shared_ptr<TestScope>& parent, ScopeLevel level);
    ScopeLevel GetLevel() const { return level; }
    
    NamePtr AddVariable (TypePtr type,
      const s1::uc::String& identifier,
      ExpressionPtr initialValue,
      bool constant);
      
    NamePtr AddTypeAlias (TypePtr aliasedType,
      const s1::uc::String& identifier);
      
    FunctionPtr AddFunction (TypePtr returnType,
      const s1::uc::String& identifier,
      const FunctionFormalParameters& params);
  
    NamePtr ResolveIdentifier (const s1::uc::String& identifier);
  };
  
  ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel)
  {
    return ScopePtr (new TestScope (this,
      boost::static_pointer_cast<TestScope> (parentScope),
      scopeLevel));
  }
  
  typedef CommonType TestType;
  typedef CommonName TestName;
  
  struct TestExpressionBase : public Expression
  {
    virtual const std::string& GetExprString() = 0;
    virtual TypePtr GetValueType() = 0;
  };
  
  struct TestExpressionConst : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    TestExpressionConst (const std::string& str,
			 const TypePtr& valueType) : str (str), valueType (valueType) { }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionVar : public TestExpressionBase
  {
    std::string identifier;
    TypePtr valueType;
    
    TestExpressionVar (const s1::uc::String& identifier,
		       const TypePtr& valueType) : valueType (valueType)
    {
      identifier.toUTF8String (this->identifier);
    }
    
    const std::string& GetExprString() { return identifier; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionOp : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    TestExpressionOp (const char* op, ExpressionPtr left, ExpressionPtr right)
    {
      str = "(";
      str.append (static_cast<TestExpressionBase*> (left.get())->GetExprString());
      str.append (" ");
      str.append (op);
      str.append (" ");
      str.append (static_cast<TestExpressionBase*> (right.get())->GetExprString());
      str.append (")");
      
      valueType = static_cast<TestExpressionBase*> (left.get())->GetValueType();
    }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionUnary : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    TestExpressionUnary (const char* op, ExpressionPtr right)
    {
      str = op;
      str.append (static_cast<TestExpressionBase*> (right.get())->GetExprString());
      valueType = static_cast<TestExpressionBase*> (right.get())->GetValueType();
    }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionTernary : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    TestExpressionTernary (ExpressionPtr condition, ExpressionPtr ifExpr, ExpressionPtr thenExpr)
    {
      str = "(";
      str.append (static_cast<TestExpressionBase*> (condition.get())->GetExprString());
      str.append (" ? ");
      str.append (static_cast<TestExpressionBase*> (ifExpr.get())->GetExprString());
      str.append (" : ");
      str.append (static_cast<TestExpressionBase*> (thenExpr.get())->GetExprString());
      str.append (")");
      
      valueType = static_cast<TestExpressionBase*> (ifExpr.get())->GetValueType();
    }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionAttr : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    TestExpressionAttr (ExpressionPtr base, const s1::uc::String& attr,
			TestSemanticsHandler& handler)
    {
      std::string attrStr;
      attr.toUTF8String (attrStr);
      
      str = static_cast<TestExpressionBase*> (base.get())->GetExprString();
      str.append (".");
      str.append (attrStr);
      
      Attribute attrInfo = IdentifyAttribute (attr);
      TypePtr baseType = static_cast<TestExpressionBase*> (base.get())->GetValueType();
      if (baseType)
	valueType = boost::static_pointer_cast<CommonType> (handler.GetAttributeType (
	  boost::static_pointer_cast<TestType> (baseType), attrInfo));
    }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionArray : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    TestExpressionArray (ExpressionPtr base, ExpressionPtr index)
    {
      str = static_cast<TestExpressionBase*> (base.get())->GetExprString();
      str.append ("[");
      str.append (static_cast<TestExpressionBase*> (index.get())->GetExprString());
      str.append ("]");
      
      TypePtr baseType = static_cast<TestExpressionBase*> (base.get())->GetValueType();
      if (baseType)
      {
	TestType* testBaseType = static_cast<TestType*> (baseType.get());
	if (testBaseType->typeClass == TestType::Array)
	  valueType = testBaseType->avmBase;
      }
    }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  struct TestExpressionFunction : public TestExpressionBase
  {
    std::string str;
    TypePtr valueType;
    
    void ParamsToStr (const ExpressionVector& params)
    {
      bool first = true;
      for (ExpressionVector::const_iterator exprIt = params.begin(); exprIt != params.end(); ++exprIt)
      {
	if (!first)
	  str.append (", ");
	else
	  first = false;
	
	str.append (
	  boost::static_pointer_cast<TestExpressionBase> (*exprIt)->GetExprString());
      }
    }
    
    TestExpressionFunction (const boost::shared_ptr<TestName>& name,
			    const ExpressionVector& params)
    {
      {
	name->identifier.toUTF8String (str);
      }
      
      str.append (" (");
      ParamsToStr (params);
      str.append (")");
      
      valueType = name->valueType;
    }
    
    TestExpressionFunction (const boost::shared_ptr<TestType>& type,
			    const ExpressionVector& params)
    {
      {
	type->ToString().toUTF8String (str);
      }
      
      str.append (" (");
      ParamsToStr (params);
      str.append (")");
      
      valueType = type;
    }
    
    const std::string& GetExprString() { return str; }
    TypePtr GetValueType() { return valueType; }
  };
  
  ExpressionPtr CreateConstBoolExpression (bool value)
  {
    return ExpressionPtr (new TestExpressionConst (value ? "true" : "false",
						   CreateType (Bool)));
  }
  
  ExpressionPtr CreateConstNumericExpression (const s1::uc::String& valueStr)
  {
    std::string str;
    valueStr.toUTF8String (str);
    return ExpressionPtr (new TestExpressionConst (str,
      CreateType (DetectNumericType (valueStr))));
  }
  
  ExpressionPtr CreateVariableExpression (NamePtr name)
  {
    return ExpressionPtr (new TestExpressionVar (
      static_cast<CommonName*> (name.get())->identifier,
      static_cast<CommonName*> (name.get())->valueType));
  }
  
  ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
				       const s1::uc::String& attr)
  {
    return ExpressionPtr (new TestExpressionAttr (expr, attr, *this));
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
  
  ExpressionPtr CreateFunctionCallExpression (NamePtr funcName,
					      const ExpressionVector& params)
  {
    return ExpressionPtr (new TestExpressionFunction (
      boost::static_pointer_cast<TestName> (funcName), params));
  }
  
  ExpressionPtr CreateTypeConstructorExpression (TypePtr type,
						 const ExpressionVector& params)
  {
    return ExpressionPtr (new TestExpressionFunction (
      boost::static_pointer_cast<TestType> (type), params));
  }
    
  class TestBlock : public Block
  {
    typedef std::vector<std::string> StrVector;
    StrVector blockCommands;
    
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
	this->commandStrings.push_back (expr->GetExprString() + ";");
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
	  returnStr.append (expr->GetExprString());
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
	condStr.append (branchCondition->GetExprString());
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
	condStr.append (loopCondition->GetExprString());
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
	condStr.append (initExpr->GetExprString());
	condStr.append ("; ");
	condStr.append (loopCond->GetExprString());
	condStr.append ("; ");
	condStr.append (tailExpr->GetExprString());
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
    
    ScopePtr blockScope;
  public:
    TestBlock (ScopePtr blockScope) : blockScope (blockScope) {}
    
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
    
    ScopePtr GetInnerScope() { return blockScope; }
    
    void AddExpressionCommand (ExpressionPtr expr)
    {
      CommandExpression cmd (static_cast<TestExpressionBase*> (expr.get()));
      AddCommands (cmd);
    }
    
    void AddReturnCommand (ExpressionPtr returnValue)
    {
      CommandReturn cmd (static_cast<TestExpressionBase*> (returnValue.get()));
      AddCommands (cmd);
    }
    
    void AddBranching (ExpressionPtr branchCondition, BlockPtr ifBlock,
		       BlockPtr elseBlock)
    {
      CommandIf cmd (static_cast<TestExpressionBase*> (branchCondition.get()),
		     static_cast<TestBlock*> (ifBlock.get()),
		     static_cast<TestBlock*> (elseBlock.get()));
      AddCommands (cmd);
    }
    
    void AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock)
    {
      CommandWhile cmd (static_cast<TestExpressionBase*> (loopCond.get()),
			static_cast<TestBlock*> (loopBlock.get()));
      AddCommands (cmd);
    }
    
    void AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond, ExpressionPtr tailExpr,
		     BlockPtr loopBlock)
    {
      CommandFor cmd (static_cast<TestExpressionBase*> (initExpr.get()),
		      static_cast<TestExpressionBase*> (loopCond.get()),
		      static_cast<TestExpressionBase*> (tailExpr.get()),
		      static_cast<TestBlock*> (loopBlock.get()));
      AddCommands (cmd);
    }
			      
    void AddNestedBlock (BlockPtr block)
    {
      CommandBlock cmd (static_cast<TestBlock*> (block.get()));
      AddCommands (cmd);
    }
  };
  
  BlockPtr CreateBlock (ScopePtr parentScope)
  {
    ScopePtr blockScope = CreateScope (parentScope, Function);
    return BlockPtr (new TestBlock (blockScope));
  }
};

#endif // __TESTSEMANTICSHANDLER_H__
