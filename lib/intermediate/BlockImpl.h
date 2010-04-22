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
      
      SequencePtr sequence;
    public:
      BlockImpl (IntermediateGeneratorSemanticsHandler* handler, ScopePtr innerScope);
      
      ScopePtr GetInnerScope() { return innerScope; }
      
      void AddExpressionCommand (ExpressionPtr expr);
      void AddReturnCommand (ExpressionPtr returnValue) {}
      void AddBranching (ExpressionPtr branchCondition, BlockPtr ifBlock,
			 BlockPtr elseBlock) {}
      void AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock) {}
      void AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond, ExpressionPtr tailExpr,
		       BlockPtr loopBlock) {}
      void AddNestedBlock (BlockPtr block) {}
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BLOCKIMPL_H__
