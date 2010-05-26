#include "base/common.h"

#include "FunctionCallExpressionImpl.h"

#include "BlockImpl.h"
#include "intermediate/Exception.h"
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
       : ExpressionImpl (handler), functionName (functionName), params (params), overloadSelected (false),
         haveRegisters (false)
    {
    }

    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::SelectOverload ()
    {
      if (overloadSelected) return;
      overloadSelected = true;
      
      boost::shared_ptr<NameImpl> nameImpl (boost::shared_static_cast<NameImpl> (functionName));
      boost::shared_ptr<ScopeImpl> funcScopeImpl (boost::shared_static_cast<ScopeImpl> (ScopePtr (nameImpl->ownerScope)));
      
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

    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::FetchRegisters (BlockImpl& block)
    {
      if (haveRegisters) return;
      haveRegisters = true;
      
      for (size_t i = 0; i < actualParams.size(); i++)
      {
	RegisterID reg1, reg2;
	boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::shared_static_cast<ExpressionImpl> (actualParams[i]));
	if (overload->params[i].dir & ScopeImpl::dirIn)
	  reg1 = paramExprImpl->GetRegister (block, false);
	if (overload->params[i].dir & ScopeImpl::dirInOut)
	  paramExprImpl->InvalidateRegister();
	if (overload->params[i].dir & ScopeImpl::dirOut)
	  reg2 = paramExprImpl->GetRegister (block, true);
	fetchedRegs.push_back (std::make_pair (reg1, reg2));
      }
    }
      
    RegisterID IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::GetRegister (BlockImpl& block,
											       bool writeable)
    {
      SelectOverload ();
	
      if (!overload->identifier.isEmpty())
      {
	// Let params grab registers ...
	FetchRegisters (block);
      }
      return RegisterID ();
    }
      
    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::InvalidateRegister ()
    {
      SelectOverload ();
	
      // Let params grab registers ...
      for (size_t i = 0; i < actualParams.size(); i++)
      {
	boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::shared_static_cast<ExpressionImpl> (actualParams[i]));
	paramExprImpl->InvalidateRegister ();
      }
      haveRegisters = false;
    }
      
    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::GetValueType ()
    {
      SelectOverload ();
	
      return boost::shared_static_cast<TypeImpl> (overload->returnType);
    }
    
    void IntermediateGeneratorSemanticsHandler::FunctionCallExpressionImpl::AddToSequence (BlockImpl& block,
											   RegisterID destination)
    {
      SelectOverload ();
      
      if (overload->identifier.isEmpty()) return;
      
      FetchRegisters (block);
      
      std::vector<RegisterID> inParams;
      std::vector<RegisterID> outParams;
      for (size_t i = 0; i < overload->params.size(); i++)
      {
	const ScopeImpl::FunctionFormalParameter& param (overload->params[i]);
	if (param.dir & ScopeImpl::dirIn)
	{
	  boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::shared_static_cast<ExpressionImpl> (actualParams[i]));
	  boost::shared_ptr<TypeImpl> paramExprType (paramExprImpl->GetValueType());
	  RegisterID inReg (fetchedRegs[i].first);
	  if (!inReg.IsValid())
	  {
	    inReg = handler->AllocateRegister (*(block.GetSequence()), paramExprType, Intermediate);
	    paramExprImpl->AddToSequence (block, inReg);
	  }
	  boost::shared_ptr<TypeImpl> formalParamType (boost::shared_static_cast<TypeImpl> (param.type));
	  if (!paramExprType->IsEqual (*formalParamType))
	  {
	    RegisterID targetReg (handler->AllocateRegister (*(block.GetSequence()), formalParamType, Intermediate));
	    handler->GenerateCast (*(block.GetSequence()), targetReg, formalParamType, inReg, paramExprType);
	    inReg = targetReg;
	  }
	  inParams.push_back (inReg);
	}
	if (param.dir & ScopeImpl::dirOut)
	{
	  boost::shared_ptr<ExpressionImpl> paramExprImpl (boost::shared_static_cast<ExpressionImpl> (actualParams[i]));
	  RegisterID outReg (fetchedRegs[i].second);
	  if (!outReg.IsValid())
	    throw Exception (ActualParameterNotAnLValue);
	  outParams.push_back (outReg);
	}
      }
      
      // Look for globals imported into function, add as parameters
      boost::shared_ptr<BlockImpl> funcBlockImpl (boost::shared_static_cast<BlockImpl> (overload->block));
      Sequence::RegisterImpMappings imports (funcBlockImpl->GetSequence()->GetImports());
      for (Sequence::RegisterImpMappings::const_iterator imported (imports.begin());
	   imported != imports.end();
	   ++imported)
      {
	NameImplPtr global (handler->globalScope->ResolveIdentifierInternal (imported->first));
	if (global)
	{
	  RegisterID globLocal (block.ImportName (global, false));
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
	  RegisterID globLocal (block.ImportName (global, true));
	  outParams.push_back (globLocal);
	}
      }
      
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpFunctionCall> (destination, overload->identifier,
								       inParams, outParams));
      block.GetSequence()->AddOp (seqOp);
    }
  } // namespace intermediate
} // namespace s1
