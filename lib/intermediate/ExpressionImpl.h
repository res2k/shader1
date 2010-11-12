#ifndef __INTERMEDIATE_EXPRESSIONIMPL_H__
#define __INTERMEDIATE_EXPRESSIONIMPL_H__

#include "CommandImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::ExpressionImpl : public CommandImpl
    {
    public:
      ExpressionImpl (IntermediateGeneratorSemanticsHandler* handler);
      
      virtual NameImplPtr GetExpressionName() { return NameImplPtr (); }
      
      void AddToSequence (BlockImpl& block);
      
      virtual NameImplSet QueryWrittenNames (bool asLvalue);
      
      virtual boost::shared_ptr<TypeImpl> GetValueType() = 0;
      virtual RegisterPtr AddToSequence (BlockImpl& block,
					 RegisterClassification targetClassify,
					 bool asLvalue = false) = 0;
      virtual void AddToSequencePostAction (BlockImpl& block,
					    const RegisterPtr& target,
					    bool wasLvalue) {}
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXPRESSIONIMPL_H__
