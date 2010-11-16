#include "base/common.h"
#include "CommonSequenceVisitor.h"

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

namespace s1
{
  namespace optimize
  {
    CommonSequenceVisitor::CommonSequenceVisitor (const intermediate::SequencePtr& newSequence)
     : newSequence (newSequence)
    {}
    
    void CommonSequenceVisitor::OpConstBool (const RegisterPtr& destination,
					     bool value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpConst> (MapRegister (destination),
							   value));
      newSequence->AddOp (newOp);
    }
    
    void CommonSequenceVisitor::OpConstInt (const RegisterPtr& destination,
					    int value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpConst> (MapRegister (destination),
							   value));
      newSequence->AddOp (newOp);
    }
    
    void CommonSequenceVisitor::OpConstUInt (const RegisterPtr& destination,
					     unsigned int value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpConst> (MapRegister (destination),
							   value));
      newSequence->AddOp (newOp);
    }
    
    void CommonSequenceVisitor::OpConstFloat (const RegisterPtr& destination,
					      float value)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpConst> (MapRegister (destination),
							   value));
      newSequence->AddOp (newOp);
    }
    
    void CommonSequenceVisitor::OpAssign (const RegisterPtr& destination,
					  const RegisterPtr& source)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpAssign> (MapRegister (destination),
							    MapRegister (source)));
      newSequence->AddOp (newOp);
    }

    void CommonSequenceVisitor::OpCast (const RegisterPtr& destination,
					intermediate::BasicType destType,
					const RegisterPtr& source)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpCast> (MapRegister (destination),
							  destType,
							  MapRegister (source)));
      newSequence->AddOp (newOp);
    }

    void CommonSequenceVisitor::OpMakeVector (const RegisterPtr& destination,
					      intermediate::BasicType compType,
					      const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      BOOST_FOREACH(const RegisterPtr& source, sources)
      {
	newSources.push_back (MapRegister (source));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpMakeVector> (MapRegister (destination),
								compType,
								newSources));
      newSequence->AddOp (newOp);
    }
				  
    void CommonSequenceVisitor::OpMakeMatrix (const RegisterPtr& destination,
					      intermediate::BasicType compType,
					      unsigned int matrixRows, unsigned int matrixCols,
					      const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      BOOST_FOREACH(const RegisterPtr& source, sources)
      {
	newSources.push_back (MapRegister (source));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpMakeMatrix> (MapRegister (destination),
								compType,
								matrixRows, matrixCols,
								newSources));
      newSequence->AddOp (newOp);
    }
				  
    void CommonSequenceVisitor::OpMakeArray (const RegisterPtr& destination,
					     const std::vector<RegisterPtr>& sources)
    {
      std::vector<RegisterPtr> newSources;
      BOOST_FOREACH(const RegisterPtr& source, sources)
      {
	newSources.push_back (MapRegister (source));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpMakeArray> (MapRegister (destination),
							       newSources));
      newSequence->AddOp (newOp);
    }
				  
    void CommonSequenceVisitor::OpExtractArrayElement (const RegisterPtr& destination,
						       const RegisterPtr& source,
						       const RegisterPtr& index)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpExtractArrayElement> (MapRegister (destination),
									 MapRegister (source),
									 MapRegister (index)));
      newSequence->AddOp (newOp);
    }

    void CommonSequenceVisitor::OpChangeArrayElement (const RegisterPtr& destination,
						      const RegisterPtr& source,
						      const RegisterPtr& index,
						      const RegisterPtr& newValue)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpChangeArrayElement> (MapRegister (destination),
									MapRegister (source),
									MapRegister (index),
									MapRegister (newValue)));
      newSequence->AddOp (newOp);
    }
						      
    void CommonSequenceVisitor::OpGetArrayLength (const RegisterPtr& destination,
						  const RegisterPtr& array)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpGetArrayLength> (MapRegister (destination),
								    MapRegister (array)));
      newSequence->AddOp (newOp);
    }

    void CommonSequenceVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
							  const RegisterPtr& source,
							  unsigned int comp)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpExtractVectorComponent> (MapRegister (destination),
									    MapRegister (source),
									    comp));
      newSequence->AddOp (newOp);
    }
							  
    void CommonSequenceVisitor::OpArith (const RegisterPtr& destination,
					 ArithmeticOp op,
					 const RegisterPtr& source1,
					 const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpArith> (MapRegister (destination),
							   op,
							   MapRegister (source1),
							   MapRegister (source2)));
      newSequence->AddOp (newOp);
    }

    void CommonSequenceVisitor::OpLogic (const RegisterPtr& destination,
					 LogicOp op,
					 const RegisterPtr& source1,
					 const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpLogic> (MapRegister (destination),
							   op,
							   MapRegister (source1),
							   MapRegister (source2)));
      newSequence->AddOp (newOp);
    }

    void CommonSequenceVisitor::OpUnary (const RegisterPtr& destination,
					 UnaryOp op,
					 const RegisterPtr& source)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpUnaryOp> (MapRegister (destination),
							     op,
							     MapRegister (source)));
      newSequence->AddOp (newOp);
    }
			    
    void CommonSequenceVisitor::OpCompare (const RegisterPtr& destination,
					   CompareOp op,
					   const RegisterPtr& source1,
					   const RegisterPtr& source2)
    {
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpCompare> (MapRegister (destination),
							     op,
							     MapRegister (source1),
							     MapRegister (source2)));
      newSequence->AddOp (newOp);
    }
		      
    void CommonSequenceVisitor::OpBlock (const intermediate::SequencePtr& seq,
					 const Sequence::IdentifierToRegMap& identToReg_imp,
					 const Sequence::IdentifierToRegMap& identToReg_exp)
    {
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
	boost::make_shared<intermediate::SequenceOpBlock> (seq,
							   newIdentToReg_imp,
							   newIdentToReg_exp));
      newSequence->AddOp (newOp);
    }
		  
    void CommonSequenceVisitor::OpBranch (const RegisterPtr& conditionReg,
					  const intermediate::SequenceOpPtr& seqOpIf,
					  const intermediate::SequenceOpPtr& seqOpElse)
    {
      SequenceOpPtr newSeqOpIf;
      if (newSeqOpIf)
      {
	intermediate::SequencePtr newSeq (boost::make_shared<intermediate::Sequence> ());
	SequenceVisitor* visitor = Clone (newSeq);
	visitor->SetVisitedOp (seqOpIf);
	seqOpIf->Visit (*visitor);
	delete visitor;
	
	assert (newSeq->GetNumOps() == 1);
	newSeqOpIf = newSeq->GetOp (0);
      }
      SequenceOpPtr newSeqOpElse;
      if (newSeqOpElse)
      {
	intermediate::SequencePtr newSeq (boost::make_shared<intermediate::Sequence> ());
	SequenceVisitor* visitor = Clone (newSeq);
	visitor->SetVisitedOp (seqOpElse);
	seqOpElse->Visit (*visitor);
	delete visitor;
	
	assert (newSeq->GetNumOps() == 1);
	newSeqOpElse = newSeq->GetOp (0);
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpBranch> (MapRegister (conditionReg),
							    newSeqOpIf,
							    newSeqOpElse));
      newSequence->AddOp (newOp);
    }
		    
    void CommonSequenceVisitor::OpWhile (const RegisterPtr& conditionReg,
					 const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
					 const intermediate::SequenceOpPtr& seqOpBody)
    {
      SequenceOpPtr newSeqOpBody;
      if (seqOpBody)
      {
	intermediate::SequencePtr newSeq (boost::make_shared<intermediate::Sequence> ());
	SequenceVisitor* visitor = Clone (newSeq);
	visitor->SetVisitedOp (seqOpBody);
	seqOpBody->Visit (*visitor);
	delete visitor;
	
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
	boost::make_shared<intermediate::SequenceOpWhile> (MapRegister (conditionReg),
							   newLoopedRegs,
							   newSeqOpBody));
      newSequence->AddOp (newOp);
    }
		  
    void CommonSequenceVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      std::vector<RegisterPtr> newOutParams;
      BOOST_FOREACH(const RegisterPtr& outParam, outParamVals)
      {
	newOutParams.push_back (MapRegister (outParam));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpReturn> (newOutParams));
      newSequence->AddOp (newOp);
    }
    
    void CommonSequenceVisitor::OpFunctionCall (const UnicodeString& funcIdent,
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
	boost::make_shared<intermediate::SequenceOpFunctionCall> (funcIdent,
								  newInParams,
								  newOutParams));
      newSequence->AddOp (newOp);
    }
    
    void CommonSequenceVisitor::OpBuiltinCall (const RegisterPtr& destination,
					       intermediate::BuiltinFunction what,
					       const std::vector<RegisterPtr>& inParams)
    {
      std::vector<RegisterPtr> newInParams;
      BOOST_FOREACH(const RegisterPtr& inParam, inParams)
      {
	newInParams.push_back (MapRegister (inParam));
      }
      
      SequenceOpPtr newOp (
	boost::make_shared<intermediate::SequenceOpBuiltinCall> (MapRegister (destination),
								 what,
								 newInParams));
      newSequence->AddOp (newOp);
    }
  } // namespace optimize
} // namespace s1
