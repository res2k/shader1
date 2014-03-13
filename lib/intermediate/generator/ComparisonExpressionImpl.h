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
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_COMPARISONEXPRESSIONIMPL_H__
