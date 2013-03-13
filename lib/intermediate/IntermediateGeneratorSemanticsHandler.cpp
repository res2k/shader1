#include "base/common.h"

#include "intermediate/Exception.h"
#include "FunctionCallExpressionImpl.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "parser/Exception.h"

#include "BlockImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "TypeImpl.h"

#include "ArithmeticExpressionImpl.h"
#include "ArrayElementExpressionImpl.h"
#include "AssignmentExpressionImpl.h"
#include "AttributeExpressionImpl.h"
#include "BoolExpressionImpl.h"
#include "ComparisonExpressionImpl.h"
#include "LogicExpressionImpl.h"
#include "NumericExpressionImpl.h"
#include "TernaryExpressionImpl.h"
#include "TypeConstructorExpressionImpl.h"
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
    
    std::string IntermediateGeneratorSemanticsHandler::GetTypeString (const TypePtr& type)
    {
      return GetTypeString (boost::static_pointer_cast<TypeImpl> (type));
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
    
    IntermediateGeneratorSemanticsHandler::Attribute
    IntermediateGeneratorSemanticsHandler::IdentifyAttribute (const UnicodeString& attributeStr)
    {
      if (attributeStr.length() == 0)
	// Empty attribute? Bogus.
	return Attribute (Attribute::Unknown);
      
      if (attributeStr == UnicodeString ("length"))
	return Attribute (Attribute::arrayLength);
      else if (attributeStr == UnicodeString ("row"))
	return Attribute (Attribute::matrixRow);
      else if (attributeStr == UnicodeString ("col"))
	return Attribute (Attribute::matrixCol);
      else if (attributeStr == UnicodeString ("transpose"))
	return Attribute (Attribute::matrixTranspose);
      else if (attributeStr == UnicodeString ("invert"))
	return Attribute (Attribute::matrixInvert);
      
      // Attribute is swizzle
      if (attributeStr.length() > 4)
	// Attribute too long for a swizzle
	return Attribute (Attribute::Unknown);
      unsigned char attr[4];
      switch (attributeStr[0])
      {
      case 'x':
      case 'y':
      case 'z':
      case 'w':
	{
	  // XYZW style swizzle
	  for (int i = 0; i < attributeStr.length(); i++)
	  {
	    unsigned char comp;
	    switch (attributeStr[i])
	    {
	    case 'x': comp = 0; break;
	    case 'y': comp = 1; break;
	    case 'z': comp = 2; break;
	    case 'w': comp = 3; break;
	    default:
	      // Unknown component
	      return Attribute (Attribute::Unknown);
	    }
	    attr[i] = comp;
	  }
	}
	break;
      case 'r':
      case 'g':
      case 'b':
      case 'a':
	{
	  // RGBA style swizzle
	  for (int i = 0; i < attributeStr.length(); i++)
	  {
	    unsigned char comp;
	    switch (attributeStr[i])
	    {
	    case 'r': comp = 0; break;
	    case 'g': comp = 1; break;
	    case 'b': comp = 2; break;
	    case 'a': comp = 3; break;
	    default:
	      // Unknown component
	      return Attribute (Attribute::Unknown);
	    }
	    attr[i] = comp;
	  }
	}
	break;
      default:
	// Unknown component
	return Attribute (Attribute::Unknown);
      }
      return Attribute (attributeStr.length(),
			attr[0], attr[1], attr[2], attr[3]);
    }
    
    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::GetAttributeType (const TypeImplPtr& expressionType,
							     const Attribute& attr)
    {
      TypePtr attrType;
      switch (attr.attrClass)
      {
      case Attribute::arrayLength:
	if (expressionType->typeClass == TypeImpl::Array)
	  attrType = uintType; // Type is fix
	break;
      case Attribute::matrixCol:
	if (expressionType->typeClass == TypeImpl::Matrix)
	  attrType = CreateArrayType (
	    CreateVectorType (expressionType->avmBase, expressionType->matrixRows));
	break;
      case Attribute::matrixRow:
	if (expressionType->typeClass == TypeImpl::Matrix)
	  attrType = CreateArrayType (
	    CreateVectorType (expressionType->avmBase, expressionType->matrixCols));
	break;
      case Attribute::matrixTranspose:
	if (expressionType->typeClass == TypeImpl::Matrix)
	  attrType = CreateMatrixType (expressionType->avmBase, expressionType->matrixRows, expressionType->matrixCols);
	break;
      case Attribute::matrixInvert:
	if ((expressionType->typeClass == TypeImpl::Matrix)
	    && (expressionType->matrixRows == expressionType->matrixCols))
	  attrType = expressionType;
	break;
      case Attribute::vectorSwizzle:
	if (expressionType->typeClass == TypeImpl::Vector)
	{
	  if (attr.swizzleCompNum == 1)
	    // 1-component swizzles return the base type, not a 1-component vector
	    attrType = expressionType->avmBase;
	  else
	    attrType = CreateVectorType (expressionType->avmBase, attr.swizzleCompNum);
	}
	break;
      case Attribute::Unknown:
	break;
      }
      return boost::static_pointer_cast<TypeImpl> (attrType);
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::AllocateRegister (Sequence& seq,
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
      
      return seq.AllocateRegister (type, regName);
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::AllocateRegister (Sequence& seq,
									 const RegisterPtr& oldReg)
    {
      return seq.AllocateRegister (oldReg);
    }
      
    void IntermediateGeneratorSemanticsHandler::GenerateCast (Sequence& seq,
							      const RegisterPtr& castDestination,
							      const TypeImplPtr& typeDestination,
							      const RegisterPtr& castSource,
							      const TypeImplPtr& typeSource)
    {
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
	// Special case: allow casting from a base type to a vector, replicate value across all components
	if (typeSource->typeClass == TypeImpl::Base)
	{
	  std::vector<RegisterPtr> srcVec;
	  TypeImplPtr destBaseType (boost::static_pointer_cast<TypeImpl> (typeDestination->avmBase));
	  RegisterPtr srcReg;
	  if (!destBaseType->IsEqual (*(typeSource)))
	  {
	    // Generate cast
	    RegisterPtr srcVecReg (AllocateRegister (seq, destBaseType, Intermediate));
	    GenerateCast (seq, srcVecReg, destBaseType, castSource, typeSource);
	    srcReg = srcVecReg;
	  }
	  else
	    srcReg = castSource;
	  srcVec.insert (srcVec.begin(), typeDestination->vectorDim, srcReg);
	  // Generate "make vector" op
	  SequenceOpPtr seqOp;
	  switch (destBaseType->base)
	  {
	    case Bool:
	      seqOp = boost::make_shared<SequenceOpMakeVector> (castDestination,
								intermediate::Bool,
								srcVec);
	      break;
	    case Int:
	      seqOp = boost::make_shared<SequenceOpMakeVector> (castDestination,
								intermediate::Int,
								srcVec);
	      break;
	    case UInt:
	      seqOp = boost::make_shared<SequenceOpMakeVector> (castDestination,
								intermediate::UInt,
								srcVec);
	      break;
	    case Float:
	      seqOp = boost::make_shared<SequenceOpMakeVector> (castDestination,
								intermediate::Float,
								srcVec);
	      break;
	    default:
	      // Void can't be casted
	      break;
	  }
	  if (seqOp)
	  {
	    seq.AddOp (seqOp);
	    return;
	  }
	  break;
	}
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
	  if ((*funcIt)->builtin) continue;
	  
	  boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> ((*funcIt)->block));
	  blockImpl->FinishBlock();
	  
	  parser::SemanticsHandler::Scope::FunctionFormalParameters params ((*funcIt)->params);
	  TypeImplPtr retTypeImpl (boost::static_pointer_cast<TypeImpl> ((*funcIt)->returnType));
	  if (!voidType->IsEqual (*retTypeImpl))
	  {
	    parser::SemanticsHandler::Scope::FunctionFormalParameter retParam;
	    retParam.type = (*funcIt)->returnType;
	    retParam.identifier = BlockImpl::varReturnValueName;
	    retParam.dir = parser::SemanticsHandler::Scope::dirOut;
	    params.insert (params.begin(), retParam);
	  }
	  
	  ProgramFunctionPtr newFunc (boost::make_shared <ProgramFunction> ((*funcIt)->originalIdentifier,
									    (*funcIt)->identifier,
									    params,
									    blockImpl->GetSequence (),
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
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (name));
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
    {
      Attribute attrInfo (IdentifyAttribute (attr));
      if (attrInfo.attrClass == Attribute::Unknown)
	throw Exception (InvalidAttribute);
      return boost::make_shared<AttributeExpressionImpl> (this, expr, attrInfo);
    }
    
    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArrayElementAccess (ExpressionPtr arrayExpr,
										   ExpressionPtr elementIndexExpr)
    {
      return boost::make_shared<ArrayElementExpressionImpl> (this, arrayExpr, elementIndexExpr);
    }
    
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
    { 
      return boost::make_shared<TernaryExpressionImpl> (this,
							boost::static_pointer_cast<ExpressionImpl> (condition),
							boost::static_pointer_cast<ExpressionImpl> (ifExpr),
							boost::static_pointer_cast<ExpressionImpl> (thenExpr));
    }
    
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
    {
      return ExpressionPtr (
	boost::make_shared<TypeConstructorExpressionImpl> (this,
							   boost::static_pointer_cast<TypeImpl> (type),
							   params));
    }
    
    ScopePtr IntermediateGeneratorSemanticsHandler::CreateScope (ScopePtr parentScope,
								 ScopeLevel scopeLevel,
								 const TypePtr& funcReturnType)
    {
      ScopeImplPtr newScope (boost::make_shared<ScopeImpl> (this,
							    boost::static_pointer_cast<ScopeImpl> (parentScope),
							    scopeLevel,
							    funcReturnType));
      switch (scopeLevel)
      {
      case SemanticsHandler::Builtin:
	builtinScope = newScope;
	SetupBuiltins (builtinScope);
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
