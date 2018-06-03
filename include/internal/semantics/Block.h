/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_BLOCK_H_
#define SEMANTICS_BLOCK_H_

#include "forwarddecl.h"

namespace s1
{
  namespace semantics
  {
    /// A block of expression, branching or nesting commands.
    struct Block
    {
      virtual ~Block() {}

      /// Finish function. Call after block was filled.
      virtual void Finish() = 0;

      virtual ScopePtr GetInnerScope() = 0;
      
      virtual void AddExpressionCommand (ExpressionPtr expr) = 0;
      virtual void AddReturnCommand (ExpressionPtr returnValue) = 0;
      virtual void AddBranching (ExpressionPtr branchCondition, BlockPtr ifBlock,
                                  BlockPtr elseBlock) = 0;
      virtual void AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock) = 0;
      virtual void AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond, ExpressionPtr tailExpr,
                                BlockPtr loopBlock) = 0;
      virtual void AddNestedBlock (BlockPtr block) = 0;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_BLOCK_H_
