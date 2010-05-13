#include "BlockImpl.h"

#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "AssignmentExpressionImpl.h"
#include "ExpressionImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "VariableExpressionImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::BlockImpl::BlockImpl (IntermediateGeneratorSemanticsHandler* handler,
								 ScopePtr innerScope)
     : handler (handler), innerScope (innerScope),
       sequence (boost::make_shared<Sequence> ())
    {}
     
    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddExpressionCommand (ExpressionPtr expr)
    {
      FlushVariableInitializers();
      CommandImpl* impl = static_cast<CommandImpl*> (expr.get());
      impl->AddToSequence (*this);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddNestedBlock (BlockPtr block)
    {
      FlushVariableInitializers();
      boost::shared_ptr<BlockImpl> blockImpl (boost::shared_static_cast<BlockImpl> (block));
      blockImpl->FinishBlock();
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpBlock> (blockImpl->GetSequence()));
      sequence->AddOp (seqOp);
    }
    
    void IntermediateGeneratorSemanticsHandler::BlockImpl::FlushVariableInitializers()
    {
      // Get new variables ...
      std::vector<NamePtr> newVars (boost::shared_static_cast<ScopeImpl>(innerScope)->FlushNewVars());
      for (std::vector<NamePtr>::const_iterator varIt = newVars.begin();
	   varIt != newVars.end();
	   ++varIt)
      {
	boost::shared_ptr<NameImpl> name (boost::shared_static_cast<NameImpl> (*varIt));
	// ... check if it has initialization value ...
	if (name->varValue)
	{
	  // ... if so, synthesize assignment
	  boost::shared_ptr<ExpressionImpl> exprTarget (boost::make_shared<VariableExpressionImpl> (handler, name));
	  ExpressionPtr expr (boost::make_shared<AssignmentExpressionImpl> (handler, exprTarget,
									    boost::static_pointer_cast<ExpressionImpl> (name->varValue)));
	  // Note recursion is okay as FlushNewVars() will return an empty array
	  AddExpressionCommand (expr);
	}
      }
    }
  } // namespace intermediate
} // namespace s1
