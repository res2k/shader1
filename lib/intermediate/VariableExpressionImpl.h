#ifndef __INTERMEDIATE_VARIABLEEXPRESSIONIMPL_H__
#define __INTERMEDIATE_VARIABLEEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::VariableExpressionImpl : public ExpressionImpl
    {
      NameImplPtr name;
    public:
      VariableExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			      const NameImplPtr& name);
			      
      NameImplPtr GetExpressionName() { return name; }
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				const UnicodeString& name = UnicodeString (),
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_VARIABLEEXPRESSIONIMPL_H__
