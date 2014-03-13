#ifndef __INTERMEDIATE_FUNCTIONIMPL_H__
#define __INTERMEDIATE_FUNCTIONIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::FunctionImpl : public Function
    {
      IntermediateGeneratorSemanticsHandler* handler;
      ScopeImpl::FunctionInfoPtr funcInfo;
      BlockPtr block;
    public:
      FunctionImpl (IntermediateGeneratorSemanticsHandler* handler,
                    ScopeImpl::FunctionInfoPtr funcInfo,
                    const BlockPtr& block)
        : handler (handler), funcInfo (funcInfo), block (block) {}
      BlockPtr GetBody() { return block; }
      void Finish();
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FUNCTIONIMPL_H__
