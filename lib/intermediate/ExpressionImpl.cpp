#include "base/common.h"

#include "BlockImpl.h"
#include "ExpressionImpl.h"
#include "TypeImpl.h"

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::ExpressionImpl::ExpressionImpl (IntermediateGeneratorSemanticsHandler* handler)
     : CommandImpl (handler)
    {
    }

    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::ExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      return NameImplSet ();
    }
     
    void IntermediateGeneratorSemanticsHandler::ExpressionImpl::AddToSequence (BlockImpl& block)
    {
      boost::shared_ptr<TypeImpl> exprValueType = GetValueType ();
      // Create a dummy destination
      RegisterID dummyDest;
      if (!handler->GetVoidType()->IsEqual (*exprValueType))
	dummyDest = handler->AllocateRegister (*(block.GetSequence()), exprValueType, Dummy);
      // Add expression, write to dummy destination
      AddToSequence (block, dummyDest);
      /* Need to generate operations even if the result isn't used, due
         to possible side effects.
         Eventually, dead code elimination should take care of removing the
         dummy and computations (if possible).
       */
    }
  } // namespace intermediate
} // namespace s1
