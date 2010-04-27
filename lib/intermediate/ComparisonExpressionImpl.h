#ifndef __INTERMEDIATE_COMPARISONEXPRESSIONIMPL_H__
#define __INTERMEDIATE_COMPARISONEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ComparisonExpressionImpl : public ExpressionImpl
    {
      CompareOp op;
      boost::shared_ptr<ExpressionImpl> operand1;
      boost::shared_ptr<ExpressionImpl> operand2;
    public:
      ComparisonExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				CompareOp op,
				const boost::shared_ptr<ExpressionImpl>& operand1,
				const boost::shared_ptr<ExpressionImpl>& operand2);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      void AddToSequence (BlockImpl& block, Sequence& seq, Sequence::RegisterID destination);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_COMPARISONEXPRESSIONIMPL_H__
