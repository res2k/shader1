#include "BlockImpl.h"

#include "ExpressionImpl.h"

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
      CommandImpl* impl = static_cast<CommandImpl*> (expr.get());
      impl->AddToSequence (*this);
    }
  } // namespace intermediate
} // namespace s1
