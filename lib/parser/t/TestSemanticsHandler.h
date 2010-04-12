#ifndef __TESTSEMANTICSHANDLER_H__
#define __TESTSEMANTICSHANDLER_H__

#include "parser/CommonSemanticsHandler.h"

#include <vector>

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
  typedef CommonType TestType;
  
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
