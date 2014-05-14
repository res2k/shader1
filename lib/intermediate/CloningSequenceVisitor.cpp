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

#include <boost/foreach.hpp>
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
      BlockNestingSequenceVisitor (const SequencePtr& newSequence,
				   CloningSequenceVisitor* owner)
       : CloningSequenceVisitor (newSequence), owner (owner) {}
      
      void SetVisitedOp (const SequenceOpPtr& op) { }
      
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
      void OpFunctionCall (const UnicodeString& funcIdent,
			    const std::vector<RegisterPtr>& inParams,
			    const std::vector<RegisterPtr>& outParams)
      { assert (false); }
      void OpBuiltinCall (const RegisterPtr& destination,
			  BuiltinFunction what,
			  const std::vector<RegisterPtr>& inParams)
      { assert (false); }
    protected:
      CloningSequenceVisitor* owner;
      
      CloningSequenceVisitor* Clone (const SequencePtr& newSequence,
				    const RegisterMap& regMap)
      {
	assert (false);
	return 0;
      }
    };
    
    //-----------------------------------------------------------------------
    
    CloningSequenceVisitor::CloningSequenceVisitor (const SequencePtr& newSequence)
     : newSequence (newSequence)
    {}

    void CloningSequenceVisitor::NestedBlock (CloningSequenceVisitor* handlingVisitor,
					     const SequencePtr& seq,
					     const Sequence::IdentifierToRegMap& identToReg_imp,
					     const Sequence::IdentifierToRegMap& identToReg_exp)
    {
      SequencePtr newSeq (boost::make_shared<Sequence> ());
      newSeq->AddImports (seq->GetImports ());
      newSeq->AddExports (seq->GetExports ());
      newSeq->SetIdentifierRegisters  (seq->GetIdentifierToRegisterMap());
      
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
	boost::scoped_ptr<CloningSequenceVisitor> visitor (Clone (newSeq, regMap));
	if (VisitBackwards())
	  seq->ReverseVisit (*visitor);
	else
	  seq->Visit (*visitor);
	PostVisitSequence (visitor.get(), newSeq, regMap);
      }
      
      //CloningSequenceVisitor::OpBlock (newSeq, identToReg_imp, identToReg_exp);
      Sequence::IdentifierToRegMap newIdentToReg_imp;
      for (Sequence::IdentifierToRegMap::const_iterator i2r = identToReg_imp.begin();
	   i2r != identToReg_imp.end();
	   ++i2r)
      {
	newIdentToReg_imp.insert (std::make_pair (i2r->first, MapRegister (i2r->second)));
      }
      
      Sequence::IdentifierToRegMap newIdentToReg_exp;
      for (Sequence::IdentifierToRegMap::const_iterator i2r = identToReg_exp.begin();
	   i2r != identToReg_exp.end();
	   ++i2r)
      {
	newIdentToReg_exp.insert (std::make_pair (i2r->first, MapRegister (i2r->second)));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpBlock> (newSeq,
							   newIdentToReg_imp,
							   newIdentToReg_exp));
      handlingVisitor->AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpConstBool (const RegisterPtr& destination,
					     bool value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpConst> (MapRegister (destination),
							   value));
      AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpConstInt (const RegisterPtr& destination,
					    int value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpConst> (MapRegister (destination),
							   value));
      AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpConstUInt (const RegisterPtr& destination,
					     unsigned int value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpConst> (MapRegister (destination),
							   value));
      AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpConstFloat (const RegisterPtr& destination,
					      float value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpConst> (MapRegister (destination),
							   value));
      AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpAssign (const RegisterPtr& destination,
					  const RegisterPtr& source)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpAssign> (MapRegister (destination),
							    MapRegister (source)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpCast (const RegisterPtr& destination,
					BasicType destType,
					const RegisterPtr& source)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpCast> (MapRegister (destination),
							  destType,
							  MapRegister (source)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpMakeVector (const RegisterPtr& destination,
					      BasicType compType,
					      const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      BOOST_FOREACH(const RegisterPtr& source, sources)
      {
	newSources.push_back (MapRegister (source));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpMakeVector> (MapRegister (destination),
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
      BOOST_FOREACH(const RegisterPtr& source, sources)
      {
	newSources.push_back (MapRegister (source));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpMakeMatrix> (MapRegister (destination),
								compType,
								matrixRows, matrixCols,
								newSources));
      AddOpToSequence (newOp);
    }
				  
    void CloningSequenceVisitor::OpMakeArray (const RegisterPtr& destination,
					     const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      BOOST_FOREACH(const RegisterPtr& source, sources)
      {
	newSources.push_back (MapRegister (source));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpMakeArray> (MapRegister (destination),
							       newSources));
      AddOpToSequence (newOp);
    }
				  
    void CloningSequenceVisitor::OpExtractArrayElement (const RegisterPtr& destination,
						       const RegisterPtr& source,
						       const RegisterPtr& index)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpExtractArrayElement> (MapRegister (destination),
									 MapRegister (source),
									 MapRegister (index)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpChangeArrayElement (const RegisterPtr& destination,
						      const RegisterPtr& source,
						      const RegisterPtr& index,
						      const RegisterPtr& newValue)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpChangeArrayElement> (MapRegister (destination),
									MapRegister (source),
									MapRegister (index),
									MapRegister (newValue)));
      AddOpToSequence (newOp);
    }
						      
    void CloningSequenceVisitor::OpGetArrayLength (const RegisterPtr& destination,
						  const RegisterPtr& array)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpGetArrayLength> (MapRegister (destination),
								    MapRegister (array)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
							  const RegisterPtr& source,
							  unsigned int comp)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpExtractVectorComponent> (MapRegister (destination),
									    MapRegister (source),
									    comp));
      AddOpToSequence (newOp);
    }
							  
    void CloningSequenceVisitor::OpArith (const RegisterPtr& destination,
					 ArithmeticOp op,
					 const RegisterPtr& source1,
					 const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpArith> (MapRegister (destination),
							   op,
							   MapRegister (source1),
							   MapRegister (source2)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpLogic (const RegisterPtr& destination,
					 LogicOp op,
					 const RegisterPtr& source1,
					 const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpLogic> (MapRegister (destination),
							   op,
							   MapRegister (source1),
							   MapRegister (source2)));
      AddOpToSequence (newOp);
    }

    void CloningSequenceVisitor::OpUnary (const RegisterPtr& destination,
					 UnaryOp op,
					 const RegisterPtr& source)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpUnaryOp> (MapRegister (destination),
							     op,
							     MapRegister (source)));
      AddOpToSequence (newOp);
    }
			    
    void CloningSequenceVisitor::OpCompare (const RegisterPtr& destination,
					   CompareOp op,
					   const RegisterPtr& source1,
					   const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpCompare> (MapRegister (destination),
							     op,
							     MapRegister (source1),
							     MapRegister (source2)));
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
	SequencePtr newSeq (boost::make_shared<Sequence> ());
	BlockNestingSequenceVisitor visitor (newSeq, this);
	visitor.SetVisitedOp (seqOpIf);
	seqOpIf->Visit (visitor);
	
	assert (newSeq->GetNumOps() == 1);
	newSeqOpIf = newSeq->GetOp (0);
      }
      SequenceOpPtr newSeqOpElse;
      if (seqOpElse)
      {
	SequencePtr newSeq (boost::make_shared<Sequence> ());
	BlockNestingSequenceVisitor visitor (newSeq, this);
	visitor.SetVisitedOp (seqOpElse);
	seqOpElse->Visit (visitor);
	
	assert (newSeq->GetNumOps() == 1);
	newSeqOpElse = newSeq->GetOp (0);
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpBranch> (MapRegister (conditionReg),
							    newSeqOpIf,
							    newSeqOpElse));
      AddOpToSequence (newOp);
    }
		    
    void CloningSequenceVisitor::OpWhile (const RegisterPtr& conditionReg,
					 const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
					 const SequenceOpPtr& seqOpBody)
    {
      SequenceOpPtr newSeqOpBody;
      if (seqOpBody)
      {
	SequencePtr newSeq (boost::make_shared<Sequence> ());
	BlockNestingSequenceVisitor visitor (newSeq, this);
	visitor.SetVisitedOp (seqOpBody);
	seqOpBody->Visit (visitor);
	
	assert (newSeq->GetNumOps() == 1);
	newSeqOpBody = newSeq->GetOp (0);
      }
      
      typedef std::pair<RegisterPtr, RegisterPtr> RegPair;
      std::vector<RegPair> newLoopedRegs;
      BOOST_FOREACH(const RegPair& loopedReg, loopedRegs)
      {
	newLoopedRegs.push_back (std::make_pair (MapRegister (loopedReg.first),
						 MapRegister (loopedReg.second)));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpWhile> (MapRegister (conditionReg),
							   newLoopedRegs,
							   newSeqOpBody));
      AddOpToSequence (newOp);
    }
		  
    void CloningSequenceVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      std::vector<RegisterPtr> newOutParams;
      BOOST_FOREACH(const RegisterPtr& outParam, outParamVals)
      {
	newOutParams.push_back (MapRegister (outParam));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpReturn> (newOutParams));
      AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpFunctionCall (const UnicodeString& funcIdent,
						const std::vector<RegisterPtr>& inParams,
						const std::vector<RegisterPtr>& outParams)
    {
      std::vector<RegisterPtr> newInParams;
      BOOST_FOREACH(const RegisterPtr& inParam, inParams)
      {
	newInParams.push_back (MapRegister (inParam));
      }
      std::vector<RegisterPtr> newOutParams;
      BOOST_FOREACH(const RegisterPtr& outParam, outParams)
      {
	newOutParams.push_back (MapRegister (outParam));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpFunctionCall> (funcIdent,
								  newInParams,
								  newOutParams));
      AddOpToSequence (newOp);
    }
    
    void CloningSequenceVisitor::OpBuiltinCall (const RegisterPtr& destination,
					       BuiltinFunction what,
					       const std::vector<RegisterPtr>& inParams)
    {
      std::vector<RegisterPtr> newInParams;
      BOOST_FOREACH(const RegisterPtr& inParam, inParams)
      {
	newInParams.push_back (MapRegister (inParam));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<SequenceOpBuiltinCall> (MapRegister (destination),
								 what,
								 newInParams));
      AddOpToSequence (newOp);
    }
  } // namespace intermediate
} // namespace s1
