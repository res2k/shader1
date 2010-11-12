#ifndef __INTERMEDIATE_BLOCKIMPL_H__
#define __INTERMEDIATE_BLOCKIMPL_H__

#include "base/unordered_set"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Sequence.h"

namespace std
{
  namespace tr1
  {
    template<typename T>
    struct hash<boost::shared_ptr<T> >
    {
      size_t operator() (const boost::shared_ptr<T>& ptr) const
      {
	return uintptr_t (ptr.get());
      }
    };
  } // namespace tr1
} // namespace std

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
      
      // Special, internal names
      NameImplPtr varCondition;
      typedef std::tr1::unordered_map<std::string, NameImplPtr> TernaryResultVarsMap;
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
      typedef std::tr1::unordered_map<NameImplPtr, NameReg> NameRegMap;
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
      
      const SequencePtr& GetSequence() const { return sequence; }
      SequencePtr& GetSequence() { return sequence; }
      
      void FinishBlock() { FlushVariableInitializers(); }
      void GenerateGlobalVarInitialization ();
      NameImplPtr GetTernaryResultName (const TypeImplPtr& resultType);
      
      RegisterPtr GetRegisterForName (const NameImplPtr& name, bool writeable);
      bool OverrideNameRegister (const NameImplPtr& name, const RegisterPtr& reg);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_BLOCKIMPL_H__
