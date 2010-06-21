#ifndef __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__
#define __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::TypeConstructorExpressionImpl : public ExpressionImpl
    {
      TypeImplPtr type;
      ExpressionVector params;
      
      void ExtractBaseExpressions (ExpressionVector& baseExpr);
    public:
      TypeConstructorExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				     const TypeImplPtr& type,
				     const ExpressionVector& params);
      
      TypeImplPtr GetValueType () { return type; }
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TYPECONSTRUCTOREXPRESSIONIMPL_H__
