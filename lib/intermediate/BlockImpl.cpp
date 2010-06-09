#include "base/common.h"

#include "BlockImpl.h"

#include "intermediate/Exception.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceOp/SequenceOpReturn.h"
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

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddReturnCommand (ExpressionPtr returnValue)
    {
      FlushVariableInitializers();
      
      RegisterID retValReg;
      if (returnValue)
      {
	ExpressionImpl* impl = static_cast<ExpressionImpl*> (returnValue.get());
	TypeImplPtr retType (boost::shared_static_cast<TypeImpl> (
	  boost::shared_static_cast<ScopeImpl> (innerScope)->GetFunctionReturnType()));
	  
	RegisterID exprTargetReg (handler->AllocateRegister (*sequence, impl->GetValueType (), Intermediate));
	impl->AddToSequence (*this, exprTargetReg);
	if (retType->IsEqual (*(impl->GetValueType ())))
	{
	  retValReg = exprTargetReg;
	}
	else
	{
	  retValReg = handler->AllocateRegister (*sequence, retType, Intermediate);
	  handler->GenerateCast (*sequence, retValReg, retType,
				 exprTargetReg, impl->GetValueType ());
	}
      }
      SequenceOpPtr seqOp (boost::make_shared<SequenceOpReturn> (retValReg));
      sequence->AddOp (seqOp);
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
      
      if (!elseBlock)
      {
	elseBlock = handler->CreateBlock (GetInnerScope());
      }
      
      boost::shared_ptr<BlockImpl> ifBlockImpl (boost::shared_static_cast<BlockImpl> (ifBlock));
      boost::shared_ptr<BlockImpl> elseBlockImpl (boost::shared_static_cast<BlockImpl> (elseBlock));
      
      ifBlockImpl->FinishBlock();
      elseBlockImpl->FinishBlock();
      
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      // Collect registers read by the blocks
      std::vector<RegisterID> readRegistersIf;
      std::vector<RegisterID> readRegistersElse;
      {
	for (ImportedNamesMap::const_iterator import = ifBlockImpl->importedNames.begin();
	     import != ifBlockImpl->importedNames.end();
	     ++import)
	{
	  if ((boost::shared_ptr<ScopeImpl> (import->first->ownerScope) != blockScopeImpl)
	      && !import->second.initiallyWriteable)
	  {
	    RegisterID reg (ImportName (import->first, false));
	    sequence->SetIdentifierRegisterID (import->first->identifier, reg);
	    readRegistersIf.push_back (reg);
	  }
	}
	for (ImportedNamesMap::const_iterator import = elseBlockImpl->importedNames.begin();
	     import != elseBlockImpl->importedNames.end();
	     ++import)
	{
	  if ((boost::shared_ptr<ScopeImpl> (import->first->ownerScope) != blockScopeImpl)
	      && !import->second.initiallyWriteable)
	  {
	    RegisterID reg (ImportName (import->first, false));
	    sequence->SetIdentifierRegisterID (import->first->identifier, reg);
	    readRegistersElse.push_back (reg);
	  }
	}
      }
      
      /* Pass 'snapshot' of identifiers-to-register-ID map
         When resolving imports, the registers for variables _at the time of the block
         insertion_ is needed, hence the snapshot.
         Also, do it before allocating new registers for the 'written registers',
         as we want the ID before that */
      Sequence::IdentifierToRegIDMap identifierToRegIDMap (sequence->GetIdentifierToRegisterIDMap ());
      
      NameImplSet allExportedNames (ifBlockImpl->exportedNames);
      allExportedNames.insert (elseBlockImpl->exportedNames.begin(), elseBlockImpl->exportedNames.end());
      
      /* Check for names that are exported from one block but not the other.
	 In this case, the original value has to be copied inside the block from which it's not exported. */
      for (NameImplSet::const_iterator exportedName = allExportedNames.begin();
	    exportedName != allExportedNames.end();
	    exportedName++)
      {
	if (ifBlockImpl->exportedNames.find (*exportedName) == ifBlockImpl->exportedNames.end())
	{
	  ExpressionPtr targetExpr (handler->CreateVariableExpression (*exportedName));
	  ExpressionPtr valueExpr (handler->CreateVariableExpression (*exportedName));
	  ExpressionPtr assignExpr (handler->CreateAssignExpression (targetExpr, valueExpr));
	  ifBlockImpl->AddExpressionCommand (assignExpr); // FIXME: should prolly not change original block...
	}
	if (elseBlockImpl->exportedNames.find (*exportedName) == elseBlockImpl->exportedNames.end())
	{
	  ExpressionPtr targetExpr (handler->CreateVariableExpression (*exportedName));
	  ExpressionPtr valueExpr (handler->CreateVariableExpression (*exportedName));
	  ExpressionPtr assignExpr (handler->CreateAssignExpression (targetExpr, valueExpr));
	  elseBlockImpl->AddExpressionCommand (assignExpr); // FIXME: should prolly not change original block...
	}
      }
      
      // Generate register IDs for all values the nested blocks export
      typedef std::tr1::unordered_map<NameImplPtr, RegisterID> ExportedNamesMap;
      ExportedNamesMap seenExportedNames;
      std::vector<RegisterID> writtenRegistersIf;
      std::vector<RegisterID> writtenRegistersElse;
      {
	for (NameImplSet::const_iterator exportedName = ifBlockImpl->exportedNames.begin();
	     exportedName != ifBlockImpl->exportedNames.end();
	     exportedName++)
	{
	  RegisterID reg;
	  if (boost::shared_ptr<ScopeImpl> ((*exportedName)->ownerScope) == blockScopeImpl)
	  {
	    reg = (*exportedName)->GetRegister (handler, *this, true);
	  }
	  else
	  {
	    reg = ImportName ((*exportedName), true);
	    sequence->SetIdentifierRegisterID ((*exportedName)->identifier, reg);
	  }
	  writtenRegistersIf.push_back (reg);
	  seenExportedNames.insert (std::make_pair (*exportedName, reg));
	}
      }
      {
	for (NameImplSet::const_iterator exportedName = elseBlockImpl->exportedNames.begin();
	     exportedName != elseBlockImpl->exportedNames.end();
	     exportedName++)
	{
	  RegisterID reg;
	  
	  ExportedNamesMap::const_iterator prevName (seenExportedNames.find (*exportedName));
	  
	  if (prevName != seenExportedNames.end())
	  {
	    reg = prevName->second;
	  }
	  else
	  {
	    if (boost::shared_ptr<ScopeImpl> ((*exportedName)->ownerScope) == blockScopeImpl)
	    {
	      reg = (*exportedName)->GetRegister (handler, *this, true);
	    }
	    else
	    {
	      reg = ImportName ((*exportedName), true);
	      sequence->SetIdentifierRegisterID ((*exportedName)->identifier, reg);
	    }
	  }
	  writtenRegistersElse.push_back (reg);
	}
      }
      
      SequenceOpPtr seqOpIf (boost::make_shared<SequenceOpBlock> (ifBlockImpl->GetSequence(),
								  identifierToRegIDMap,
								  sequence->GetIdentifierToRegisterIDMap (),
								  readRegistersIf,
								  writtenRegistersIf));
      SequenceOpPtr seqOpElse (boost::make_shared<SequenceOpBlock> (elseBlockImpl->GetSequence(),
								    identifierToRegIDMap,
								    sequence->GetIdentifierToRegisterIDMap (),
								    readRegistersElse,
								    writtenRegistersElse));
      
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
    
    void IntermediateGeneratorSemanticsHandler::BlockImpl::GenerateGlobalVarInitialization ()
    {
      // Create a block with all assignments
      BlockPtr globalsInitBlock (handler->CreateBlock (innerScope));
      
      std::vector<NamePtr> globalVars (handler->globalScope->GetAllVars());
      for (std::vector<NamePtr>::const_iterator global (globalVars.begin());
	   global != globalVars.end();
	   ++global)
      {
	boost::shared_ptr<NameImpl> nameImpl (boost::shared_static_cast<NameImpl> (*global));
	if (nameImpl->varValue)
	{
	  // Synthesize an expression to assign the global with the default value
	  ExpressionPtr nameExpr (handler->CreateVariableExpression (nameImpl));
	  ExpressionPtr assignExpr (handler->CreateAssignExpression (nameExpr, nameImpl->varValue));
	  globalsInitBlock->AddExpressionCommand (assignExpr);
	}
      }
      
      SequenceOpPtr seqOp (CreateBlockSeqOp (globalsInitBlock, NameImplSet()));
      sequence->AddOp (seqOp);
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
	  RegisterID reg;
	  if (boost::shared_ptr<ScopeImpl> ((*exportedName)->ownerScope) == blockScopeImpl)
	  {
	    reg = (*exportedName)->GetRegister (handler, *this, !isLoopName);
	  }
	  else
	  {
	    reg = ImportName ((*exportedName), !isLoopName);
	    sequence->SetIdentifierRegisterID ((*exportedName)->identifier, reg);
	  }
	  writtenRegisters.push_back (reg);
	}
      }
      // Apply overrides for register IDs of exported identifiers
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
	impName.initialReg = reg;
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
