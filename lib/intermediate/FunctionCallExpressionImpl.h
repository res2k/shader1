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
      
      void SelectOverload ();
      typedef std::vector<std::pair<RegisterPtr, RegisterPtr> > FetchedRegs;
      struct PostActionInfo
      {
	boost::shared_ptr<ExpressionImpl> expr;
	RegisterPtr reg;
	bool lValue;
	
	PostActionInfo (const boost::shared_ptr<ExpressionImpl>& expr, RegisterPtr reg, bool lValue)
	 : expr (expr), reg (reg), lValue (lValue) {}
      };
      typedef std::vector<PostActionInfo> PostActions;
      void FetchRegisters (s1::intermediate::IntermediateGeneratorSemanticsHandler::BlockImpl & block,
			   FetchedRegs& fetchedRegs, PostActions& postActions);
    public:
      FunctionCallExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
				  const NamePtr& functionName,
				  const ExpressionVector& params);
      
      boost::shared_ptr<TypeImpl> GetValueType ();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
				 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FUNCTIONCALLEXPRESSIONIMPL_H__
