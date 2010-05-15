#include "BlockImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "AssignmentExpressionImpl.h"
#include "ExpressionImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "VariableExpressionImpl.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    IntermediateGeneratorSemanticsHandler::BlockImpl::BlockImpl (IntermediateGeneratorSemanticsHandler* handler,
								 ScopePtr innerScope)
     : handler (handler), innerScope (innerScope),
       sequence (boost::make_shared<Sequence> ())
    {}
     
    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddExpressionCommand (ExpressionPtr expr)
    {
      FlushVariableInitializers();
      CommandImpl* impl = static_cast<CommandImpl*> (expr.get());
      impl->AddToSequence (*this);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddNestedBlock (BlockPtr block)
    {
      FlushVariableInitializers();
      boost::shared_ptr<BlockImpl> blockImpl (boost::shared_static_cast<BlockImpl> (block));
      blockImpl->FinishBlock();
      
      /* Pass 'snapshot' of identifiers-to-register-ID map
         When resolving imports, the registers for variables _at the time of the block
         insertion_ is needed, hence the snapshot.
         Also, do it before allocating new registers for the 'written registers',
         as we want the ID before that */
      Sequence::IdentifierToRegIDMap identifierToRegIDMap (sequence->GetIdentifierToRegisterIDMap ());
      // Generate register IDs for all values the nested block exports
      std::vector<RegisterID> writtenRegisters;
      {
	for (ExportedNamesSet::const_iterator exportedName = blockImpl->exportedNames.begin();
	     exportedName != blockImpl->exportedNames.end();
	     exportedName++)
	{
	  boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
	  if (boost::shared_ptr<ScopeImpl> ((*exportedName)->ownerScope) == blockScopeImpl)
	  {
	    writtenRegisters.push_back ((*exportedName)->GetRegister (handler, *this, true));
	  }
	}
      }
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpBlock> (blockImpl->GetSequence(),
								identifierToRegIDMap,
								writtenRegisters));
      sequence->AddOp (seqOp);
    }
    
    void IntermediateGeneratorSemanticsHandler::BlockImpl::FlushVariableInitializers()
    {
      // Get new variables ...
      std::vector<NamePtr> newVars (boost::shared_static_cast<ScopeImpl>(innerScope)->FlushNewVars());
      for (std::vector<NamePtr>::const_iterator varIt = newVars.begin();
	   varIt != newVars.end();
	   ++varIt)
      {
	boost::shared_ptr<NameImpl> name (boost::shared_static_cast<NameImpl> (*varIt));
	// ... check if it has initialization value ...
	if (name->varValue)
	{
	  // ... if so, synthesize assignment
	  boost::shared_ptr<ExpressionImpl> exprTarget (boost::make_shared<VariableExpressionImpl> (handler, name));
	  ExpressionPtr expr (boost::make_shared<AssignmentExpressionImpl> (handler, exprTarget,
									    boost::static_pointer_cast<ExpressionImpl> (name->varValue)));
	  // Note recursion is okay as FlushNewVars() will return an empty array
	  AddExpressionCommand (expr);
	}
      }
    }

    RegisterID IntermediateGeneratorSemanticsHandler::BlockImpl::ImportName (NamePtr name, bool writeable)
    {
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (name));
      if (writeable && nameImpl->varConstant)
      {
	throw Exception (AssignmentTargetIsNotAnLValue);
      }
      
      RegisterID reg;
      ImportedName& impName = importedNames[name];
      if (!impName.currentRegister.IsValid())
      {
	reg = impName.currentRegister = impName.initialRegister =
	  handler->AllocateRegister (*sequence, nameImpl->valueType, Imported,
				     nameImpl->identifier);
	sequence->AddImport (nameImpl->identifier, reg);
      }
      else
      {
	if (writeable)
	  impName.currentRegister = handler->AllocateRegister (*sequence, impName.currentRegister);
	reg = impName.currentRegister;
      }
      if (writeable)
      {
	sequence->SetExport (nameImpl->identifier, reg);
	exportedNames.insert (nameImpl);
      }
      return reg;
    }
  } // namespace intermediate
} // namespace s1
