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
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ATTRIBUTEEXPRESSIONIMPL_H__
