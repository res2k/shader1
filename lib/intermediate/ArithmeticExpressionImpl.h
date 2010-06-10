#ifndef __INTERMEDIATE_ARITHMETICEXPRESSIONIMPL_H__
#define __INTERMEDIATE_ARITHMETICEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ArithmeticExpressionImpl : public ExpressionImpl
    {
      ArithmeticOp op;
      boost::shared_ptr<ExpressionImpl> operand1;
      boost::shared_ptr<ExpressionImpl> operand2;
    public:
      ArithmeticExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				ArithmeticOp op,
				const boost::shared_ptr<ExpressionImpl>& operand1,
				const boost::shared_ptr<ExpressionImpl>& operand2);
      
      RegisterID GetRegister (BlockImpl& block, bool writeable);
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				const UnicodeString& name = UnicodeString (),
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ARITHMETICEXPRESSIONIMPL_H__
