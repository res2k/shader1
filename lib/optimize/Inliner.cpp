#include "base/common.h"
#include "optimize/Inliner.h"

#include "intermediate/SequenceOp/SequenceOpBlock.h"

#include "CommonSequenceVisitor.h"

#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>
#include <unicode/ustdio.h>

namespace s1
{
  namespace optimize
  {
    class Inliner::InlineBranchBlockVisitor : public CommonSequenceVisitor
    {
    public:
      InlineBranchBlockVisitor (const intermediate::SequencePtr& outputSeq)
       : CommonSequenceVisitor (outputSeq) { }
      
      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence)
      {
	assert (false); /* ... since we're supposed to be used on a sequence with
			   only a single block op. */
	return new InlineBranchBlockVisitor (newSequence);
      }
      
      void OpBlock (const intermediate::SequencePtr& seq,
		    const Sequence::IdentifierToRegMap& identToReg_imp,
		    const Sequence::IdentifierToRegMap& identToReg_exp)
      {
	intermediate::SequencePtr newSeq (boost::make_shared<intermediate::Sequence> ());
	newSeq->AddImports (seq->GetImports ());
	newSeq->AddExports (seq->GetExports ());
	newSeq->SetIdentifierRegisters  (seq->GetIdentifierToRegisterMap());
	
	Inliner::InlineAllBlocks (newSeq, seq);
	
	SequenceOpPtr newOp (
	  boost::make_shared<intermediate::SequenceOpBlock> (newSeq,
							     identToReg_imp,
							     identToReg_exp));
	AddOpToSequence (newOp);
      }
    };
      
    //-----------------------------------------------------------------------
    
    class Inliner::InlineBlockVisitor : public CommonSequenceVisitor
    {
      unsigned int blockNum;
    public:
      InlineBlockVisitor (const intermediate::SequencePtr& outputSeq)
       : CommonSequenceVisitor (outputSeq), blockNum (0)
      {
      }
      
      bool HasInlined() const { return blockNum > 0; }
      
      void OpBlock (const intermediate::SequencePtr& seq,
		    const Sequence::IdentifierToRegMap& identToRegID_imp,
		    const Sequence::IdentifierToRegMap& identToRegID_exp);

      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence)
      {
	return new InlineBranchBlockVisitor (newSequence);
      }
    };
    
    //-----------------------------------------------------------------------
    
    class Inliner::InliningVisitor : public CommonSequenceVisitor
    {
      UnicodeString regsSuffix;
      
      typedef boost::unordered_map<RegisterPtr, RegisterPtr> RegisterMap;
      RegisterMap registerMap;
    public:
      InliningVisitor (const intermediate::SequencePtr& outputSeq,
		       const UnicodeString& regsSuffix)
       : CommonSequenceVisitor (outputSeq), regsSuffix (regsSuffix)
      {
      }
		       
      void AddRegisterMapping (const RegisterPtr& from, const RegisterPtr& to)
      {
	registerMap[from] = to;
      }
      
      RegisterPtr MapRegister (const RegisterPtr& reg)
      {
	RegisterMap::const_iterator mappedReg = registerMap.find (reg);
	if (mappedReg != registerMap.end())
	  return mappedReg->second;
	
	UnicodeString newRegName (reg->GetName());
	newRegName.append (regsSuffix);
	RegisterPtr newReg = newSequence->AllocateRegister (reg->GetOriginalType(), newRegName);
	registerMap[reg] = newReg;
	return newReg;
      }
      
      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence)
      {
	/* Called for branch or while ops.
	   The expected result is a sequence with a single block op.
	   */
	return new InlineBranchBlockVisitor (newSequence);
      }
    };
    
    //-----------------------------------------------------------------------
    
    void Inliner::InlineBlockVisitor::OpBlock (const intermediate::SequencePtr& seq,
					       const Sequence::IdentifierToRegMap& identToRegID_imp,
					       const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      UChar blockSuffix[charsToFormatUint + 3];
      u_snprintf (blockSuffix, sizeof (blockSuffix)/sizeof (UChar),
		  "$b%u", blockNum);
      blockNum++;
      
      InliningVisitor visitor (newSequence, blockSuffix);
      for (Sequence::RegisterImpMappings::const_iterator imp = seq->GetImports().begin();
	    imp != seq->GetImports().end();
	    ++imp)
      {
	Sequence::IdentifierToRegMap::const_iterator mappedReg = identToRegID_imp.find (imp->first);
	assert (mappedReg != identToRegID_imp.end());
	visitor.AddRegisterMapping (imp->second, mappedReg->second);
      }
      for (Sequence::RegisterExpMappings::const_iterator exp = seq->GetExports().begin();
	    exp != seq->GetExports().end();
	    ++exp)
      {
	Sequence::IdentifierToRegMap::const_iterator mappedReg = identToRegID_exp.find (exp->first);
	assert (mappedReg != identToRegID_exp.end());
	visitor.AddRegisterMapping (exp->second, mappedReg->second);
      }
      
      intermediate::SequencePtr seqToInline (boost::make_shared<intermediate::Sequence> ());
      Inliner::InlineAllBlocks (seqToInline, seq);
      seqToInline->Visit (visitor);
    }
      
    //-----------------------------------------------------------------------
    
    bool Inliner::InlineAllBlocks (const intermediate::SequencePtr& outputSeq,
				   const intermediate::SequencePtr& inputSeq)
    {
      InlineBlockVisitor visitor (outputSeq);
      inputSeq->Visit (visitor);
      return visitor.HasInlined();
    }
  } // namespace optimize
} // namespace s1
