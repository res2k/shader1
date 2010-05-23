#include "base/common.h"

#include "VariableExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "NameImpl.h"
#include "ScopeImpl.h"

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
      
    RegisterID
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::GetRegister (BlockImpl& block, bool writeable)
    {
      if (!myReg.IsValid())
      {
	boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (block.GetInnerScope()));
	if (boost::shared_ptr<ScopeImpl> (name->ownerScope) != blockScopeImpl)
	{
	  myReg = block.ImportName (name, writeable);
	}
	else
	{
	  myReg = name->GetRegister (handler, block, writeable);
	}
      }
      return myReg;
    }
      
    IntermediateGeneratorSemanticsHandler::NameImplSet
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::QueryWrittenNames (bool asLvalue)
    {
      NameImplSet set;
      if (asLvalue) set.insert (name);
      return set;
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::GetValueType()
    {
      return name->valueType;
    }
    
    void IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::AddToSequence (BlockImpl& block,
										       RegisterID destination)
    {
      if (destination.IsValid())
      {
	// Evaluating to a destination -> assignment
	SequenceOpPtr seqOp;
	seqOp = SequenceOpPtr (boost::make_shared<SequenceOpAssign> (destination, GetRegister (block, false)));
	block.GetSequence()->AddOp (seqOp);
      }
      else
      {
	/* Don't need anything here... other ops will either write to, or
	  read from, the register for this var. */
      }
    }
  } // namespace intermediate
} // namespace s1
