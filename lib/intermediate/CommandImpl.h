#ifndef __INTERMEDIATE_COMMANDIMPL_H__
#define __INTERMEDIATE_COMMANDIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Sequence.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::CommandImpl : public Expression
    {
    protected:
      IntermediateGeneratorSemanticsHandler* handler;
    public:
      CommandImpl (IntermediateGeneratorSemanticsHandler* handler) : handler (handler) {}
      
      virtual void AddToSequence (BlockImpl& block) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_COMMANDIMPL_H__
