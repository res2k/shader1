#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ExpressionImpl::ExpressionImpl (IntermediateGeneratorSemanticsHandler* handler)
     : CommandImpl (handler)
    {
    }
     
    void IntermediateGeneratorSemanticsHandler::ExpressionImpl::AddToSequence (BlockImpl& block, Sequence& seq)
    {
      boost::shared_ptr<TypeImpl> exprValueType = GetValueType ();
      // Create a dummy destination
      RegisterID dummyDest = handler->AllocateRegister (seq, exprValueType, Dummy);
      // Add expression, write to dummy destination
      AddToSequence (block, seq, dummyDest);
      /* Need to generate operations even if the result isn't used, due
         to possible side effects.
         Eventually, dead code elimination should take care of removing the
         dummy and computations (if possible).
       */
    }
  } // namespace intermediate
} // namespace s1
