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
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (block.GetInnerScope()));
      if (boost::shared_ptr<ScopeImpl> (this->name->ownerScope) != blockScopeImpl)
      {
	return block.ImportName (this->name, asLvalue);
      }
      else
      {
	return this->name->GetRegister (handler, block, asLvalue);
      }
    }
  } // namespace intermediate
} // namespace s1
