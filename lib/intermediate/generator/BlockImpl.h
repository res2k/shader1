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
#include "semantics/Block.h"

#include <outcome/outcome.hpp>

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler::BlockImpl : public semantics::Block
    {
    protected:
      IntermediateGeneratorSemanticsHandler* handler;
      semantics::ScopePtr innerScope;
      
      SequenceBuilderPtr sequenceBuilder;
      
      // Special, internal names
      NameImplPtr varCondition;
      typedef std::unordered_map<std::string, NameImplPtr> TernaryResultVarsMap;
      TernaryResultVarsMap varsTernaryResult;
      NameImplPtr varReturnValue;
      
      /**
       * Checks for new variables since last command and synthesizes initializations,
       * if necessary
       */
      void FlushVariableInitializers();

      /// Call handler 'Error' method with context of expression
      void ExpressionError (const semantics::Expression& expr, Error code);
      
      struct NameReg
      {
        bool isImported;
        RegisterPtr reg;
        RegisterPtr initialReg;
        bool initiallyWriteable;
        
        NameReg() : isImported (false) {}
      };
      typedef std::unordered_map<NameImplPtr, NameReg> NameRegMap;
      NameRegMap nameRegisters;
      NameImplSet exportedNames;
      
      SequenceOpPtr CreateBlockSeqOp (semantics::BlockPtr block,
                                      const ExpressionContext& errorContext,
                                      const NameImplSet& loopNames = NameImplSet());
    public:
      static const char varReturnValueName[];
      
      BlockImpl (IntermediateGeneratorSemanticsHandler* handler, semantics::ScopePtr innerScope);
      
      /**\name SemanticsHandler::Block implementation
       * @{ */
      void Finish() override { FlushVariableInitializers(); }

      semantics::ScopePtr GetInnerScope() { return innerScope; }
      
      void AddExpressionCommand (ExpressionPtr expr);
      void AddReturnCommand (ExpressionPtr returnValue);
      void AddBranching (ExpressionPtr branchCondition, semantics::BlockPtr ifBlock,
                         semantics::BlockPtr elseBlock);
      void AddWhileLoop (ExpressionPtr loopCond, semantics::BlockPtr loopBlock);
      void AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond, ExpressionPtr tailExpr,
                       semantics::BlockPtr loopBlock);
      void AddNestedBlock (semantics::BlockPtr block);
      /** @} */
      
      const SequencePtr& GetSequence();
      const SequenceBuilderPtr& GetSequenceBuilder();
      
      NameImplPtr GetTernaryResultName (const TypeImplPtr& resultType);

      typedef OUTCOME_V2_NAMESPACE::result<RegisterPtr, Error> result_RegisterPtr;
      result_RegisterPtr GetRegisterForName (const NameImplPtr& name, bool writeable);
      typedef OUTCOME_V2_NAMESPACE::result<void, Error> result_void;
      result_void OverrideNameRegister (const NameImplPtr& name, const RegisterPtr& reg);
      
      const NameImplSet& GetExportedNames() const { return exportedNames; }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BLOCKIMPL_H__
