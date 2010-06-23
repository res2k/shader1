#ifndef __INTERMEDIATE_ARRAYELEMENTEXPRESSIONIMPL_H__
#define __INTERMEDIATE_ARRAYELEMENTEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ArrayElementExpressionImpl : public ExpressionImpl
    {
      ExpressionPtr arrayExpr;
      ExpressionPtr indexExpr;
    public:
      ArrayElementExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				  const ExpressionPtr& arrayExpr,
				  const ExpressionPtr& indexExpr);
      
      TypeImplPtr GetValueType ();
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				bool asLvalue = false);
      void AddToSequencePostAction (BlockImpl& block, const RegisterID& target,
				    bool wasLvalue);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ARRAYELEMENTEXPRESSIONIMPL_H__s
