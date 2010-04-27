#include "VariableExpressionImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOpAssign.h"
#include "NameImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::VariableExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const boost::shared_ptr<NameImpl>& name)
       : ExpressionImpl (handler), name (name)
    {
    }
      
    Sequence::RegisterID
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::GetRegister (Sequence& seq, bool writeable)
    {
      // TODO: Need to handle vars from outer scopes differently
      return name->GetRegister (handler, seq, writeable);
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::GetValueType()
    {
      return name->valueType;
    }
    
    void IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::AddToSequence (BlockImpl& block,
										       Sequence& seq,
										       Sequence::RegisterID destination)
    {
      if (destination.IsValid())
      {
	// Evaluating to a destination -> assignment
	SequenceOpPtr seqOp;
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpAssign> (destination, GetRegister (seq, false)));
	seq.AddOp (seqOp);
      }
      else
      {
	/* Don't need anything here... other ops will either write to, or
	  read from, the register for this var. */
      }
    }
  } // namespace intermediate
} // namespace s1
