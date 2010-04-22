#ifndef __INTERMEDIATE_EXPRESSIONIMPL_H__
#define __INTERMEDIATE_EXPRESSIONIMPL_H__

#include "CommandImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ExpressionImpl : public CommandImpl
    {
    public:
      ExpressionImpl (IntermediateGeneratorSemanticsHandler* handler);
      
      void AddToSequence (BlockImpl& block, Sequence& seq);
      
      /**
       * Get register with value of this expression.
       * Can be an 'invalid' register if the expression doesn't represent an
       * atomic value.
       */
      virtual Sequence::RegisterID GetRegister (Sequence& seq, bool writeable)
      { return Sequence::RegisterID (); }
      
      virtual boost::shared_ptr<TypeImpl> GetValueType() = 0;
      virtual void AddToSequence (BlockImpl& block, Sequence& seq, Sequence::RegisterID destination) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXPRESSIONIMPL_H__
