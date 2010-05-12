#include "NameImpl.h"
#include "ExpressionImpl.h"
#include "BlockImpl.h"

namespace s1
{
  namespace intermediate
  {
    RegisterID IntermediateGeneratorSemanticsHandler::NameImpl::GetRegister (IntermediateGeneratorSemanticsHandler* handler,
									     BlockImpl& block, bool writeable)
    {
      /* Note: asking for a register for a constant value is only an error for the second time and
	  after; the first request is satisfied as the constant may have to be loaded somewhere */
      if (!varReg.IsValid())
      {
	varReg = handler->AllocateRegister (*(block.GetSequence()), valueType,
					    IntermediateGeneratorSemanticsHandler::Variable, identifier);
      }
      else if (writeable)
      {
	if (varConstant)
	  // Throw?
	  return RegisterID ();
	// Query a new generation
	varReg = handler->AllocateRegister (*(block.GetSequence()), varReg);
      }
      return varReg;
    }
  } // namespace intermediate
} // namespace s1
