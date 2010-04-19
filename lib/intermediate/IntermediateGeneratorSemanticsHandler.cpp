// Must be first; otherwise, link errors may occur
#include "base/hash_UnicodeString.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "parser/Exception.h"

#include "BlockImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    typedef IntermediateGeneratorSemanticsHandler::NamePtr NamePtr;
    typedef IntermediateGeneratorSemanticsHandler::ScopePtr ScopePtr;
    typedef IntermediateGeneratorSemanticsHandler::BlockPtr BlockPtr;
    typedef IntermediateGeneratorSemanticsHandler::TypePtr TypePtr;
    
    TypePtr IntermediateGeneratorSemanticsHandler::CreateType (BaseType type)
    {
      return TypePtr (new TypeImpl (type));
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
  
    ScopePtr IntermediateGeneratorSemanticsHandler::CreateScope (ScopePtr parentScope,
								 ScopeLevel scopeLevel)
    {
      return ScopePtr (new ScopeImpl (this,
	boost::shared_static_cast<ScopeImpl> (parentScope),
	scopeLevel));
    }
      
    BlockPtr IntermediateGeneratorSemanticsHandler::CreateBlock (ScopePtr parentScope)
    {
      ScopePtr blockScope = CreateScope (parentScope, Function);
      return BlockPtr (new BlockImpl (blockScope));
    }
  
  } // namespace intermediate
} // namespace s1
