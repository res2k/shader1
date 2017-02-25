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
#include "intermediate/CloningSequenceVisitor.h"

#include "intermediate/SequenceOp/SequenceOpArith.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceOp/SequenceOpCompare.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceOp/SequenceOpGetArrayLength.h"
#include "intermediate/SequenceOp/SequenceOpLogic.h"
#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"
#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"

#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>

namespace s1
{
  namespace intermediate
  {
    class CloningSequenceVisitor::BlockNestingSequenceVisitor
      : public CloningSequenceVisitor
    {
    public:
      BlockNestingSequenceVisitor (const SequenceBuilderPtr& newSequenceBuilder,
                                   CloningSequenceVisitor* owner)
       : CloningSequenceVisitor (newSequenceBuilder), owner (owner) {}

      void OpConstBool (const RegisterPtr& destination,
                        bool value)
      { assert (false); }
      void OpConstInt (const RegisterPtr& destination,
                        int value)
      { assert (false); }
      void OpConstUInt (const RegisterPtr& destination,
                        unsigned int value)
      { assert (false); }
      void OpConstFloat (const RegisterPtr& destination,
                          float value)
      { assert (false); }

      void OpAssign (const RegisterPtr& destination,
                      const RegisterPtr& source)
      { assert (false); }

      void OpCast (const RegisterPtr& destination,
                    BasicType destType,
                    const RegisterPtr& source)
      { assert (false); }

      void OpMakeVector (const RegisterPtr& destination,
                          BasicType compType,
                          const std::vector<RegisterPtr>& sources)
      { assert (false); }

      void OpMakeMatrix (const RegisterPtr& destination,
                          BasicType compType,
                          unsigned int matrixRows, unsigned int matrixCols,
                          const std::vector<RegisterPtr>& sources)
      { assert (false); }

      void OpMakeArray (const RegisterPtr& destination,
                        const std::vector<RegisterPtr>& sources)
      { assert (false); }
      void OpExtractArrayElement (const RegisterPtr& destination,
                                  const RegisterPtr& source,
                                  const RegisterPtr& index)
      { assert (false); }
      void OpChangeArrayElement (const RegisterPtr& destination,
                                  const RegisterPtr& source,
                                  const RegisterPtr& index,
                                  const RegisterPtr& newValue)
      { assert (false); }
      void OpGetArrayLength (const RegisterPtr& destination,
                              const RegisterPtr& array)
      { assert (false); }

      void OpExtractVectorComponent (const RegisterPtr& destination,
                                      const RegisterPtr& source,
                                      unsigned int comp)
      { assert (false); }

      void OpArith (const RegisterPtr& destination,
                    ArithmeticOp op,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2)
      { assert (false); }

      void OpLogic (const RegisterPtr& destination,
                    LogicOp op,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2)
      { assert (false); }

      void OpUnary (const RegisterPtr& destination,
                    UnaryOp op,
                    const RegisterPtr& source)
      { assert (false); }

      void OpCompare (const RegisterPtr& destination,
                      CompareOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2)
      { assert (false); }

      void OpBlock (const SequencePtr& seq,
                    const Sequence::IdentifierToRegMap& identToReg_imp,
                    const Sequence::IdentifierToRegMap& identToReg_exp)
      {
        owner->NestedBlock (this, seq, identToReg_imp, identToReg_exp);
      }

      void OpBranch (const RegisterPtr& conditionReg,
                      const SequenceOpPtr& seqOpIf,
                      const SequenceOpPtr& seqOpElse)
      { assert (false); }
      void OpWhile (const RegisterPtr& conditionReg,
                    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                    const SequenceOpPtr& seqOpBody)
      { assert (false); }

      void OpReturn (const std::vector<RegisterPtr>& outParamVals)
      { assert (false); }
      void OpFunctionCall (const uc::String& funcIdent,
                            const std::vector<RegisterPtr>& inParams,
                            const std::vector<RegisterPtr>& outParams)
      { assert (false); }
      void OpBuiltinCall (const RegisterPtr& destination,
                          BuiltinFunction what,
                          const std::vector<RegisterPtr>& inParams)
      { assert (false); }
    protected:
      CloningSequenceVisitor* owner;

      CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                    const RegisterMap& regMap)
      {
        assert (false);
        return 0;
      }
    };

    //-----------------------------------------------------------------------

    CloningSequenceVisitor::CloningSequenceVisitor (const SequenceBuilderPtr& newSequenceBuilder)
     : newSequenceBuilder (newSequenceBuilder)
    {}

    void CloningSequenceVisitor::NestedBlock (CloningSequenceVisitor* handlingVisitor,
                                             const SequencePtr& seq,
                                             const Sequence::IdentifierToRegMap& identToReg_imp,
                                             const Sequence::IdentifierToRegMap& identToReg_exp)
    {
      SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
      newSeqBuilder->AddImports (seq->GetImports ());
      newSeqBuilder->AddExports (seq->GetExports ());
      //newSeq->SetIdentifierRegisters  (seq->GetIdentifierToRegisterMap());

      RegisterMap regMap;
      for (Sequence::RegisterImpMappings::const_iterator imp = seq->GetImports().begin();
            imp != seq->GetImports().end();
            ++imp)
      {
        Sequence::IdentifierToRegMap::const_iterator thisSeqReg = identToReg_imp.find (imp->first);
        if (thisSeqReg != identToReg_imp.end())
          regMap[thisSeqReg->second] = imp->second;
      }
      for (Sequence::RegisterExpMappings::const_iterator exp = seq->GetExports().begin();
            exp != seq->GetExports().end();
            ++exp)
      {
        Sequence::IdentifierToRegMap::const_iterator thisSeqReg = identToReg_exp.find (exp->first);
        if (thisSeqReg != identToReg_exp.end())
          regMap[thisSeqReg->second] = exp->second;
      }

      {
        boost::scoped_ptr<CloningSequenceVisitor> visitor (Clone (newSeqBuilder, seq, regMap));
        if (VisitBackwards())
          seq->ReverseVisit (*visitor);
        else
          seq->Visit (*visitor);
        PostVisitSequence (visitor.get(), newSeqBuilder, regMap);
      }

      //CloningSequenceVisitor::OpBlock (newSeq, identToReg_imp, identToReg_exp);
      Sequence::IdentifierToRegMap newIdentToReg_imp;
      for (Sequence::IdentifierToRegMap::const_iterator i2r = identToReg_imp.begin();
           i2r != identToReg_imp.end();
           ++i2r)
      {
        newIdentToReg_imp.insert (std::make_pair (i2r->first, MapRegisterIn (i2r->second)));
      }

      Sequence::IdentifierToRegMap newIdentToReg_exp;
      for (Sequence::IdentifierToRegMap::const_iterator i2r = identToReg_exp.begin();
           i2r != identToReg_exp.end();
           ++i2r)
      {
        newIdentToReg_exp.insert (std::make_pair (i2r->first, MapRegisterOut (i2r->second)));
      }

      SequenceOpPtr newOp (new SequenceOpBlock (newSeqBuilder->GetSequence(),
                                                newIdentToReg_imp,
                                                newIdentToReg_exp));
      handlingVisitor->AddOpToSequence (newOp);
    }

    CloningSequenceVisitor* CloningSequenceVisitor::Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                                           const SequencePtr& oldSequence,
                                                           const RegisterMap& regMap)
    {
      return Clone (newSequenceBuilder, regMap);
    }

    void CloningSequenceVisitor::OpConstBool (const RegisterPtr& destination,
                                             bool value)
    {
      SequenceOpPtr newOp (new SequenceOpConst (MapRegisterOut (destination),
                                                value));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpConstInt (const RegisterPtr& destination,
                                            int value)
    {
      SequenceOpPtr newOp (new SequenceOpConst (MapRegisterOut (destination),
                                                value));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpConstUInt (const RegisterPtr& destination,
                                             unsigned int value)
    {
      SequenceOpPtr newOp (new SequenceOpConst (MapRegisterOut (destination),
                                                value));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpConstFloat (const RegisterPtr& destination,
                                              float value)
    {
      SequenceOpPtr newOp (new SequenceOpConst (MapRegisterOut (destination),
                                                value));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpAssign (const RegisterPtr& destination,
                                          const RegisterPtr& source)
    {
      SequenceOpPtr newOp (new SequenceOpAssign (MapRegisterOut (destination),
                                                 MapRegisterIn (source)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpCast (const RegisterPtr& destination,
                                        BasicType destType,
                                        const RegisterPtr& source)
    {
      SequenceOpPtr newOp (new SequenceOpCast (MapRegisterOut (destination),
                                               destType,
                                               MapRegisterIn (source)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpMakeVector (const RegisterPtr& destination,
                                              BasicType compType,
                                              const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      for(const RegisterPtr& source : sources)
      {
        newSources.push_back (MapRegisterIn (source));
      }

      SequenceOpPtr newOp (new SequenceOpMakeVector (MapRegisterOut (destination),
                                                     compType,
                                                     newSources));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpMakeMatrix (const RegisterPtr& destination,
                                              BasicType compType,
                                              unsigned int matrixRows, unsigned int matrixCols,
                                              const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      for(const RegisterPtr& source : sources)
      {
        newSources.push_back (MapRegisterIn (source));
      }

      SequenceOpPtr newOp (new SequenceOpMakeMatrix (MapRegisterOut (destination),
                                                     compType,
                                                     matrixRows, matrixCols,
                                                     newSources));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpMakeArray (const RegisterPtr& destination,
                                             const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      for(const RegisterPtr& source : sources)
      {
        newSources.push_back (MapRegisterIn (source));
      }

      SequenceOpPtr newOp (new SequenceOpMakeArray (MapRegisterOut (destination),
                                                    newSources));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpExtractArrayElement (const RegisterPtr& destination,
                                                       const RegisterPtr& source,
                                                       const RegisterPtr& index)
    {
      SequenceOpPtr newOp (new SequenceOpExtractArrayElement (MapRegisterOut (destination),
                                                              MapRegisterIn (source),
                                                              MapRegisterIn (index)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpChangeArrayElement (const RegisterPtr& destination,
                                                      const RegisterPtr& source,
                                                      const RegisterPtr& index,
                                                      const RegisterPtr& newValue)
    {
      SequenceOpPtr newOp (new SequenceOpChangeArrayElement (MapRegisterOut (destination),
                                                             MapRegisterIn (source),
                                                             MapRegisterIn (index),
                                                             MapRegisterIn (newValue)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpGetArrayLength (const RegisterPtr& destination,
                                                  const RegisterPtr& array)
    {
      SequenceOpPtr newOp (new SequenceOpGetArrayLength (MapRegisterOut (destination),
                                                         MapRegisterIn (array)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
                                                          const RegisterPtr& source,
                                                          unsigned int comp)
    {
      SequenceOpPtr newOp (new SequenceOpExtractVectorComponent (MapRegisterOut (destination),
                                                                 MapRegisterIn (source),
                                                                 comp));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpArith (const RegisterPtr& destination,
                                         ArithmeticOp op,
                                         const RegisterPtr& source1,
                                         const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (new SequenceOpArith (MapRegisterOut (destination),
                                                op,
                                                MapRegisterIn (source1),
                                                MapRegisterIn (source2)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpLogic (const RegisterPtr& destination,
                                         LogicOp op,
                                         const RegisterPtr& source1,
                                         const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (new SequenceOpLogic (MapRegisterOut (destination),
                                                op,
                                                MapRegisterIn (source1),
                                                MapRegisterIn (source2)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpUnary (const RegisterPtr& destination,
                                         UnaryOp op,
                                         const RegisterPtr& source)
    {
      SequenceOpPtr newOp (new SequenceOpUnaryOp (MapRegisterOut (destination),
                                                  op,
                                                  MapRegisterIn (source)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpCompare (const RegisterPtr& destination,
                                           CompareOp op,
                                           const RegisterPtr& source1,
                                           const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (new SequenceOpCompare (MapRegisterOut (destination),
                                                  op,
                                                  MapRegisterIn (source1),
                                                  MapRegisterIn (source2)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpBlock (const SequencePtr& seq,
                                         const Sequence::IdentifierToRegMap& identToReg_imp,
                                         const Sequence::IdentifierToRegMap& identToReg_exp)
    {
      NestedBlock (this, seq, identToReg_imp, identToReg_exp);
    }

    void CloningSequenceVisitor::OpBranch (const RegisterPtr& conditionReg,
                                          const SequenceOpPtr& seqOpIf,
                                          const SequenceOpPtr& seqOpElse)
    {
      SequenceOpPtr newSeqOpIf;
      if (seqOpIf)
      {
        SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
        BlockNestingSequenceVisitor visitor (newSeqBuilder, this);
        visitor.PreVisitOp (seqOpIf);
        seqOpIf->Visit (visitor);
        visitor.PostVisitOp ();

        SequencePtr newSeq (newSeqBuilder->GetSequence());
        assert (newSeq->GetNumOps() == 1);
        newSeqOpIf = newSeq->GetOp (0);
      }
      SequenceOpPtr newSeqOpElse;
      if (seqOpElse)
      {
        SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
        BlockNestingSequenceVisitor visitor (newSeqBuilder, this);
        visitor.PreVisitOp (seqOpElse);
        seqOpElse->Visit (visitor);
        visitor.PostVisitOp ();

        SequencePtr newSeq (newSeqBuilder->GetSequence());
        assert (newSeq->GetNumOps() == 1);
        newSeqOpElse = newSeq->GetOp (0);
      }

      SequenceOpPtr newOp (new SequenceOpBranch (MapRegisterIn (conditionReg),
                                                 newSeqOpIf,
                                                 newSeqOpElse));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpWhile (const RegisterPtr& conditionReg,
                                         const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                                         const SequenceOpPtr& seqOpBody)
    {
      typedef std::pair<RegisterPtr, RegisterPtr> RegPair;
      std::vector<RegPair> newLoopedRegs;
      for(const RegPair& loopedReg : loopedRegs)
      {
        newLoopedRegs.emplace_back (MapRegisterIn (loopedReg.first), loopedReg.second);
      }

      SequenceOpPtr newSeqOpBody;
      if (seqOpBody)
      {
        SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
        BlockNestingSequenceVisitor visitor (newSeqBuilder, this);
        visitor.PreVisitOp (seqOpBody);
        seqOpBody->Visit (visitor);
        visitor.PostVisitOp ();

        SequencePtr newSeq (newSeqBuilder->GetSequence());
        assert (newSeq->GetNumOps() == 1);
        newSeqOpBody = newSeq->GetOp (0);
      }

      for(RegPair& loopedReg : newLoopedRegs)
      {
        loopedReg.second = MapRegisterOut (loopedReg.second);
      }

      SequenceOpPtr newOp (new SequenceOpWhile (MapRegisterIn (conditionReg),
                                                newLoopedRegs,
                                                newSeqOpBody));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      std::vector<RegisterPtr> newOutParams;
      for(const RegisterPtr& outParam : outParamVals)
      {
        newOutParams.push_back (MapRegisterIn (outParam));
      }

      SequenceOpPtr newOp (new SequenceOpReturn (newOutParams));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpFunctionCall (const uc::String& funcIdent,
                                                const std::vector<RegisterPtr>& inParams,
                                                const std::vector<RegisterPtr>& outParams)
    {
      std::vector<RegisterPtr> newInParams;
      for(const RegisterPtr& inParam : inParams)
      {
        newInParams.push_back (MapRegisterIn (inParam));
      }
      std::vector<RegisterPtr> newOutParams;
      for(const RegisterPtr& outParam : outParams)
      {
        newOutParams.push_back (MapRegisterOut (outParam));
      }

      SequenceOpPtr newOp (new SequenceOpFunctionCall (funcIdent,
                                                       newInParams,
                                                       newOutParams));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpBuiltinCall (const RegisterPtr& destination,
                                               BuiltinFunction what,
                                               const std::vector<RegisterPtr>& inParams)
    {
      std::vector<RegisterPtr> newInParams;
      for(const RegisterPtr& inParam : inParams)
      {
        newInParams.push_back (MapRegisterIn (inParam));
      }

      SequenceOpPtr newOp (new SequenceOpBuiltinCall (MapRegisterOut (destination),
                                                      what,
                                                      newInParams));
      AddOpToSequence (newOp);
    }
  } // namespace intermediate
} // namespace s1
