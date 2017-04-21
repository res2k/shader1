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

#include "base/common.h"
#include "optimize/Inliner.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"

#include "CommonSequenceVisitor.h"

#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace optimize
  {
    class Inliner::InlineBlockVisitor : public CommonSequenceVisitor
    {
      unsigned int blockNum;
      bool& haveInlined;
    public:
      InlineBlockVisitor (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                          bool& haveInlined)
       : CommonSequenceVisitor (outputSeqBuilder), blockNum (0), haveInlined (haveInlined)
      {
      }

      void OpBlock (const intermediate::SequencePtr& seq,
                    const Sequence::IdentifierToRegMap& identToRegID_imp,
                    const Sequence::IdentifierToRegMap& identToRegID_exp);

      CommonSequenceVisitor* Clone (const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                                    const RegisterMap& regMap)
      {
        return new InlineBlockVisitor (newSequenceBuilder, haveInlined);
      }
    };

    //-----------------------------------------------------------------------

    class Inliner::InliningVisitor : public CommonSequenceVisitor
    {
      uc::String regsSuffix;

      RegisterMap registerMap;
    public:
      InliningVisitor (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                       const uc::String& regsSuffix)
       : CommonSequenceVisitor (outputSeqBuilder), regsSuffix (regsSuffix)
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

        uc::String newRegName (reg->GetName());
        newRegName.append (regsSuffix);
        RegisterPtr newReg = newSequenceBuilder->AllocateRegister (reg->GetOriginalType(), newRegName);
        registerMap[reg] = newReg;
        return newReg;
      }

      CommonSequenceVisitor* Clone (const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                                    const RegisterMap& regMap)
      {
        /* Called for blocks in branch or while ops.
           Inline contained ops, but not the block itself.
           */
        bool haveInlined;
        return new InlineBlockVisitor (newSequenceBuilder, haveInlined);
      }
    };

    //-----------------------------------------------------------------------

    format::StaticFormatter FormatBlockSuffix ("$b{0}");

    void Inliner::InlineBlockVisitor::OpBlock (const intermediate::SequencePtr& seq,
                                               const Sequence::IdentifierToRegMap& identToRegID_imp,
                                               const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      haveInlined = true;
      if (seq->GetNumOps() == 0) return;

      uc::String blockSuffix;
      FormatBlockSuffix (blockSuffix, blockNum);
      blockNum++;

      InliningVisitor visitor (newSequenceBuilder, blockSuffix);
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

      intermediate::SequenceBuilderPtr seqToInline (boost::make_shared<intermediate::SequenceBuilder> ());
      Inliner::InlineAllBlocks (seqToInline, seq);
      seqToInline->GetSequence()->Visit (visitor);
    }

    //-----------------------------------------------------------------------

    bool Inliner::InlineAllBlocks (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                                   const intermediate::SequencePtr& inputSeq)
    {
      bool haveInlined (false);
      InlineBlockVisitor visitor (outputSeqBuilder, haveInlined);
      inputSeq->Visit (visitor);
      return haveInlined;
    }
  } // namespace optimize
} // namespace s1
