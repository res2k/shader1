#ifndef __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__
#define __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::TernaryExpressionImpl : public ExpressionImpl
    {
      boost::shared_ptr<ExpressionImpl> condition;
      boost::shared_ptr<ExpressionImpl> ifExpr;
      boost::shared_ptr<ExpressionImpl> elseExpr;
    public:
      TernaryExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			     const boost::shared_ptr<ExpressionImpl>& condition,
			     const boost::shared_ptr<ExpressionImpl>& ifExpr,
			     const boost::shared_ptr<ExpressionImpl>& elseExpr);
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__
