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
      RegisterPtr res (AddToSequence (block, Dummy));
      /* Need to generate operations even if the result isn't used, due
         to possible side effects.
         Eventually, dead code elimination should take care of removing the
         dummy and computations (if possible).
       */
      AddToSequencePostAction (block, res, false);
    }
  } // namespace intermediate
} // namespace s1
