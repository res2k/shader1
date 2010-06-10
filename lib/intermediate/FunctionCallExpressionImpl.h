#ifndef __INTERMEDIATE_FUNCTIONCALLEXPRESSIONIMPL_H__
#define __INTERMEDIATE_FUNCTIONCALLEXPRESSIONIMPL_H__

#include "ExpressionImpl.h"
#include "ScopeImpl.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl : public ExpressionImpl
    {
      NamePtr functionName;
      ExpressionVector params;
      
      bool overloadSelected;
      ScopeImpl::FunctionInfoPtr overload;
      
      ExpressionVector actualParams;
      bool haveRegisters;
      std::vector<std::pair<RegisterID, RegisterID> > fetchedRegs;
      
      void SelectOverload ();
      void FetchRegisters (s1::intermediate::IntermediateGeneratorSemanticsHandler::BlockImpl & block);
    public:
      FunctionCallExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				  const NamePtr& functionName,
				  const ExpressionVector& params);
      
      RegisterID GetRegister (BlockImpl& block, bool writeable);
      void InvalidateRegister ();
      
      boost::shared_ptr<TypeImpl> GetValueType ();
      RegisterID AddToSequence (BlockImpl& block, RegisterClassification classify,
				const UnicodeString& name = UnicodeString (),
				bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FUNCTIONCALLEXPRESSIONIMPL_H__