#include "base/hash_UnicodeString.h"

#include "parser/CommonSemanticsHandler.h"
#include "parser/Exception.h"

namespace s1
{
  namespace parser
  {
    bool CommonSemanticsHandler::CommonType::CompatibleLossless (const CommonType& to)
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
    
    bool CommonSemanticsHandler::CommonType::CompatibleLossy (const CommonType& to)
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
    
    bool CommonSemanticsHandler::CommonType::IsEqual (const CommonType& other)
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
