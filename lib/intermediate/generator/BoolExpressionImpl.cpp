#include "base/common.h"

#include "BoolExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::BoolExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      bool value)
       : ExpressionImpl (handler), value (value)
    {
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::GetValueType()
    {
      return handler->GetBoolType();
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::BoolExpressionImpl::AddToSequence (BlockImpl& block,
											  RegisterClassification classify,
											  bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      RegisterPtr destination (handler->AllocateRegister (*(block.GetSequence()), GetValueType(), classify));
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpConst> (destination, value));
      assert (seqOp);
      block.GetSequence()->AddOp (seqOp);
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
