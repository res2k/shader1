#include "VariableExpressionImpl.h"

#include "intermediate/Exception.h"
#include "NameImpl.h"

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
      /* Don't need anything here... other ops will either write to, or
         read from, the register for this var. */
    }
  } // namespace intermediate
} // namespace s1
