#ifndef __INTERMEDIATE_VARIABLEEXPRESSIONIMPL_H__
#define __INTERMEDIATE_VARIABLEEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::VariableExpressionImpl : public ExpressionImpl
    {
      boost::shared_ptr<NameImpl> name;
      
      RegisterID myReg;
    public:
      VariableExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
			      const boost::shared_ptr<NameImpl>& name);
      
      RegisterID GetRegister (BlockImpl& block, bool writeable);
      void InvalidateRegister () { myReg = RegisterID (); }
      
      boost::shared_ptr<TypeImpl> GetValueType();
      void AddToSequence (BlockImpl& block, RegisterID destination);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_VARIABLEEXPRESSIONIMPL_H__
