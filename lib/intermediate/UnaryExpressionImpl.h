#ifndef __INTERMEDIATE_UNARYEXPRESSIONIMPL_H__
#define __INTERMEDIATE_UNARYEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::UnaryExpressionImpl : public ExpressionImpl
    {
      UnaryOp op;
      boost::shared_ptr<ExpressionImpl> operand;
    public:
      UnaryExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			   UnaryOp op,
			   const boost::shared_ptr<ExpressionImpl>& operand);
      
      RegisterID GetRegister (BlockImpl& block, bool writeable);
      void InvalidateRegister ();
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      void AddToSequence (BlockImpl& block, RegisterID destination);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_UNARYEXPRESSIONIMPL_H__
