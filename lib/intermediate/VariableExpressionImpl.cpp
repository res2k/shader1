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
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (block.GetInnerScope()));
      if (boost::shared_ptr<ScopeImpl> (name->ownerScope) != blockScopeImpl)
      {
	return block.ImportName (name, writeable);
      }
      else
      {
	return name->GetRegister (handler, block, writeable);
      }
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
    
    RegisterID IntermediateGeneratorSemanticsHandler::VariableExpressionImpl::AddToSequence (BlockImpl& block,
											     RegisterClassification classify,
											     const UnicodeString& name,
											     bool asLvalue)
    {
      return GetRegister (block, asLvalue);
    }
  } // namespace intermediate
} // namespace s1
