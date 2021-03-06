/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __TESTSEQUENCEVISITOR_H__
#define __TESTSEQUENCEVISITOR_H__

#include "intermediate/Sequence.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "intermediate/SequenceVisitor.h"

#include <boost/make_shared.hpp>

class TestSequenceVisitor : public s1::intermediate::SequenceVisitor
{
public:
  enum Operation
  {
    opConstBool,
    opConstInt,
    opConstUInt,
    opConstFloat,
    opAssignment,
    opCastToInvalid,
    opCastToBool,
    opCastToInt,
    opCastToUInt,
    opCastToFloat,
    opMakeVectorInvalid,
    opMakeVectorBool,
    opMakeVectorInt,
    opMakeVectorUInt,
    opMakeVectorFloat,
    opVectorCross,
    opVectorDot,
    opVectorLength,
    opVectorNormalize,
    opMakeMatrixInvalid,
    opMakeMatrixBool,
    opMakeMatrixInt,
    opMakeMatrixUInt,
    opMakeMatrixFloat,
    opMatrixLinAlgMul,
    opMakeArray,
    opExtractArrayElement,
    opExtractVectorComponent,
    opChangeArrayElement,
    opGetArrayLength,
    opArithAdd,
    opArithSub,
    opArithMul,
    opArithDiv,
    opArithMod,
    opLogicAnd,
    opLogicOr,
    opUnaryInv,
    opUnaryNeg,
    opUnaryNot,
    opCompareEq,
    opCompareNE,
    opCompareLE,
    opCompareLT,
    opCompareGE,
    opCompareGT,
    opBlock,
    opBranch,
    opWhile,
    opSampleTexture1D,
    opSampleTexture2D,
    opSampleTexture3D,
    opSampleTextureCUBE,
    opReturn,
    opFunctionCall,
    opBuiltinCall
  };
  typedef s1::intermediate::RegisterPtr RegisterPtr;
  typedef s1::intermediate::Sequence Sequence;
  struct SequenceEntry
  {
    Operation op;

    union
    {
      bool boolConst;
      int intConst;
      unsigned int uintConst;
      float floatConst;
    };

    RegisterPtr destReg;
    RegisterPtr sourceReg[3];
    std::vector<RegisterPtr> sourceRegs;

    unsigned int matrixRows;
    unsigned int matrixCols;

    unsigned int extractComp;

    s1::uc::String functionIdentifier;
    s1::intermediate::BuiltinFunction whatBuiltin;
    std::vector<RegisterPtr> inParams;
    std::vector<RegisterPtr> outParams;

    boost::shared_ptr<TestSequenceVisitor> blockVisitor;
    boost::shared_ptr<TestSequenceVisitor> branchIfVisitor;
    boost::shared_ptr<TestSequenceVisitor> branchElseVisitor;
    boost::shared_ptr<TestSequenceVisitor> whileVisitor;
  };

  std::vector<SequenceEntry> entries;

  void PreVisitOp (const s1::intermediate::SequenceOpPtr& op) override {}
  void PostVisitOp () override {}
  void VisitEnd() override {}

  void OpConstBool (const RegisterPtr& destination,
                    bool value) override
  {
    SequenceEntry entry;
    entry.op = opConstBool;
    entry.destReg = destination;
    entry.boolConst = value;
    entries.push_back (entry);
  }

  void OpConstInt (const RegisterPtr& destination,
                   int value) override
  {
    SequenceEntry entry;
    entry.op = opConstInt;
    entry.destReg = destination;
    entry.intConst = value;
    entries.push_back (entry);
  }

  void OpConstUInt (const RegisterPtr& destination,
                    unsigned int value) override
  {
    SequenceEntry entry;
    entry.op = opConstUInt;
    entry.destReg = destination;
    entry.uintConst = value;
    entries.push_back (entry);
  }

  void OpConstFloat (const RegisterPtr& destination,
                     float value) override
  {
    SequenceEntry entry;
    entry.op = opConstFloat;
    entry.destReg = destination;
    entry.floatConst = value;
    entries.push_back (entry);
  }


  void OpAssign (const RegisterPtr& destination,
                 const RegisterPtr& source) override
  {
    SequenceEntry entry;
    entry.op = opAssignment;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }


  void OpCast (const RegisterPtr& destination,
               s1::intermediate::BasicType destType,
               const RegisterPtr& source) override
  {
    SequenceEntry entry;
    switch (destType)
    {
    case s1::intermediate::BasicType::Invalid:
      entry.op = opCastToInvalid;
      break;
    case s1::intermediate::BasicType::Bool:
      entry.op = opCastToBool;
      break;
    case s1::intermediate::BasicType::Int:
      entry.op = opCastToInt;
      break;
    case s1::intermediate::BasicType::UInt:
      entry.op = opCastToUInt;
      break;
    case s1::intermediate::BasicType::Float:
      entry.op = opCastToFloat;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }


  void OpMakeVector (const RegisterPtr& destination,
                     s1::intermediate::BasicType compType,
                     const std::vector<RegisterPtr>& sources) override
  {
    SequenceEntry entry;
    switch (compType)
    {
    case s1::intermediate::BasicType::Invalid:
      entry.op = opMakeVectorInvalid;
      break;
    case s1::intermediate::BasicType::Bool:
      entry.op = opMakeVectorBool;
      break;
    case s1::intermediate::BasicType::Int:
      entry.op = opMakeVectorInt;
      break;
    case s1::intermediate::BasicType::UInt:
      entry.op = opMakeVectorUInt;
      break;
    case s1::intermediate::BasicType::Float:
      entry.op = opMakeVectorFloat;
      break;
    }
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entries.push_back (entry);
  }

  void OpVectorDot (const RegisterPtr& destination,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2) override
  {
    SequenceEntry entry;
    entry.op = opVectorDot;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }
  void OpVectorCross (const RegisterPtr& destination,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2) override
  {
    SequenceEntry entry;
    entry.op = opVectorCross;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }
  void OpVectorNormalize (const RegisterPtr& destination,
                          const RegisterPtr& source) override
  {
    SequenceEntry entry;
    entry.op = opVectorNormalize;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }
  void OpVectorLength (const RegisterPtr& destination,
                       const RegisterPtr& source) override
  {
    SequenceEntry entry;
    entry.op = opVectorLength;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }

  void OpMakeMatrix (const RegisterPtr& destination,
                     s1::intermediate::BasicType compType,
                     unsigned int matrixRows, unsigned int matrixCols,
                     const std::vector<RegisterPtr>& sources) override
  {
    SequenceEntry entry;
    switch (compType)
    {
    case s1::intermediate::BasicType::Invalid:
      entry.op = opMakeMatrixInvalid;
      break;
    case s1::intermediate::BasicType::Bool:
      entry.op = opMakeMatrixBool;
      break;
    case s1::intermediate::BasicType::Int:
      entry.op = opMakeMatrixInt;
      break;
    case s1::intermediate::BasicType::UInt:
      entry.op = opMakeMatrixUInt;
      break;
    case s1::intermediate::BasicType::Float:
      entry.op = opMakeMatrixFloat;
      break;
    }
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entry.matrixRows = matrixRows;
    entry.matrixCols = matrixCols;
    entries.push_back (entry);
  }

  void OpMatrixLinAlgMul (const RegisterPtr& destination,
                          const RegisterPtr& source1,
                          const RegisterPtr& source2) override
  {
    SequenceEntry entry;
    entry.op = opMatrixLinAlgMul;
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }


  void OpMakeArray (const RegisterPtr& destination,
                    const std::vector<RegisterPtr>& sources) override
  {
    SequenceEntry entry;
    entry.op = opMakeArray;
    entry.destReg = destination;
    entry.sourceRegs = sources;
    entries.push_back (entry);
  }

  void OpExtractArrayElement (const RegisterPtr& destination,
                              const RegisterPtr& source,
                              const RegisterPtr& index) override
  {
    SequenceEntry entry;
    entry.op = opExtractArrayElement;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.sourceReg[1] = index;
    entries.push_back (entry);
  }

  void OpChangeArrayElement (const RegisterPtr& destination,
                             const RegisterPtr& source,
                             const RegisterPtr& index,
                             const RegisterPtr& newValue) override
  {
    SequenceEntry entry;
    entry.op = opChangeArrayElement;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.sourceReg[1] = index;
    entry.sourceReg[2] = newValue;
    entries.push_back (entry);
  }

  void OpGetArrayLength (const RegisterPtr& destination,
                         const RegisterPtr& array) override
  {
    SequenceEntry entry;
    entry.op = opGetArrayLength;
    entry.destReg = destination;
    entry.sourceReg[0] = array;
    entries.push_back (entry);
  }


  void OpExtractVectorComponent (const RegisterPtr& destination,
                                 const RegisterPtr& source,
                                 unsigned int comp) override
  {
    SequenceEntry entry;
    entry.op = opExtractVectorComponent ;
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entry.extractComp = comp;
    entries.push_back (entry);
  }


  void OpArith (const RegisterPtr& destination,
                ArithmeticOp op,
                const RegisterPtr& source1,
                const RegisterPtr& source2) override
  {
    SequenceEntry entry;
    switch (op)
    {
    case Add:
      entry.op = opArithAdd;
      break;
    case Sub:
      entry.op = opArithSub;
      break;
    case Mul:
      entry.op = opArithMul;
      break;
    case Div:
      entry.op = opArithDiv;
      break;
    case Mod:
      entry.op = opArithMod;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }


  void OpLogic (const RegisterPtr& destination,
                LogicOp op,
                const RegisterPtr& source1,
                const RegisterPtr& source2) override
  {
    SequenceEntry entry;
    switch (op)
    {
    case And:
      entry.op = opLogicAnd;
      break;
    case Or:
      entry.op = opLogicOr;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }


  void OpUnary (const RegisterPtr& destination,
                UnaryOp op,
                const RegisterPtr& source) override
  {
    SequenceEntry entry;
    switch (op)
    {
    case Inv:
      entry.op = opUnaryInv;
      break;
    case Neg:
      entry.op = opUnaryNeg;
      break;
    case Not:
      entry.op = opUnaryNot;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source;
    entries.push_back (entry);
  }


  void OpCompare (const RegisterPtr& destination,
                  CompareOp op,
                  const RegisterPtr& source1,
                  const RegisterPtr& source2) override
  {
    SequenceEntry entry;
    switch (op)
    {
    case Eq:
      entry.op = opCompareEq;
      break;
    case NE:
      entry.op = opCompareNE;
      break;
    case LT:
      entry.op = opCompareLT;
      break;
    case LE:
      entry.op = opCompareLE;
      break;
    case GT:
      entry.op = opCompareGT;
      break;
    case GE:
      entry.op = opCompareGE;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = source1;
    entry.sourceReg[1] = source2;
    entries.push_back (entry);
  }

  void OpBlock (const boost::shared_ptr<Sequence>& seq,
                const Sequence::IdentifierToRegMap&,
                const Sequence::IdentifierToRegMap&) override
  {
    boost::shared_ptr<TestSequenceVisitor> blockVisitor = boost::make_shared<TestSequenceVisitor> ();
    seq->Visit (*blockVisitor);

    SequenceEntry entry;
    entry.op = opBlock;
    entry.blockVisitor = blockVisitor;
    entries.push_back (entry);
  }

  void OpBranch (const RegisterPtr& conditionReg,
                 const s1::intermediate::SequenceOpPtr& seqOpIf,
                 const s1::intermediate::SequenceOpPtr& seqOpElse) override
  {
    boost::shared_ptr<TestSequenceVisitor> ifVisitor = boost::make_shared<TestSequenceVisitor> ();
    if (seqOpIf) seqOpIf->Visit (*ifVisitor);
    boost::shared_ptr<TestSequenceVisitor> elseVisitor = boost::make_shared<TestSequenceVisitor> ();
    if (seqOpElse) seqOpIf->Visit (*elseVisitor);

    SequenceEntry entry;
    entry.op = opBranch;
    entry.sourceReg[0] = conditionReg;
    entry.branchIfVisitor = ifVisitor;
    entry.branchElseVisitor = elseVisitor;
    entries.push_back (entry);
  }

  void OpWhile (const RegisterPtr& conditionReg,
                const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                const s1::intermediate::SequenceOpPtr& seqOpBody) override
  {
    boost::shared_ptr<TestSequenceVisitor> whileVisitor = boost::make_shared<TestSequenceVisitor> ();
    seqOpBody->Visit (*whileVisitor);

    SequenceEntry entry;
    entry.op = opWhile;
    entry.sourceReg[0] = conditionReg;
    entry.blockVisitor = whileVisitor;
    entries.push_back (entry);
  }

  void OpSampleTexture (const RegisterPtr& destination,
                        SampleTextureOp what,
                        const RegisterPtr& sampler,
                        const RegisterPtr& coord) override
  {
    SequenceEntry entry;
    switch (what)
    {
    case tex1D:
      entry.op = opSampleTexture1D;
      break;
    case tex2D:
      entry.op = opSampleTexture2D;
      break;
    case tex3D:
      entry.op = opSampleTexture3D;
      break;
    case texCUBE:
      entry.op = opSampleTextureCUBE;
      break;
    }
    entry.destReg = destination;
    entry.sourceReg[0] = sampler;
    entry.sourceReg[1] = coord;
    entries.push_back (entry);
  }

  void OpReturn (const std::vector<RegisterPtr>& outParamVals) override
  {
    SequenceEntry entry;
    entry.op = opReturn;
    entry.sourceRegs = outParamVals;
    entries.push_back (entry);
  }

  void OpFunctionCall (const s1::uc::String& funcIdent,
                       const std::vector<RegisterPtr>& inParams,
                       const std::vector<RegisterPtr>& outParams) override
  {
    SequenceEntry entry;
    entry.op = opFunctionCall;
    entry.functionIdentifier = funcIdent;
    entry.inParams = inParams;
    entry.outParams = outParams;
    entries.push_back (entry);
  }

  void OpBuiltinCall (const RegisterPtr& destination,
                      s1::intermediate::BuiltinFunction what,
                      const std::vector<RegisterPtr>& inParams) override
  {
    SequenceEntry entry;
    entry.op = opBuiltinCall;
    entry.destReg = destination;
    entry.whatBuiltin = what;
    entry.inParams = inParams;
    entries.push_back (entry);
  }
};

#endif // __TESTSEQUENCEVISITOR_H__
