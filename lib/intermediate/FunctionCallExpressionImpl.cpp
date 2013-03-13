#include "base/common.h"

#include "FunctionCallExpressionImpl.h"

#include "BlockImpl.h"
#include "Builtin.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "NameImpl.h"
#include "ScopeImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FunctionCallExpressionImpl (
      IntermediateGeneratorSemanticsHandler* handler,
      const NamePtr& functionName,
      const ExpressionVector& params)
       : ExpressionImpl (handler), functionName (functionName), params (params), overloadSelected (false)
    {
    }

    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::SelectOverload ()
    {
      if (overloadSelected) return;
      overloadSelected = true;
      
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (functionName));
      boost::shared_ptr<ScopeImpl> funcScopeImpl (boost::static_pointer_cast<ScopeImpl> (ScopePtr (nameImpl->ownerScope)));
      
      // Collect overload candidates
      ScopeImpl::FunctionInfoVector candidates (funcScopeImpl->CollectOverloadCandidates (functionName, params));
      
      if (candidates.size() == 0)
      {
	// No candidates
	throw Exception (NoMatchingFunctionOverload);
      }
      else if (candidates.size() > 1)
      {
	// Ambiguous overload
	throw Exception (AmbiguousFunctionOverload);
      }
      overload = candidates[0];
      
      for (size_t i = 0; i < overload->params.size(); i++)
      {
	const ScopeImpl::FunctionFormalParameter& param (overload->params[i]);
	
	ExpressionPtr paramExpr;
	if (i < params.size())
	  paramExpr = params[i];
	else
	  paramExpr = param.defaultValue;
	
	actualParams.push_back (paramExpr);
      }
    }

    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FetchRegisters (BlockImpl& block,
											    FetchedRegs& fetchedRegs,
											    PostActions& postActions)
    {
      for (size_t i = 0; i < actualParams.size(); i++)
      {
	RegisterPtr reg1, reg2;
	boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::static_pointer_cast<ExpressionImpl> (actualParams[i]));
	if (overload->params[i].dir & ScopeImpl::dirIn)
	{
	  reg1 = paramExprImpl->AddToSequence (block, Intermediate, false);
	  postActions.push_back (PostActionInfo (paramExprImpl, reg1, false));
	}
	if (overload->params[i].dir & ScopeImpl::dirOut)
	{
	  reg2 = paramExprImpl->AddToSequence (block, Intermediate, true);
	  postActions.push_back (PostActionInfo (paramExprImpl, reg2, false));
	}
	fetchedRegs.push_back (std::make_pair (reg1, reg2));
      }
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::GetValueType ()
    {
      SelectOverload ();
	
      return boost::static_pointer_cast<TypeImpl> (overload->returnType);
    }
    
    RegisterPtr IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::AddToSequence (BlockImpl& block,
												  RegisterClassification classify,
												  bool asLvalue)
    {
      if (asLvalue) return RegisterPtr();
      
      SelectOverload ();
      
      if (overload->identifier.isEmpty()) return RegisterPtr();
      
      FetchedRegs fetchedRegs;
      PostActions postActions;
      FetchRegisters (block, fetchedRegs, postActions);
      
      std::vector<RegisterPtr> inParams;
      std::vector<RegisterPtr> outParams;
      for (size_t i = 0; i < overload->params.size(); i++)
      {
	const ScopeImpl::FunctionFormalParameter& param (overload->params[i]);
	if (param.dir & ScopeImpl::dirIn)
	{
	  boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::static_pointer_cast<ExpressionImpl> (actualParams[i]));
	  boost::shared_ptr<TypeImpl> paramExprType (paramExprImpl->GetValueType());
	  RegisterPtr inReg (fetchedRegs[i].first);
	  assert (inReg);
	  boost::shared_ptr<TypeImpl> formalParamType (boost::static_pointer_cast<TypeImpl> (param.type));
	  if (!paramExprType->IsEqual (*formalParamType))
	  {
	    RegisterPtr targetReg (handler->AllocateRegister (*(block.GetSequence()), formalParamType, Intermediate));
	    handler->GenerateCast (*(block.GetSequence()), targetReg, formalParamType, inReg, paramExprType);
	    inReg = targetReg;
	  }
	  inParams.push_back (inReg);
	}
	if (param.dir & ScopeImpl::dirOut)
	{
	  boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::static_pointer_cast<ExpressionImpl> (actualParams[i]));
	  RegisterPtr outReg (fetchedRegs[i].second);
	  if (!outReg)
	    throw Exception (ActualParameterNotAnLValue);
	  outParams.push_back (outReg);
	}
      }
      
      if (overload->block)
      {
	// Look for globals imported into function, add as parameters
	boost::shared_ptr<BlockImpl> funcBlockImpl (boost::static_pointer_cast<BlockImpl> (overload->block));
	Sequence::RegisterImpMappings imports (funcBlockImpl->GetSequence()->GetImports());
	for (Sequence::RegisterImpMappings::const_iterator imported (imports.begin());
	    imported != imports.end();
	    ++imported)
	{
	  NameImplPtr global (handler->globalScope->ResolveIdentifierInternal (imported->first));
	  if (global)
	  {
	    RegisterPtr globLocal (block.GetRegisterForName (global, false));
	    inParams.push_back (globLocal);
	  }
	}
	
	intermediate::Sequence::RegisterExpMappings exports (funcBlockImpl->GetSequence()->GetExports());
	for (intermediate::Sequence::RegisterExpMappings::const_iterator exported (exports.begin());
	      exported != exports.end();
	      ++exported)
	{
	  NameImplPtr global (handler->globalScope->ResolveIdentifierInternal (exported->first));
	  if (global)
	  {
	    RegisterPtr globLocal (block.GetRegisterForName (global, true));
	    outParams.push_back (globLocal);
	  }
	}
      }
      
      RegisterPtr destination;
      boost::shared_ptr<TypeImpl> retType (GetValueType());
      if (!retType->IsEqual (*(handler->GetVoidType())))
	destination = handler->AllocateRegister (*(block.GetSequence()), retType, classify);
      
      SequenceOpPtr seqOp;
      if (overload->builtin)
	seqOp = boost::make_shared<SequenceOpBuiltinCall> (destination, overload->builtin->GetBuiltinFunction(),
							   inParams);
      else
      {
	if (destination)
	  outParams.insert (outParams.begin(), destination);
	seqOp = boost::make_shared<SequenceOpFunctionCall> (overload->identifier, inParams, outParams);
      }
      block.GetSequence()->AddOp (seqOp);
      
      for (PostActions::const_iterator postAction (postActions.begin());
	   postAction != postActions.end();
	   ++postAction)
      {
	postAction->expr->AddToSequencePostAction (block, postAction->reg, postAction->lValue);
      }
      
      return destination;
    }
  } // namespace intermediate
} // namespace s1
