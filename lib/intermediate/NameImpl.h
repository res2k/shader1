#ifndef __INTERMEDIATE_NAMEIMPL_H__
#define __INTERMEDIATE_NAMEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    struct IntermediateGeneratorSemanticsHandler::NameImpl : public Name
    {
      UnicodeString identifier;
      NameType type;
      
      /* Variables/Constants: type of variable/constant
	* Functions: type of return value
	* Type aliases: aliased type
	*/
      TypePtr valueType;
      // Variables/Constants: value
      ExpressionPtr varValue;
      // Distinguish between variable/constant
      bool varConstant;
      
      NameImpl (const UnicodeString& identifier, NameType type, TypePtr typeOfName)
	: identifier (identifier), type (type), valueType (typeOfName) {}
      NameImpl (const UnicodeString& identifier, TypePtr typeOfName,
		ExpressionPtr value, bool constant)
	: identifier (identifier), type (Variable), valueType (typeOfName),
	  varValue (value), varConstant (constant) {}
      
      NameType GetType() { return type; }
      TypePtr GetAliasedType()
      { return type == TypeAlias ? valueType : TypePtr (); }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NAMEIMPL_H__
