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
      semantics::NameFunctionPtr functionName;
      ExpressionVector params;

      bool overloadSelected;
      semantics::BaseFunctionPtr overload;

      ExpressionVector actualParams;

      bool SelectOverload ();
      typedef std::vector<std::pair<RegisterPtr, RegisterPtr> > FetchedRegs;
      struct PostActionInfo
      {
        boost::intrusive_ptr<ExpressionImpl> expr;
        RegisterPtr reg;
        bool lValue;

        PostActionInfo (ExpressionImpl* expr, RegisterPtr reg, bool lValue)
         : expr (expr), reg (reg), lValue (lValue) {}
      };
      typedef std::vector<PostActionInfo> PostActions;
      bool FetchRegisters (s1::intermediate::IntermediateGeneratorSemanticsHandler::BlockImpl & block,
                           FetchedRegs& fetchedRegs, PostActions& postActions);

      SequenceOpPtr BuiltinOp (semantics::BuiltinFunction* builtin, RegisterPtr destination,
                               const std::vector<RegisterPtr>& inParams);
    public:
      FunctionCallExpressionImpl (IntermediateGeneratorSemanticsHandler* handler,
                                  ExpressionContext&& context,
                                  semantics::NameFunction* functionName,
                                  const ExpressionVector& params);

      semantics::TypePtr GetValueType ();
      RegisterPtr AddToSequence (BlockImpl& block, RegisterClassification classify,
                                 bool asLvalue = false);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_FUNCTIONCALLEXPRESSIONIMPL_H__
