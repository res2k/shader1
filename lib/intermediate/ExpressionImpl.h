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
      
      void AddToSequence (BlockImpl& block);
      
      virtual NameImplSet QueryWrittenNames (bool asLvalue);
      
      virtual boost::shared_ptr<TypeImpl> GetValueType() = 0;
      virtual RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
					const UnicodeString& name = UnicodeString (),
					bool asLvalue = false) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXPRESSIONIMPL_H__
