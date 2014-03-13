#ifndef __INTERMEDIATE_ATTRIBUTEEXPRESSIONIMPL_H__
#define __INTERMEDIATE_ATTRIBUTEEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::AttributeExpressionImpl : public ExpressionImpl
    {
      ExpressionPtr baseExpr;
      IntermediateGeneratorSemanticsHandler::Attribute attr;
    public:
      AttributeExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			       const ExpressionPtr& baseExpr,
			       const IntermediateGeneratorSemanticsHandler::Attribute& attr);
      
      TypeImplPtr GetValueType ();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
      void AddToSequencePostAction (BlockImpl& block,
				    const RegisterPtr& target,
				    bool wasLvalue);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ATTRIBUTEEXPRESSIONIMPL_H__
