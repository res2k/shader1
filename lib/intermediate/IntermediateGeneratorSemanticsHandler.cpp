#include "base/common.h"

// Must be first; otherwise, link errors may occur
#include "base/hash_UnicodeString.h"

#include "intermediate/Exception.h"
#include "FunctionCallExpressionImpl.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"

#include "parser/Exception.h"

#include "BlockImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "TypeImpl.h"

#include "ArithmeticExpressionImpl.h"
#include "AssignmentExpressionImpl.h"
#include "BoolExpressionImpl.h"
#include "ComparisonExpressionImpl.h"
#include "LogicExpressionImpl.h"
#include "NumericExpressionImpl.h"
#include "UnaryExpressionImpl.h"
#include "VariableExpressionImpl.h"

#include <boost/make_shared.hpp>
#include <unicode/ustdio.h>

namespace s1
{
  namespace intermediate
  {
    typedef IntermediateGeneratorSemanticsHandler::NamePtr NamePtr;
    typedef IntermediateGeneratorSemanticsHandler::ScopePtr ScopePtr;
    typedef IntermediateGeneratorSemanticsHandler::BlockPtr BlockPtr;
    typedef IntermediateGeneratorSemanticsHandler::TypePtr TypePtr;
    typedef IntermediateGeneratorSemanticsHandler::ExpressionPtr ExpressionPtr;
    
    IntermediateGeneratorSemanticsHandler::IntermediateGeneratorSemanticsHandler ()
    {
      voidType = boost::shared_ptr<TypeImpl> (new TypeImpl (Void));
      boolType = boost::shared_ptr<TypeImpl> (new TypeImpl (Bool));
      intType = boost::shared_ptr<TypeImpl> (new TypeImpl (Int));
      uintType = boost::shared_ptr<TypeImpl> (new TypeImpl (UInt));
      floatType = boost::shared_ptr<TypeImpl> (new TypeImpl (Float));
    }
    
    IntermediateGeneratorSemanticsHandler::~IntermediateGeneratorSemanticsHandler ()
    {
    }
    
    std::string IntermediateGeneratorSemanticsHandler::GetTypeString (const TypeImplPtr& type)
    {
      switch (type->typeClass)
      {
	case TypeImpl::Base:
	{
	  switch (type->base)
	  {
	    case Void: return "V";
	    case Bool: return "B";
	    case Int: return "I";
	    case UInt: return "U";
	    case Float: return "F";
	  }
	}
	break;
      case TypeImpl::Sampler:
	{
	  switch (type->sampler)
	  {
	    case _1D: return "S1";
	    case _2D: return "S2";
	    case _3D: return "S3";
	    case CUBE: return "SC";
	  }
	}
	break;
      case TypeImpl::Array:
	{
	  return std::string ("a") + GetTypeString (boost::static_pointer_cast<TypeImpl> (type->avmBase));
	}
      case TypeImpl::Vector:
	{
	  char nStr[2];
	  snprintf (nStr, sizeof (nStr), "%d", type->vectorDim);
	  return std::string ("v") + GetTypeString (boost::static_pointer_cast<TypeImpl> (type->avmBase))
	    + std::string (nStr);
	}
      case TypeImpl::Matrix:
	{
	  char nStr[4];
	  snprintf (nStr, sizeof (nStr), "%dx%d", type->matrixCols, type->matrixRows);
	  return std::string ("m") + GetTypeString (boost::static_pointer_cast<TypeImpl> (type->avmBase))
	    + std::string (nStr);
	}
      }
      assert (false);
      return std::string ();
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
      const boost::shared_ptr<TypeImpl>& t1, const boost::shared_ptr<TypeImpl>& t2)
    {
      if (t1->IsPrecisionHigherEqual (*t2))
	return t1;
      else if (t2->IsPrecisionHigherEqual (*t1))
	return t2;
      return boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl> ();
    }
      
    IntermediateGeneratorSemanticsHandler::BaseType
    IntermediateGeneratorSemanticsHandler::DetectNumericType (const UnicodeString& numericStr)
    {
      if (numericStr.startsWith (UnicodeString ("0x")) || numericStr.startsWith (UnicodeString ("0X")))
      {
	// Hex number: always unsigned int
	return UInt;
      }
      if ((numericStr.indexOf ('.') != -1) || (numericStr.indexOf ('e') != -1) || (numericStr.indexOf ('E') != -1))
      {
	// Contains '.', 'e' or 'E': must be float number
	return Float;
      }
      // Can only be an integer
      return numericStr.startsWith (UnicodeString ("-")) ? Int : UInt;
    }
    
    RegisterID IntermediateGeneratorSemanticsHandler::AllocateRegister (Sequence& seq,
									const TypeImplPtr& type,
									RegisterClassification classify,
									const UnicodeString& name)
    {
      const UChar prefix[3] = { UChar (classify), '_', 0};
      UnicodeString regName (prefix);
      if (!name.isEmpty())
	regName.append (name);
      else
      {
	static unsigned int allRegNum = 0;
	UChar regNumStr[64];
	u_snprintf (regNumStr, sizeof (regNumStr)/sizeof (UChar),
		    "tmp%u", allRegNum++);
	regName.append (regNumStr);
      }
      
      std::string typeStr (GetTypeString (type));
      return seq.AllocateRegister (typeStr, type, regName);
    }

    RegisterID IntermediateGeneratorSemanticsHandler::AllocateRegister (Sequence& seq,
									const RegisterID& oldReg)
    {
      return seq.AllocateRegister (oldReg);
    }
      
    void IntermediateGeneratorSemanticsHandler::GenerateCast (Sequence& seq,
							      const RegisterID& castDestination,
							      const TypeImplPtr& typeDestination,
							      const RegisterID& castSource,
							      const TypeImplPtr& typeSource)
    {
      assert (typeDestination->typeClass == typeSource->typeClass);
      
      switch (typeDestination->typeClass)
      {
	case TypeImpl::Base:
	{
	  SequenceOpPtr seqOp;
	  switch (typeDestination->base)
	  {
	    case Int:
	      seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::Int, castSource));
	      break;
	    case UInt:
	      seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::UInt, castSource));
	      break;
	    case Float:
	      seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::Float, castSource));
	      break;
	    default:
	      // Void, Bool can't be casted
	      break;
	  }
	  if (seqOp)
	  {
	    seq.AddOp (seqOp);
	    return;
	  }
	  break;
	}
      case TypeImpl::Sampler:
	// Cannot cast samplers
	break;
      case TypeImpl::Array:
	// TODO: Cast individual elements, if lengths match
	break;
      case TypeImpl::Vector:
	assert (typeDestination->vectorDim == typeSource->vectorDim);
	// TODO: Cast individual components
	break;
      case TypeImpl::Matrix:
	assert (typeDestination->matrixCols == typeSource->matrixCols);
	assert (typeDestination->matrixRows == typeSource->matrixRows);
	// TODO: Cast individual components
	break;
      }
      
      throw Exception (InvalidTypeCast);
    }

    ProgramPtr IntermediateGeneratorSemanticsHandler::GetProgram ()
    {
      ProgramPtr newProg (boost::make_shared <Program> ());
      if (globalScope)
      {
	ScopeImpl::FunctionInfoVector functions (globalScope->GetFunctions());
	for (ScopeImpl::FunctionInfoVector::const_iterator funcIt = functions.begin();
	     funcIt != functions.end();
	     ++funcIt)
	{
	  ProgramFunctionPtr newFunc (boost::make_shared <ProgramFunction> ((*funcIt)->identifier,
									    (*funcIt)->returnType,
									    (*funcIt)->params,
									    (*funcIt)->block,
									    IsEntryFunction ((*funcIt)->originalIdentifier)));
	  newProg->AddFunction (newFunc);
	}
      }
      return newProg;
    }
    
    TypePtr IntermediateGeneratorSemanticsHandler::CreateType (BaseType type)
    {
      switch (type)
      {
      case Void: return voidType;
      case Bool: return boolType;
      case Int: return intType;
      case UInt: return uintType;
      case Float: return floatType;
      }
      assert (false);
    }
    
    TypePtr IntermediateGeneratorSemanticsHandler::CreateSamplerType (SamplerType dim)
    {
      return TypePtr (new TypeImpl (dim));
    }
    
    TypePtr IntermediateGeneratorSemanticsHandler::CreateArrayType (TypePtr baseType)
    {
      return TypePtr (new TypeImpl (baseType));
    }
    
    TypePtr IntermediateGeneratorSemanticsHandler::CreateVectorType (TypePtr baseType,
						      unsigned int components)
    {
      return TypePtr (new TypeImpl (baseType, components));
    }
    
    TypePtr IntermediateGeneratorSemanticsHandler::CreateMatrixType (TypePtr baseType,
						      unsigned int columns,
						      unsigned int rows)
    {
      return TypePtr (new TypeImpl (baseType, columns, rows));
    }
  
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstBoolExpression (bool value)
    {
      return ExpressionPtr (new BoolExpressionImpl (this, value));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstNumericExpression (const UnicodeString& valueStr)
    {
      return ExpressionPtr (new NumericExpressionImpl (this, valueStr));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateVariableExpression (NamePtr name)
    {
      boost::shared_ptr<NameImpl> nameImpl (boost::shared_static_cast<NameImpl> (name));
      switch (nameImpl->GetType())
      {
      case Name::Variable:
	{
	  return ExpressionPtr (new VariableExpressionImpl (this, nameImpl));
	}
	break;
      default:
	// TODO: Throw or so
	break;
      }
      return ExpressionPtr();
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAttributeAccess (ExpressionPtr expr,
					  const UnicodeString& attr)
    { return ExpressionPtr(); }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArrayElementAccess (ExpressionPtr arrayExpr,
					    ExpressionPtr elementIndexExpr)
    { return ExpressionPtr(); }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAssignExpression (ExpressionPtr target,
					  ExpressionPtr value)
    {
      return ExpressionPtr (new AssignmentExpressionImpl (this,
							  boost::static_pointer_cast<ExpressionImpl> (target),
							  boost::static_pointer_cast<ExpressionImpl> (value)));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArithmeticExpression (ArithmeticOp op,
										     ExpressionPtr operand1,
										     ExpressionPtr operand2)
    {
      return ExpressionPtr (new ArithmeticExpressionImpl (this,
							  op,
							  boost::static_pointer_cast<ExpressionImpl> (operand1),
							  boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateUnaryExpression (UnaryOp op,
					  ExpressionPtr operand)
						  
    {
      return boost::make_shared<UnaryExpressionImpl> (this, op,
						      boost::static_pointer_cast<ExpressionImpl> (operand));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTernaryExpression (ExpressionPtr condition,
					    ExpressionPtr ifExpr,
					    ExpressionPtr thenExpr)
    { return ExpressionPtr(); }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateComparisonExpression (CompareOp op,
					      ExpressionPtr operand1,
					      ExpressionPtr operand2)
    {
      return ExpressionPtr (
	boost::make_shared<ComparisonExpressionImpl> (this,
						      op,
						      boost::static_pointer_cast<ExpressionImpl> (operand1),
						      boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateLogicExpression (LogicOp op,
					  ExpressionPtr operand1,
					  ExpressionPtr operand2)
    {
      return ExpressionPtr (
	boost::make_shared<LogicExpressionImpl> (this,
						 op,
						 boost::static_pointer_cast<ExpressionImpl> (operand1),
						 boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateFunctionCallExpression (NamePtr functionName,
										       const ExpressionVector& params)
    { 
      assert (functionName->GetType() == SemanticsHandler::Name::Function);
      
      return ExpressionPtr (
	boost::make_shared<FunctionCallExpressionImpl> (this,
							functionName,
							params));
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTypeConstructorExpression (TypePtr type,
						    const ExpressionVector& params)
    { return ExpressionPtr(); }
    
    ScopePtr IntermediateGeneratorSemanticsHandler::CreateScope (ScopePtr parentScope,
								 ScopeLevel scopeLevel,
								 const TypePtr& funcReturnType)
    {
      ScopeImplPtr newScope (boost::make_shared<ScopeImpl> (this,
							    boost::shared_static_cast<ScopeImpl> (parentScope),
							    scopeLevel,
							    funcReturnType));
      switch (scopeLevel)
      {
      case Builtin:
	builtinScope = newScope;
	break;
      case Global:
	globalScope = newScope;
	break;
      default:
	break;
      }
      return newScope;
    }
      
    BlockPtr IntermediateGeneratorSemanticsHandler::CreateBlock (ScopePtr parentScope)
    {
      ScopePtr blockScope = CreateScope (parentScope, Function);
      return BlockPtr (new BlockImpl (this, blockScope));
    }
  
  } // namespace intermediate
} // namespace s1
