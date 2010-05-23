#include "base/common.h"

#include "BlockImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"
#include "AssignmentExpressionImpl.h"
#include "ExpressionImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "VariableExpressionImpl.h"

#include <boost/make_shared.hpp>
#include <unicode/ustdio.h>

namespace s1
{
  namespace intermediate
  {
    static const char varConditionName[] = "$cond";
    
    IntermediateGeneratorSemanticsHandler::BlockImpl::BlockImpl (IntermediateGeneratorSemanticsHandler* handler,
								 ScopePtr innerScope)
     : handler (handler), innerScope (innerScope),
       sequence (boost::make_shared<Sequence> ())
    {
      char newCondName[sizeof (varConditionName) + charsToFormatInt + 1];
      snprintf (newCondName, sizeof (newCondName), "%s%d", varConditionName, 
		boost::shared_static_cast<ScopeImpl> (innerScope)->DistanceToScope (handler->globalScope));
      varCondition = boost::static_pointer_cast<NameImpl> (innerScope->AddVariable (handler->GetBoolType(),
										    UnicodeString (newCondName),
										    ExpressionPtr(), false));
    }
     
    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddExpressionCommand (ExpressionPtr expr)
    {
      FlushVariableInitializers();
      CommandImpl* impl = static_cast<CommandImpl*> (expr.get());
      impl->AddToSequence (*this);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddBranching (ExpressionPtr branchCondition,
									 BlockPtr ifBlock,
									 BlockPtr elseBlock)
    {
      FlushVariableInitializers();
      ExpressionImpl* impl = static_cast<ExpressionImpl*> (branchCondition.get());
      RegisterID condReg (handler->AllocateRegister (*sequence, handler->GetBoolType(),
						     Condition));
      impl->AddToSequence (*this, condReg);
      
      SequenceOpPtr seqOpIf (CreateBlockSeqOp (ifBlock));
      SequenceOpPtr seqOpElse (CreateBlockSeqOp (elseBlock));
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpBranch> (condReg, seqOpIf, seqOpElse));
      sequence->AddOp (seqOp);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock)
    {
      FlushVariableInitializers();
      
      /*
	The while loop looks like:
	  while (<cond>)
	  {
	    <body>
	  }
	We rewrite that to:
	  bool c = <cond>;	Op #1
	  while (c)		Op #2
	    <block>
	with <block>:
	  {
	    <body>
	    c = <cond>;
	  }
       */
      
      ExpressionImpl* condImpl = static_cast<ExpressionImpl*> (loopCond.get());
      RegisterID condReg (varCondition->GetRegister (handler, *this, true));
      condImpl->AddToSequence (*this, condReg);
      
      boost::shared_ptr<BlockImpl> blockImpl (boost::shared_static_cast<BlockImpl> (loopBlock));
      blockImpl->FinishBlock();
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      NameImplSet loopVars;
      // Condition var is implicitly read (condition check) and written (end of body)
      loopVars.insert (varCondition);
      // Look for variables that are both read+written in the body
      {
	for (ImportedNamesMap::const_iterator import = blockImpl->importedNames.begin();
	     import != blockImpl->importedNames.end();
	     ++import)
	{
	  if (blockImpl->exportedNames.find (import->first) != blockImpl->exportedNames.end())
	  {
	    loopVars.insert (import->first);
	  }
	}
      }
      
      // For those variable, allocate new 'writeable' registers
      std::vector<std::pair<RegisterID, RegisterID> > loopedRegs;
      for (NameImplSet::const_iterator loopVar = loopVars.begin();
	   loopVar != loopVars.end();
	   ++loopVar)
      {
	if (boost::shared_ptr<ScopeImpl> ((*loopVar)->ownerScope) == blockScopeImpl)
	{
	  RegisterID regIn ((*loopVar)->GetRegister (handler, *this, false));
	  RegisterID regOut ((*loopVar)->GetRegister (handler, *this, true));
	  loopedRegs.push_back (std::make_pair (regIn, regOut));
	  // Small kludge: change the condition register to the one that's written to
	  if (condReg == regIn) condReg = regOut;
	}
	else
	{
	  RegisterID regIn (ImportName (*loopVar, false));
	  RegisterID regOut (ImportName (*loopVar, true));
	  loopedRegs.push_back (std::make_pair (regIn, regOut));
	}
      }
      
      /* Add condition expression again at the bottom of the block
         as the "condition" in the sequence op is just a simple reg */
      boost::shared_ptr<BlockImpl> newBlock (boost::make_shared<BlockImpl> (*(boost::shared_static_cast<BlockImpl> (loopBlock))));
      condImpl->InvalidateRegister();
      condImpl->AddToSequence (*newBlock, newBlock->ImportName (varCondition, true));
      
      SequenceOpPtr seqOpBody (CreateBlockSeqOp (newBlock, loopVars));
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpWhile> (condReg, loopedRegs, seqOpBody));
      sequence->AddOp (seqOp);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond,
								       ExpressionPtr tailExpr, BlockPtr loopBlock)
    {
      FlushVariableInitializers();
      
      /*
	The for loop looks like:
	  for (<init>; <cond>; <tail>)
	  {
	    <body>
	  }
	We rewrite that to:
	  <init>;		Op #1
	  bool c = <cond>;	Op #2
	  while (c)		Op #3
	    <block>
	with <block>:
	  {
	    <body>
	    <tail>;
	    c = <cond>;
	  }
       */
      
      ExpressionImpl* initImpl = static_cast<ExpressionImpl*> (initExpr.get());
      initImpl->AddToSequence (*this);
      
      ExpressionImpl* condImpl = static_cast<ExpressionImpl*> (loopCond.get());
      RegisterID condReg (varCondition->GetRegister (handler, *this, true));
      condImpl->AddToSequence (*this, condReg);
      
      ExpressionImpl* tailImpl = static_cast<ExpressionImpl*> (tailExpr.get());
      
      boost::shared_ptr<BlockImpl> blockImpl (boost::shared_static_cast<BlockImpl> (loopBlock));
      blockImpl->FinishBlock();
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      NameImplSet loopVars;
      // Condition var is implicitly read (condition check) and written (end of body)
      loopVars.insert (varCondition);
      // Include all variables changed in the tail expression
      {
	NameImplSet tailVars (tailImpl->QueryWrittenNames (false));
	loopVars.insert (tailVars.begin(), tailVars.end());
      }
      // Look for variables that are both read+written in the body
      {
	for (ImportedNamesMap::const_iterator import = blockImpl->importedNames.begin();
	     import != blockImpl->importedNames.end();
	     ++import)
	{
	  if (blockImpl->exportedNames.find (import->first) != blockImpl->exportedNames.end())
	  {
	    loopVars.insert (import->first);
	  }
	}
      }
      
      // For those variable, allocate new 'writeable' registers
      std::vector<std::pair<RegisterID, RegisterID> > loopedRegs;
      for (NameImplSet::const_iterator loopVar = loopVars.begin();
	   loopVar != loopVars.end();
	   ++loopVar)
      {
	if (boost::shared_ptr<ScopeImpl> ((*loopVar)->ownerScope) == blockScopeImpl)
	{
	  RegisterID regIn ((*loopVar)->GetRegister (handler, *this, false));
	  RegisterID regOut ((*loopVar)->GetRegister (handler, *this, true));
	  loopedRegs.push_back (std::make_pair (regIn, regOut));
	  // Small kludge: change the condition register to the one that's written to
	  if (condReg == regIn) condReg = regOut;
	}
	else
	{
	  RegisterID regIn (ImportName (*loopVar, false));
	  RegisterID regOut (ImportName (*loopVar, true));
	  loopedRegs.push_back (std::make_pair (regIn, regOut));
	}
      }
      
      boost::shared_ptr<BlockImpl> newBlock (boost::make_shared<BlockImpl> (*(boost::shared_static_cast<BlockImpl> (loopBlock))));
      // Add "tail" expression to end of block
      tailImpl->AddToSequence (*newBlock);
      
      /* Add condition expression again at the bottom of the block
         as the "condition" in the sequence op is just a simple reg */
      condImpl->InvalidateRegister();
      condImpl->AddToSequence (*newBlock, newBlock->ImportName (varCondition, true));
      
      SequenceOpPtr seqOpBody (CreateBlockSeqOp (newBlock, loopVars));
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpWhile> (condReg, loopedRegs, seqOpBody));
      sequence->AddOp (seqOp);
    }
    
    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddNestedBlock (BlockPtr block)
    {
      FlushVariableInitializers();
      SequenceOpPtr seqOp (CreateBlockSeqOp (block));
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

    SequenceOpPtr IntermediateGeneratorSemanticsHandler::BlockImpl::CreateBlockSeqOp (s1::parser::SemanticsHandler::BlockPtr block,
										      const NameImplSet& loopNames)
    {
      boost::shared_ptr<BlockImpl> blockImpl (boost::shared_static_cast<BlockImpl> (block));
      blockImpl->FinishBlock();
      
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      std::vector<RegisterID> readRegisters;
      {
	for (ImportedNamesMap::const_iterator import = blockImpl->importedNames.begin();
	     import != blockImpl->importedNames.end();
	     ++import)
	{
	  if ((boost::shared_ptr<ScopeImpl> (import->first->ownerScope) != blockScopeImpl)
	      && !import->second.initiallyWriteable)
	  {
	    RegisterID reg (ImportName (import->first, false));
	    sequence->SetIdentifierRegisterID (import->first->identifier, reg);
	    readRegisters.push_back (reg);
	  }
	}
      }
      
      /* Pass 'snapshot' of identifiers-to-register-ID map
         When resolving imports, the registers for variables _at the time of the block
         insertion_ is needed, hence the snapshot.
         Also, do it before allocating new registers for the 'written registers',
         as we want the ID before that */
      Sequence::IdentifierToRegIDMap identifierToRegIDMap (sequence->GetIdentifierToRegisterIDMap ());
      // Generate register IDs for all values the nested block exports
      std::vector<RegisterID> writtenRegisters;
      {
	for (NameImplSet::const_iterator exportedName = blockImpl->exportedNames.begin();
	     exportedName != blockImpl->exportedNames.end();
	     exportedName++)
	{
	  /* "Loop names" are treated somewhat special as the caller will have taken care of
	     allocating writeable regs for these names. */
	  bool isLoopName = loopNames.find (*exportedName) != loopNames.end();
	  if (boost::shared_ptr<ScopeImpl> ((*exportedName)->ownerScope) == blockScopeImpl)
	  {
	    writtenRegisters.push_back ((*exportedName)->GetRegister (handler, *this, !isLoopName));
	  }
	  else
	  {
	    RegisterID reg (ImportName ((*exportedName), !isLoopName));
	    writtenRegisters.push_back (reg);
	    sequence->SetIdentifierRegisterID ((*exportedName)->identifier, reg);
	  }
	}
      }
      return boost::make_shared<SequenceOpBlock> (blockImpl->GetSequence(),
						  identifierToRegIDMap,
						  sequence->GetIdentifierToRegisterIDMap (),
						  readRegisters,
						  writtenRegisters);
    }

    RegisterID IntermediateGeneratorSemanticsHandler::BlockImpl::ImportName (NamePtr name, bool writeable)
    {
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (name));
      if (writeable && nameImpl->varConstant)
      {
	throw Exception (AssignmentTargetIsNotAnLValue);
      }
      
      ImportedName& impName = importedNames[nameImpl];
      RegisterID& reg = impName.reg;
      if (!reg.IsValid())
      {
	UnicodeString importName (nameImpl->identifier);
	/* Add a suffix derived from the "distance" of this block's scope to the scope
	   that defines 'name' in order to make local register name unique */
	int d = boost::shared_static_cast<ScopeImpl> (innerScope)->DistanceToScope (
	  boost::shared_ptr<ScopeImpl> (nameImpl->ownerScope));
	if (d >= 0)
	{
	  UChar distSuffix[charsToFormatInt + 3];
	  u_snprintf (distSuffix, sizeof (distSuffix)/sizeof (UChar),
		      "_B%d", d);
	  importName.append (distSuffix);
	}
	impName.initiallyWriteable = writeable;
	reg = handler->AllocateRegister (*sequence, nameImpl->valueType, Imported,
					 importName);
	if (!writeable) sequence->AddImport (nameImpl->identifier, reg);
      }
      else
      {
	if (writeable)
	  reg = handler->AllocateRegister (*sequence, reg);
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
