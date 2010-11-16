#include "base/common.h"
#include "optimize/Inliner.h"

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
	return new InlineBlockVisitor (newSequence);
      }
    };
    
    //-----------------------------------------------------------------------
    
    class Inliner::InliningVisitor : public CommonSequenceVisitor
    {
      UnicodeString regsPrefix;
      
      typedef boost::unordered_map<RegisterPtr, RegisterPtr> RegisterMap;
      RegisterMap registerMap;
    public:
      InliningVisitor (const intermediate::SequencePtr& outputSeq,
		       const UnicodeString& regsPrefix)
       : CommonSequenceVisitor (outputSeq), regsPrefix (regsPrefix)
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
	
	UnicodeString newRegName (regsPrefix);
	newRegName.append (reg->GetName());
	RegisterPtr newReg = newSequence->AllocateRegister (reg->GetOriginalType(), newRegName);
	registerMap[reg] = newReg;
	return newReg;
      }
      
      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence)
      {
	/* Not called for a block op (these are inlined), only for branch or
	   while ops.
	   In this case, 'simple' block inlining is what we need, so create
	   an InlineBlockVisitor */
	return new InlineBlockVisitor (newSequence);
      }
    };
    
    //-----------------------------------------------------------------------
    
    void Inliner::InlineBlockVisitor::OpBlock (const intermediate::SequencePtr& seq,
					       const Sequence::IdentifierToRegMap& identToRegID_imp,
					       const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      UChar blockPrefix[charsToFormatUint + 3];
      u_snprintf (blockPrefix, sizeof (blockPrefix)/sizeof (UChar),
		  "b%u$", blockNum);
      blockNum++;
      
      InliningVisitor visitor (newSequence, blockPrefix);
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
