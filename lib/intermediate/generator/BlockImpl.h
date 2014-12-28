/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __INTERMEDIATE_BLOCKIMPL_H__
#define __INTERMEDIATE_BLOCKIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Sequence.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::BlockImpl : public Block
    {
    protected:
      IntermediateGeneratorSemanticsHandler* handler;
      ScopePtr innerScope;
      
      SequenceBuilderPtr sequenceBuilder;
      
      // Special, internal names
      NameImplPtr varCondition;
      typedef boost::unordered_map<std::string, NameImplPtr> TernaryResultVarsMap;
      TernaryResultVarsMap varsTernaryResult;
      NameImplPtr varReturnValue;
      
      /**
       * Checks for new variables since last command and synthesizes initializations,
       * if necessary
       */
      void FlushVariableInitializers();
      
      struct NameReg
      {
	bool isImported;
	RegisterPtr reg;
	RegisterPtr initialReg;
	bool initiallyWriteable;
	
	NameReg() : isImported (false) {}
      };
      typedef boost::unordered_map<NameImplPtr, NameReg> NameRegMap;
      NameRegMap nameRegisters;
      NameImplSet exportedNames;
      
      SequenceOpPtr CreateBlockSeqOp (BlockPtr block, const NameImplSet& loopNames = NameImplSet());
    public:
      static const char varReturnValueName[];
      
      BlockImpl (IntermediateGeneratorSemanticsHandler* handler, ScopePtr innerScope);
      
      /**\name SemanticsHandler::Block implementation
       * @{ */
      ScopePtr GetInnerScope() { return innerScope; }
      
      void AddExpressionCommand (ExpressionPtr expr);
      void AddReturnCommand (ExpressionPtr returnValue);
      void AddBranching (ExpressionPtr branchCondition, BlockPtr ifBlock,
			 BlockPtr elseBlock);
      void AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock);
      void AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond, ExpressionPtr tailExpr,
		       BlockPtr loopBlock);
      void AddNestedBlock (BlockPtr block);
      /** @} */
      
      const SequencePtr& GetSequence();
      const SequenceBuilderPtr& GetSequenceBuilder();
      
      void FinishBlock() { FlushVariableInitializers(); }
      void GenerateGlobalVarInitialization ();
      NameImplPtr GetTernaryResultName (const TypeImplPtr& resultType);
      
      RegisterPtr GetRegisterForName (const NameImplPtr& name, bool writeable);
      bool OverrideNameRegister (const NameImplPtr& name, const RegisterPtr& reg);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BLOCKIMPL_H__
