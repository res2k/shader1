#ifndef __INTERMEDIATE_ASSIGNMENTEXPRESSIONIMPL_H__
#define __INTERMEDIATE_ASSIGNMENTEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::AssignmentExpressionImpl : public ExpressionImpl
    {
      boost::shared_ptr<ExpressionImpl> target;
      boost::shared_ptr<ExpressionImpl> value;
    public:
      AssignmentExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				const boost::shared_ptr<ExpressionImpl>& target,
				const boost::shared_ptr<ExpressionImpl>& value);
      
      NameImplSet QueryWrittenNames (bool asLvalue);
      
      boost::shared_ptr<TypeImpl> GetValueType();
      
      void AddToSequence (BlockImpl& block);
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				const UnicodeString& name = UnicodeString (),
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_ASSIGNMENTEXPRESSIONIMPL_H__
