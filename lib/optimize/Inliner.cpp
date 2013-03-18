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
    class Inliner::InlineBlockVisitor : public CommonSequenceVisitor
    {
      unsigned int blockNum;
      bool& haveInlined;
    public:
      InlineBlockVisitor (const intermediate::SequencePtr& outputSeq,
			  bool& haveInlined)
       : CommonSequenceVisitor (outputSeq), blockNum (0), haveInlined (haveInlined)
      {
      }
      
      void OpBlock (const intermediate::SequencePtr& seq,
		    const Sequence::IdentifierToRegMap& identToRegID_imp,
		    const Sequence::IdentifierToRegMap& identToRegID_exp);

      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence,
				    const RegisterMap& regMap)
      {
	return new InlineBlockVisitor (newSequence, haveInlined);
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
      
      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence,
				    const RegisterMap& regMap)
      {
	/* Called for blocks in branch or while ops.
	   Inline contained ops, but not the block itself.
	   */
	bool haveInlined;
	return new InlineBlockVisitor (newSequence, haveInlined);
      }
    };
    
    //-----------------------------------------------------------------------
    
    void Inliner::InlineBlockVisitor::OpBlock (const intermediate::SequencePtr& seq,
					       const Sequence::IdentifierToRegMap& identToRegID_imp,
					       const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      haveInlined = true;
      if (seq->GetNumOps() == 0) return;
      
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
	RegisterPtr mapTo;
	if (mappedReg != identToRegID_imp.end())
	  visitor.AddRegisterMapping (imp->second, mappedReg->second);
	// else: Register is uninitialized. So don't map it to anything
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
      bool haveInlined (false);
      InlineBlockVisitor visitor (outputSeq, haveInlined);
      inputSeq->Visit (visitor);
      return haveInlined;
    }
  } // namespace optimize
} // namespace s1
