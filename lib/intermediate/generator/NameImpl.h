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

#ifndef __INTERMEDIATE_NAMEIMPL_H__
#define __INTERMEDIATE_NAMEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    struct IntermediateGeneratorSemanticsHandler::NameImpl : public Name
    {
      boost::weak_ptr<ScopeImpl> ownerScope;
      uc::String identifier;
      NameType type;
      
      /* Variables/Constants: type of variable/constant
	* Functions: type of return value
	* Type aliases: aliased type
	*/
      boost::shared_ptr<TypeImpl> valueType;
      // Variables/Constants: value
      ExpressionPtr varValue;
      // Distinguish between variable/constant
      bool varConstant;
      // Output parameter
      bool isOutputParam;
      
      NameImpl (const boost::weak_ptr<ScopeImpl>& ownerScope,
		const uc::String& identifier, NameType type,
		const boost::shared_ptr<TypeImpl>& typeOfName)
	: ownerScope (ownerScope), identifier (identifier), type (type), valueType (typeOfName) {}
      NameImpl (const boost::weak_ptr<ScopeImpl>& ownerScope,
		const uc::String& identifier,
		const boost::shared_ptr<TypeImpl>& typeOfName,
		ExpressionPtr value, bool constant, bool isOutputParam = false)
	: ownerScope (ownerScope), identifier (identifier), type (Variable), valueType (typeOfName),
	  varValue (value), varConstant (constant), isOutputParam (isOutputParam) {}
      
      NameType GetType() { return type; }
      TypePtr GetAliasedType()
      { return type == TypeAlias ? boost::static_pointer_cast<Type> (valueType) : TypePtr (); }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NAMEIMPL_H__
