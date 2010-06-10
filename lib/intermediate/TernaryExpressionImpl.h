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
      
      BlockPtr ifBlock;
      BlockPtr elseBlock;
    public:
      TernaryExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			     const boost::shared_ptr<ExpressionImpl>& condition,
			     const boost::shared_ptr<ExpressionImpl>& ifExpr,
			     const boost::shared_ptr<ExpressionImpl>& elseExpr);
      
      RegisterID GetRegister (BlockImpl& block, bool writeable);
      void InvalidateRegister ();
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				const UnicodeString& name = UnicodeString (),
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TERNARYEXPRESSIONIMPL_H__
