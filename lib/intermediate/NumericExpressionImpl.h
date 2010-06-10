#ifndef __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__
#define __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::NumericExpressionImpl : public ExpressionImpl
    {
      UnicodeString valueStr;
      SemanticsHandler::BaseType numberType;
    public:
      NumericExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			     const UnicodeString& valueStr);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				const UnicodeString& name = UnicodeString (),
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__
