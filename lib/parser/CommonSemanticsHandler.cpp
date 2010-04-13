#include "base/hash_UnicodeString.h"

#include "parser/CommonSemanticsHandler.h"
#include "parser/Exception.h"

namespace s1
{
  namespace parser
  {
    bool CommonSemanticsHandler::CommonType::CompatibleLossless (const CommonType& to) const
    {
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
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
	return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
	  *(static_cast<CommonType*> (to.avmBase.get())));
      case Vector:
	// Vectors: base types must be compatible and both types have the same number of components
	return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
	  *(static_cast<CommonType*> (to.avmBase.get())))
	  && (vectorDim == to.vectorDim);
      case Matrix:
	// Matrices: base types must be compatible and both types have the same number of rows/cols
	return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
	  *(static_cast<CommonType*> (to.avmBase.get())))
	  && (matrixCols == to.matrixCols)
	  && (matrixRows == to.matrixRows);
      }
      assert (false);
      return false;
    }
    
    bool CommonSemanticsHandler::CommonType::CompatibleLossy (const CommonType& to) const
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
      }
      return false;
    }
    
    bool CommonSemanticsHandler::CommonType::IsEqual (const CommonType& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
	return base == other.base;
      case Sampler:
	return sampler == other.sampler;
      case Array:
	return static_cast<CommonType*> (avmBase.get())->IsEqual (
	  *(static_cast<CommonType*> (other.avmBase.get())));
      case Vector:
	return static_cast<CommonType*> (avmBase.get())->IsEqual (
	  *(static_cast<CommonType*> (other.avmBase.get())))
	  && (vectorDim == other.vectorDim);
      case Matrix:
	return static_cast<CommonType*> (avmBase.get())->IsEqual (
	  *(static_cast<CommonType*> (other.avmBase.get())))
	  && (matrixCols == other.matrixCols)
	  && (matrixRows == other.matrixRows);
      }
      assert (false);
      return false;
    }

    bool CommonSemanticsHandler::CommonType::IsPrecisionHigherEqual (const CommonType& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
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
	return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
	  *(static_cast<CommonType*> (other.avmBase.get())));
      case Vector:
	// Vectors: base type is higher/equal prec if both types have the same number of components
	return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
	  *(static_cast<CommonType*> (other.avmBase.get())))
	  && (vectorDim == other.vectorDim);
      case Matrix:
	// Matrices: base type is higher/equal prec if both types have the same number of rows/cols
	return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
	  *(static_cast<CommonType*> (other.avmBase.get())))
	  && (matrixCols == other.matrixCols)
	  && (matrixRows == other.matrixRows);
      }
      assert (false);
      return false;
    }
    
    boost::shared_ptr<CommonSemanticsHandler::CommonType>
    CommonSemanticsHandler::GetHigherPrecisionType (
      const boost::shared_ptr<CommonType>& t1, const boost::shared_ptr<CommonType>& t2)
    {
      if (t1->IsPrecisionHigherEqual (*t2))
	return t1;
      else if (t2->IsPrecisionHigherEqual (*t1))
	return t2;
      return boost::shared_ptr<CommonSemanticsHandler::CommonType> ();
    }
      
    CommonSemanticsHandler::BaseType CommonSemanticsHandler::DetectNumericType (const UnicodeString& numericStr)
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
    
    CommonSemanticsHandler::Attribute CommonSemanticsHandler::IdentifyAttribute (const UnicodeString& attributeStr)
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
    
    CommonSemanticsHandler::TypePtr
    CommonSemanticsHandler::GetAttributeType (const boost::shared_ptr<CommonType>& expressionType,
					      const Attribute& attr)
    {
      TypePtr attrType;
      switch (attr.attrClass)
      {
      case Attribute::arrayLength:
	if (expressionType->typeClass == CommonType::Array)
	  attrType = CreateType (UInt); // Type is fix
	break;
      case Attribute::matrixCol:
	if (expressionType->typeClass == CommonType::Matrix)
	  attrType = CreateArrayType (
	    CreateVectorType (expressionType->avmBase, expressionType->matrixRows));
	break;
      case Attribute::matrixRow:
	if (expressionType->typeClass == CommonType::Matrix)
	  attrType = CreateArrayType (
	    CreateVectorType (expressionType->avmBase, expressionType->matrixCols));
	break;
      case Attribute::matrixTranspose:
	if (expressionType->typeClass == CommonType::Matrix)
	  attrType = CreateMatrixType (expressionType->avmBase, expressionType->matrixRows, expressionType->matrixCols);
	break;
      case Attribute::matrixInvert:
	if ((expressionType->typeClass == CommonType::Matrix)
	    && (expressionType->matrixRows == expressionType->matrixCols))
	  attrType = expressionType;
	break;
      case Attribute::vectorSwizzle:
	if (expressionType->typeClass == CommonType::Vector)
	{
	  if (attr.swizzleCompNum == 1)
	    // 1-component swizzles return the base type, not a 1-component vector
	    attrType = expressionType->avmBase;
	  else
	    attrType = CreateVectorType (expressionType->avmBase, attr.swizzleCompNum);
	}
	break;
      }
      return attrType;
    }
    
    typedef CommonSemanticsHandler::TypePtr TypePtr;
    
    TypePtr CommonSemanticsHandler::CreateType (BaseType type)
    {
      return TypePtr (new CommonType (type));
    }
    
    TypePtr CommonSemanticsHandler::CreateSamplerType (SamplerType dim)
    {
      return TypePtr (new CommonType (dim));
    }
    
    TypePtr CommonSemanticsHandler::CreateArrayType (TypePtr baseType)
    {
      return TypePtr (new CommonType (baseType));
    }
    
    TypePtr CommonSemanticsHandler::CreateVectorType (TypePtr baseType,
						      unsigned int components)
    {
      return TypePtr (new CommonType (baseType, components));
    }
    
    TypePtr CommonSemanticsHandler::CreateMatrixType (TypePtr baseType,
						      unsigned int columns,
						      unsigned int rows)
    {
      return TypePtr (new CommonType (baseType, columns, rows));
    }
  
    void CommonSemanticsHandler::CommonScope::CheckIdentifierUnique (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	throw Exception (parser::IdentifierAlreadyDeclared);
      }
      if (parent)
	parent->CheckIdentifierUnique (identifier);
    }
      
    CommonSemanticsHandler::CommonScope::CommonScope (CommonScope* parent, ScopeLevel level)
     : parent (parent), level (level)
    {}
      
    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::AddVariable (TypePtr type, const UnicodeString& identifier,
						      ExpressionPtr initialValue, bool constant)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new CommonName (identifier, type, initialValue, constant));
      identifiers[identifier] = newName;
      return newName;
    }
      
    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::AddTypeAlias (TypePtr aliasedType, const UnicodeString& identifier)
    {
      CheckIdentifierUnique (identifier);
      NamePtr newName (new CommonName (identifier, Name::TypeAlias, aliasedType));
      identifiers[identifier] = newName;
      return newName;
    }
      
    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::AddFunction (TypePtr returnType, const UnicodeString& identifier)
    {
      if (level >= Function)
	throw Exception (DeclarationNotAllowedInScope);
      CheckIdentifierUnique (identifier);
      NamePtr newName (new CommonName (identifier, Name::Function, returnType));
      identifiers[identifier] = newName;
      return newName;
    }

    CommonSemanticsHandler::NamePtr
    CommonSemanticsHandler::CommonScope::ResolveIdentifier (const UnicodeString& identifier)
    {
      IdentifierMap::iterator ident = identifiers.find (identifier);
      if (ident != identifiers.end())
      {
	return ident->second;
      }
      if (parent)
	return parent->ResolveIdentifier (identifier);
      throw Exception (IdentifierUndeclared);
    }
    
    CommonSemanticsHandler::ScopePtr CommonSemanticsHandler::CreateScope (ScopePtr parentScope,
									  ScopeLevel scopeLevel)
    {
      return ScopePtr (new CommonScope (static_cast<CommonScope*> (parentScope.get()),
	scopeLevel));
    }
    
  } // namespace parser
} // namespace s1