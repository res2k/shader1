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
      
      void AddToSequence (BlockImpl& block);
      
      /**
       * Get register with value of this expression.
       * Can be an 'invalid' register if the expression doesn't represent an
       * atomic value.
       * Subsequent calls should always return the same register.
       */
      virtual RegisterID GetRegister (BlockImpl& block, bool writeable)
      { return RegisterID (); }
      /// Force next GetRegister() call to fetch a new register.
      virtual void InvalidateRegister () {}
      
      virtual NameImplSet QueryWrittenNames (bool asLvalue);
      
      virtual boost::shared_ptr<TypeImpl> GetValueType() = 0;
      virtual void AddToSequence (BlockImpl& block, RegisterID destination) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXPRESSIONIMPL_H__
