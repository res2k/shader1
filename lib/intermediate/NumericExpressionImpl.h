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
      void AddToSequence (BlockImpl& block, Sequence& seq, Sequence::RegisterID destination);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_NUMERICEXPRESSIONIMPL_H__
