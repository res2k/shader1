#ifndef __INTERMEDIATE_BOOLEXPRESSIONIMPL_H__
#define __INTERMEDIATE_BOOLEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::BoolExpressionImpl : public ExpressionImpl
    {
      bool value;
    public:
      BoolExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			  bool value);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BOOLEXPRESSIONIMPL_H__
