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
      UnicodeString identifier;
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
      // Register for variable/constant
      RegisterID varReg;
      
      NameImpl (const boost::weak_ptr<ScopeImpl>& ownerScope,
		const UnicodeString& identifier, NameType type,
		const boost::shared_ptr<TypeImpl>& typeOfName)
	: ownerScope (ownerScope), identifier (identifier), type (type), valueType (typeOfName) {}
      NameImpl (const boost::weak_ptr<ScopeImpl>& ownerScope,
		const UnicodeString& identifier,
		const boost::shared_ptr<TypeImpl>& typeOfName,
		ExpressionPtr value, bool constant)
	: ownerScope (ownerScope), identifier (identifier), type (Variable), valueType (typeOfName),
	  varValue (value), varConstant (constant) {}
      
      NameType GetType() { return type; }
      TypePtr GetAliasedType()
      { return type == TypeAlias ? boost::shared_static_cast<Type> (valueType) : TypePtr (); }
      
      RegisterID GetRegister (IntermediateGeneratorSemanticsHandler* handler,
			      BlockImpl& block, bool writeable);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NAMEIMPL_H__
