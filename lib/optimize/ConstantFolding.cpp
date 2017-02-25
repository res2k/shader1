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

#include "base/common.h"
#include "optimize/ConstantFolding.h"

#include "CommonSequenceVisitor.h"
#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include <boost/make_shared.hpp>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace optimize
  {
    class ConstantFolding::FoldingVisitor : public CommonSequenceVisitor
    {
      bool& seqChanged;

      struct ConstantBase
      {
        union
        {
          int i;
          unsigned int ui;
          float f;
          bool b;
        };
      };
      struct ConstantVal;
      typedef boost::shared_ptr<ConstantVal> ConstantValPtr;
      struct ConstantVal
      {
        // Single values, vector resp. matrix components
        ConstantBase comp[16];
        // Array elements
        size_t arraySize;
        typedef std::pair<ConstantValPtr, RegisterPtr> ConstantRegPair;
        ConstantRegPair* elements;

        ConstantVal() : arraySize (0), elements (0)
        { memset (comp, 0, sizeof (comp)); }
        ~ConstantVal() { delete[] elements; }

        ConstantVal& operator= (const ConstantVal& other)
        {
          memcpy (comp, other.comp, sizeof (comp));
          delete[] elements;
          if (other.arraySize != 0)
          {
            elements = new ConstantRegPair[other.arraySize];
            for (size_t a = 0; a < other.arraySize; a++)
            {
              elements[a] = other.elements[a];
            }
            arraySize = other.arraySize;
          }
          else
          {
            elements = 0;
            arraySize = 0;
          }
          return *this;
        }
      };

      typedef boost::unordered_map<RegisterPtr, ConstantValPtr> ConstRegsMap;
      ConstRegsMap constRegs;

      unsigned int foldRegNum;
      RegisterPtr NewConstReg (const intermediate::Sequence::TypePtr& type);
      static SequenceOpPtr MakeConstOp (const RegisterPtr& dest,
                                        const intermediate::Sequence::TypePtr& srcType,
                                        const ConstantBase& val);
      template<typename TargetType>
      static void DoCast (TargetType& target,
                          const intermediate::Sequence::TypePtr& srcType,
                          const ConstantValPtr& sourceVal);

      void ExtractTypeCompsAndBase (const intermediate::Sequence::TypePtr& srcType,
                                    unsigned int& nComps,
                                    intermediate::Sequence::TypePtr& baseType);
      void AddConstantOps (const RegisterPtr& destination,
                           const ConstantValPtr& newVal,
                           const intermediate::Sequence::TypePtr& destType,
                           unsigned int nComps,
                           const intermediate::Sequence::TypePtr& baseType);
      template<typename T, typename Functor>
      void Functor2Call (T& result, Functor& func,
                         const ConstantBase& src1Val,
                         const ConstantBase& src2Val,
                         const intermediate::Sequence::TypePtr& baseType);
      template<typename Functor>
      bool HandleBinaryOp (Functor& func,
                           const RegisterPtr& destination,
                           const RegisterPtr& source1,
                           const RegisterPtr& source2);
      template<typename Functor>
      bool HandleUnaryOp (Functor& func,
                          const RegisterPtr& destination,
                          const RegisterPtr& source);

      bool HandleBuiltinDot (const RegisterPtr& destination,
                             const intermediate::Sequence::TypePtr& srcType,
                             const ConstantValPtr& source1,
                             const ConstantValPtr& source2);
      bool HandleBuiltinCross (const RegisterPtr& destination,
                               const intermediate::Sequence::TypePtr& srcType,
                               const ConstantValPtr& source1,
                               const ConstantValPtr& source2);
      bool HandleBuiltinNormalize (const RegisterPtr& destination,
                                   const intermediate::Sequence::TypePtr& srcType,
                                   const ConstantValPtr& source1);
      bool HandleBuiltinLength (const RegisterPtr& destination,
                                const intermediate::Sequence::TypePtr& srcType,
                                const ConstantValPtr& source1);
      bool HandleBuiltinMatrixMul (const RegisterPtr& destination,
                                   const intermediate::Sequence::TypePtr& srcType,
                                   const ConstantValPtr& source1,
                                   const ConstantValPtr& source2,
                                   unsigned int src1Rows, unsigned int src1Cols,
                                   /* src2Rows = src1Cols */ unsigned int src2Cols);
      bool HandleBuiltinMin (const RegisterPtr& destination,
                             const RegisterPtr& source1,
                             const RegisterPtr& source2);
      bool HandleBuiltinMax (const RegisterPtr& destination,
                             const RegisterPtr& source1,
                             const RegisterPtr& source2);
      bool HandleBuiltinPow (const RegisterPtr& destination,
                             const RegisterPtr& source1,
                             const RegisterPtr& source2);
    public:
      FoldingVisitor (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                      bool& seqChanged,
                      const ConstRegsMap& constRegs = ConstRegsMap ())
       : CommonSequenceVisitor (outputSeqBuilder), seqChanged (seqChanged),
         constRegs (constRegs), foldRegNum (0)
      {
      }

      CommonSequenceVisitor* Clone (const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                                    const RegisterMap& regMap)
      {
        ConstRegsMap newConstRegs;
        typedef std::pair<RegisterPtr, ConstantValPtr> ConstRegPair;
        for(const ConstRegPair& constReg : constRegs)
        {
          RegisterMap::const_iterator newSeqReg = regMap.find (constReg.first);
          if (newSeqReg != regMap.end())
          {
            newConstRegs[newSeqReg->second] = constReg.second;
          }
        }
        return new FoldingVisitor (newSequenceBuilder, seqChanged, newConstRegs);
      }

      void PostVisitSequence (CommonSequenceVisitor* visitor,
                              const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                              const RegisterMap& regMap)
      {
        FoldingVisitor* foldVisitor = static_cast<FoldingVisitor*> (visitor);
        typedef std::pair<RegisterPtr, RegisterPtr> RegPair;
        for(const RegPair& mappedReg : regMap)
        {
          ConstRegsMap::const_iterator constReg = foldVisitor->constRegs.find (mappedReg.second);
          if (constReg != foldVisitor->constRegs.end ())
          {
            constRegs[mappedReg.first] = constReg->second;
          }
        }
      }

      void OpConstBool (const RegisterPtr& destination,
                        bool value);
      void OpConstInt (const RegisterPtr& destination,
                       int value);
      void OpConstUInt (const RegisterPtr& destination,
                        unsigned int value);
      void OpConstFloat (const RegisterPtr& destination,
                         float value);

      void OpAssign (const RegisterPtr& destination,
                     const RegisterPtr& source);

      void OpCast (const RegisterPtr& destination,
                   intermediate::BasicType destType,
                   const RegisterPtr& source);

      void OpMakeVector (const RegisterPtr& destination,
                         intermediate::BasicType compType,
                         const std::vector<RegisterPtr>& sources);
      void OpVectorDot (const RegisterPtr& destination,
                        const RegisterPtr& source1,
                        const RegisterPtr& source2) override;
      void OpVectorCross (const RegisterPtr& destination,
                          const RegisterPtr& source1,
                          const RegisterPtr& source2) override;
      void OpVectorNormalize (const RegisterPtr& destination,
                              const RegisterPtr& source) override;
      void OpVectorLength (const RegisterPtr& destination,
                           const RegisterPtr& source) override;

      void OpMakeMatrix (const RegisterPtr& destination,
                         intermediate::BasicType compType,
                         unsigned int matrixRows, unsigned int matrixCols,
                         const std::vector<RegisterPtr>& sources);

      void OpMakeArray (const RegisterPtr& destination,
                        const std::vector<RegisterPtr>& sources);
      void OpExtractArrayElement (const RegisterPtr& destination,
                                  const RegisterPtr& source,
                                  const RegisterPtr& index);
      void OpChangeArrayElement (const RegisterPtr& destination,
                                 const RegisterPtr& source,
                                 const RegisterPtr& index,
                                 const RegisterPtr& newValue);
      void OpGetArrayLength (const RegisterPtr& destination,
                             const RegisterPtr& array);

      void OpExtractVectorComponent (const RegisterPtr& destination,
                                     const RegisterPtr& source,
                                     unsigned int comp);

      void OpArith (const RegisterPtr& destination,
                    ArithmeticOp op,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2);

      void OpLogic (const RegisterPtr& destination,
                    LogicOp op,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2);

      void OpUnary (const RegisterPtr& destination,
                    UnaryOp op,
                    const RegisterPtr& source);

      void OpCompare (const RegisterPtr& destination,
                      CompareOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2);

      void OpBlock (const intermediate::SequencePtr& seq,
                    const Sequence::IdentifierToRegMap& identToRegID_imp,
                    const Sequence::IdentifierToRegMap& identToRegID_exp);

      void OpBranch (const RegisterPtr& conditionReg,
                     const intermediate::SequenceOpPtr& seqOpIf,
                     const intermediate::SequenceOpPtr& seqOpElse);
      void OpWhile (const RegisterPtr& conditionReg,
                    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                    const intermediate::SequenceOpPtr& seqOpBody);

      void OpReturn (const std::vector<RegisterPtr>& outParamVals);
      void OpFunctionCall (const uc::String& funcIdent,
                            const std::vector<RegisterPtr>& inParams,
                            const std::vector<RegisterPtr>& outParams);
      void OpBuiltinCall (const RegisterPtr& destination,
                          intermediate::BuiltinFunction what,
                          const std::vector<RegisterPtr>& inParams);
    };

    static format::StaticFormatter FormatFoldReg ("$fold{0}");

    intermediate::RegisterPtr
    ConstantFolding::FoldingVisitor::NewConstReg (const intermediate::Sequence::TypePtr& type)
    {
      uc::String regName;
      FormatFoldReg (regName, foldRegNum);
      foldRegNum++;
      return newSequenceBuilder->AllocateRegister (type, regName);
    }

    intermediate::SequenceOpPtr
    ConstantFolding::FoldingVisitor::MakeConstOp (const RegisterPtr& dest,
                                                  const intermediate::Sequence::TypePtr& srcType,
                                                  const ConstantBase& val)
    {
      SequenceOpPtr newOp;
      switch (srcType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        newOp = new intermediate::SequenceOpConst (dest, val.i);
        break;
      case parser::SemanticsHandler::UInt:
        newOp = new intermediate::SequenceOpConst (dest, val.ui);
        break;
      case parser::SemanticsHandler::Float:
        newOp = new intermediate::SequenceOpConst (dest, val.f);
        break;
      case parser::SemanticsHandler::Bool:
        newOp = new intermediate::SequenceOpConst (dest, val.b);
        break;
      default:
        assert (false);
      }
      return newOp;
    }

    void ConstantFolding::FoldingVisitor::OpConstBool (const RegisterPtr& destination,
                                                       bool value)
    {
      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      newVal->comp[0].b = value;
      constRegs[destination] = newVal;

      CommonSequenceVisitor::OpConstBool (destination, value);
    }

    void ConstantFolding::FoldingVisitor::OpConstInt (const RegisterPtr& destination,
                                                      int value)
    {
      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      newVal->comp[0].i = value;
      constRegs[destination] = newVal;

      CommonSequenceVisitor::OpConstInt (destination, value);
    }

    void ConstantFolding::FoldingVisitor::OpConstUInt (const RegisterPtr& destination,
                                                       unsigned int value)
    {
      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      newVal->comp[0].ui = value;
      constRegs[destination] = newVal;

      CommonSequenceVisitor::OpConstUInt (destination, value);
    }

    void ConstantFolding::FoldingVisitor::OpConstFloat (const RegisterPtr& destination,
                                                        float value)
    {
      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      newVal->comp[0].f = value;
      constRegs[destination] = newVal;

      CommonSequenceVisitor::OpConstFloat (destination, value);
    }

    void ConstantFolding::FoldingVisitor::OpAssign (const RegisterPtr& destination,
                                                    const RegisterPtr& source)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      if (srcConst != constRegs.end())
      {
        constRegs[destination] = srcConst->second;
      }

      CommonSequenceVisitor::OpAssign (destination, source);
    }

    template<typename TargetType>
    void ConstantFolding::FoldingVisitor::DoCast (TargetType& target,
                                                  const intermediate::Sequence::TypePtr& srcType,
                                                  const ConstantValPtr& sourceVal)
    {
      switch (srcType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        target = TargetType (sourceVal->comp[0].i);
        break;
      case parser::SemanticsHandler::UInt:
        target = TargetType (sourceVal->comp[0].ui);
        break;
      case parser::SemanticsHandler::Float:
        target = TargetType (sourceVal->comp[0].f);
        break;
      default:
        assert (false);
      }
    }

    void ConstantFolding::FoldingVisitor::OpCast (const RegisterPtr& destination,
                                                  intermediate::BasicType destType,
                                                  const RegisterPtr& source)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      if (srcConst != constRegs.end())
      {
        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();

        intermediate::Sequence::TypePtr srcType (source->GetOriginalType());
        assert (srcType->GetTypeClass() == parser::SemanticsHandler::Type::Base);

        switch (destType)
        {
        case intermediate::Int:
          DoCast (newVal->comp[0].i, srcType, srcConst->second);
          break;
        case intermediate::UInt:
          DoCast (newVal->comp[0].ui, srcType, srcConst->second);
          break;
        case intermediate::Float:
          DoCast (newVal->comp[0].f, srcType, srcConst->second);
          break;
        default:
          assert (false);
        }
        constRegs[destination] = newVal;

        SequenceOpPtr newOp (MakeConstOp (destination, destination->GetOriginalType(), newVal->comp[0]));
        AddOpToSequence (newOp);
        seqChanged = true;
      }
      else
      {
        CommonSequenceVisitor::OpCast (destination, destType, source);
      }
    }

    void ConstantFolding::FoldingVisitor::OpMakeVector (const RegisterPtr& destination,
                                                        intermediate::BasicType compType,
                                                        const std::vector<RegisterPtr>& sources)
    {
      ConstantValPtr srcConstVals[4];
      assert (sources.size() <= 4);
      bool allConst = true;
      for (size_t i = 0; i < sources.size(); i++)
      {
        ConstRegsMap::const_iterator srcConst = constRegs.find (sources[i]);
        if (srcConst == constRegs.end())
        {
          allConst = false;
          break;
        }
        srcConstVals[i] = srcConst->second;
      }

      if (allConst)
      {
        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        for (size_t i = 0; i < sources.size(); i++)
          newVal->comp[i] = srcConstVals[i]->comp[0];
        constRegs[destination] = newVal;
      }

      CommonSequenceVisitor::OpMakeVector (destination, compType, sources);
    }

    void ConstantFolding::FoldingVisitor::OpVectorDot (const RegisterPtr& destination,
                                                       const RegisterPtr& source1,
                                                       const RegisterPtr& source2)
    {
      ConstRegsMap::const_iterator src1Const = constRegs.find (source1);
      ConstRegsMap::const_iterator src2Const = constRegs.find (source2);
      if ((src1Const != constRegs.end())
          && (src2Const != constRegs.end()))
      {
        ConstantValPtr src1Val (src1Const->second);
        ConstantValPtr src2Val (src2Const->second);

        if (HandleBuiltinDot (destination,
                              source1->GetOriginalType(),
                              src1Val, src2Val))
        {
          return;
        }
      }

      CommonSequenceVisitor::OpVectorDot (destination, source1, source2);
    }

    void ConstantFolding::FoldingVisitor::OpVectorCross (const RegisterPtr& destination,
                                                         const RegisterPtr& source1,
                                                         const RegisterPtr& source2)
    {
      ConstRegsMap::const_iterator src1Const = constRegs.find (source1);
      ConstRegsMap::const_iterator src2Const = constRegs.find (source2);
      if ((src1Const != constRegs.end())
          && (src2Const != constRegs.end()))
      {
        ConstantValPtr src1Val (src1Const->second);
        ConstantValPtr src2Val (src2Const->second);

        if (HandleBuiltinCross (destination,
                                source1->GetOriginalType (),
                                src1Val, src2Val))
        {
          return;
        }
      }

      CommonSequenceVisitor::OpVectorCross (destination, source1, source2);
    }

    void ConstantFolding::FoldingVisitor::OpVectorNormalize (const RegisterPtr& destination,
                                                             const RegisterPtr& source)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      if (srcConst != constRegs.end())
      {
        ConstantValPtr srcVal (srcConst->second);

        if (HandleBuiltinNormalize (destination,
                                    source->GetOriginalType (),
                                    srcVal))
        {
          return;
        }
      }

      CommonSequenceVisitor::OpVectorNormalize (destination, source);
    }

    void ConstantFolding::FoldingVisitor::OpVectorLength (const RegisterPtr& destination,
                                                          const RegisterPtr& source)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      if (srcConst != constRegs.end())
      {
        ConstantValPtr srcVal (srcConst->second);

        if (HandleBuiltinLength (destination,
                                 source->GetOriginalType (),
                                 srcVal))
        {
          return;
        }
      }

      CommonSequenceVisitor::OpVectorLength (destination, source);
    }

    void ConstantFolding::FoldingVisitor::OpMakeMatrix (const RegisterPtr& destination,
                                                             intermediate::BasicType compType,
                                                             unsigned int matrixRows, unsigned int matrixCols,
                                                             const std::vector<RegisterPtr>& sources)
    {
      ConstantValPtr srcConstVals[4];
      assert (sources.size() == matrixRows*matrixCols);
      bool allConst = true;
      for (size_t i = 0; i < sources.size(); i++)
      {
        ConstRegsMap::const_iterator srcConst = constRegs.find (sources[i]);
        if (srcConst == constRegs.end())
        {
          allConst = false;
          break;
        }
        srcConstVals[i] = srcConst->second;
      }

      if (allConst)
      {
        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        for (size_t i = 0; i < sources.size(); i++)
          newVal->comp[i] = srcConstVals[i]->comp[0];
        constRegs[destination] = newVal;
      }

      CommonSequenceVisitor::OpMakeMatrix (destination, compType, matrixRows, matrixCols, sources);
    }

    void ConstantFolding::FoldingVisitor::OpMakeArray (const RegisterPtr& destination,
                                                       const std::vector<RegisterPtr>& sources)
    {
      std::vector<ConstantValPtr> srcConstVals;
      bool someConst = false;
      for (size_t i = 0; i < sources.size(); i++)
      {
        ConstRegsMap::const_iterator srcConst = constRegs.find (sources[i]);
        if (srcConst == constRegs.end())
        {
          srcConstVals.emplace_back ();
        }
        else
        {
          srcConstVals.push_back (srcConst->second);
          someConst = true;
        }
      }

      if (someConst)
      {
        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        newVal->arraySize = sources.size();
        newVal->elements = new ConstantVal::ConstantRegPair[sources.size()];
        for (size_t i = 0; i < sources.size(); i++)
        {
          if (srcConstVals[i])
            newVal->elements[i] = std::make_pair (srcConstVals[i], sources[i]);
          else
            newVal->elements[i] = std::make_pair (ConstantValPtr (), RegisterPtr ());
        }
        constRegs[destination] = newVal;
      }

      CommonSequenceVisitor::OpMakeArray (destination, sources);
    }

    void ConstantFolding::FoldingVisitor::OpExtractArrayElement (const RegisterPtr& destination,
                                                                 const RegisterPtr& source,
                                                                 const RegisterPtr& index)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      ConstRegsMap::const_iterator idxConst = constRegs.find (index);
      if ((srcConst != constRegs.end()) && (idxConst != constRegs.end()))
      {
        ConstantValPtr srcVal (srcConst->second);
        ConstantValPtr idxVal (idxConst->second);
        size_t arrayIdx = idxVal->comp[0].ui;
        if (arrayIdx < srcVal->arraySize)
        {
          ConstantVal::ConstantRegPair elemConst (srcVal->elements[arrayIdx]);
          if (elemConst.first)
          {
            ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
            *newVal = *(elemConst.first);
            constRegs[destination] = newVal;

            SequenceOpPtr newOp;
            newOp = new intermediate::SequenceOpAssign (destination, elemConst.second);
            AddOpToSequence (newOp);
            seqChanged = true;
            return;
          }
        }
      }

      CommonSequenceVisitor::OpExtractArrayElement (destination, source, index);
    }

    void ConstantFolding::FoldingVisitor::OpChangeArrayElement (const RegisterPtr& destination,
                                                                const RegisterPtr& source,
                                                                const RegisterPtr& index,
                                                                const RegisterPtr& newValue)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      ConstRegsMap::const_iterator idxConst = constRegs.find (index);
      ConstRegsMap::const_iterator newConst = constRegs.find (newValue);
      if ((srcConst != constRegs.end())
          && (idxConst != constRegs.end()))
      {
        ConstantValPtr srcVal (srcConst->second);
        ConstantValPtr idxVal (idxConst->second);
        size_t arrayIdx = idxVal->comp[0].ui;
        if (arrayIdx < srcVal->arraySize)
        {
          ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
          *newVal = *srcVal;
          newVal->elements[arrayIdx] = std::make_pair (newConst->second, newValue);
          constRegs[destination] = newVal;
        }
      }

      CommonSequenceVisitor::OpChangeArrayElement (destination, source, index, newValue);
    }

    void ConstantFolding::FoldingVisitor::OpGetArrayLength (const RegisterPtr& destination,
                                                            const RegisterPtr& array)
    {
      ConstRegsMap::const_iterator arrConst = constRegs.find (array);

      if (arrConst != constRegs.end())
      {
        ConstantValPtr arrVal (arrConst->second);
        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        // TODO: Overflow check
        newVal->comp[0].ui = static_cast<unsigned int> (arrVal->arraySize);
        constRegs[destination] = newVal;

        SequenceOpPtr newOp;
        newOp = new intermediate::SequenceOpConst (destination, (unsigned int)arrVal->arraySize);
        AddOpToSequence (newOp);
        seqChanged = true;
      }
      else
      {
        CommonSequenceVisitor::OpGetArrayLength (destination, array);
      }
    }

    void ConstantFolding::FoldingVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
                                                                    const RegisterPtr& source,
                                                                    unsigned int comp)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      if (srcConst != constRegs.end())
      {
        ConstantValPtr srcVal (srcConst->second);

        intermediate::Sequence::TypePtr compType (source->GetOriginalType()->GetArrayVectorMatrixBaseType());
        assert (compType->GetTypeClass() == parser::SemanticsHandler::Type::Base);

        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        switch (compType->GetBaseType())
        {
        case parser::SemanticsHandler::Int:
          newVal->comp[0].i = srcVal->comp[comp].i;
          break;
        case parser::SemanticsHandler::UInt:
          newVal->comp[0].ui = srcVal->comp[comp].ui;
          break;
        case parser::SemanticsHandler::Float:
          newVal->comp[0].f = srcVal->comp[comp].f;
          break;
        case parser::SemanticsHandler::Bool:
          newVal->comp[0].b = srcVal->comp[comp].b;
          break;
        default:
          assert (false);
        }
        constRegs[destination] = newVal;

        SequenceOpPtr newOp (MakeConstOp (destination, compType, newVal->comp[0]));
        AddOpToSequence (newOp);
        seqChanged = true;
        return;
      }

      CommonSequenceVisitor::OpExtractVectorComponent (destination, source, comp);
    }

    void ConstantFolding::FoldingVisitor::ExtractTypeCompsAndBase (const intermediate::Sequence::TypePtr& srcType,
                                                                   unsigned int& nComps,
                                                                   intermediate::Sequence::TypePtr& baseType)
    {
      switch (srcType->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
        nComps = 1;
        baseType = srcType;
        break;
      case parser::SemanticsHandler::Type::Vector:
        nComps = srcType->GetVectorTypeComponents();
        baseType = srcType->GetArrayVectorMatrixBaseType();
        break;
      case parser::SemanticsHandler::Type::Matrix:
        nComps = srcType->GetMatrixTypeCols() * srcType->GetMatrixTypeRows();
        baseType = srcType->GetArrayVectorMatrixBaseType();
        break;
      default:
        nComps = 0;
        assert (false);
      }
    }

    void ConstantFolding::FoldingVisitor::AddConstantOps (const RegisterPtr& destination,
                                                          const ConstantValPtr& newVal,
                                                          const intermediate::Sequence::TypePtr& destType,
                                                          unsigned int nComps,
                                                          const intermediate::Sequence::TypePtr& baseType)
    {
      intermediate::BasicType basicType;
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int: 	basicType = intermediate::Int; break;
      case parser::SemanticsHandler::UInt: 	basicType = intermediate::UInt; break;
      case parser::SemanticsHandler::Float: 	basicType = intermediate::Float; break;
      case parser::SemanticsHandler::Bool: 	basicType = intermediate::Bool; break;
      default:				assert (false);
      }

      SequenceOpPtr newOp;
      switch (destType->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
        newOp = MakeConstOp (destination, destType, newVal->comp[0]);
        break;
      case parser::SemanticsHandler::Type::Vector:
        {
          std::vector<RegisterPtr> vecRegs;
          for (size_t c = 0; c < nComps; c++)
          {
            RegisterPtr newReg (NewConstReg (baseType));
            AddOpToSequence (MakeConstOp (newReg, baseType, newVal->comp[c]));
            vecRegs.push_back (newReg);
          }
          newOp = new intermediate::SequenceOpMakeVector (destination, basicType, vecRegs);
        }
        break;
      case parser::SemanticsHandler::Type::Matrix:
        {
          std::vector<RegisterPtr> matRegs;
          for (size_t c = 0; c < nComps; c++)
          {
            RegisterPtr newReg (NewConstReg (baseType));
            AddOpToSequence (MakeConstOp (newReg, baseType, newVal->comp[c]));
            matRegs.push_back (newReg);
          }
          newOp = new intermediate::SequenceOpMakeMatrix (destination,
                                                          basicType,
                                                          destType->GetMatrixTypeRows(),
                                                          destType->GetMatrixTypeCols(),
                                                          matRegs);
        }
        break;
      default:
        assert (false);
      }

      AddOpToSequence (newOp);
      seqChanged = true;
    }

    template<typename T, typename Functor>
    void ConstantFolding::FoldingVisitor::Functor2Call (T& result, Functor& func,
                                                        const ConstantBase& src1Val,
                                                        const ConstantBase& src2Val,
                                                        const intermediate::Sequence::TypePtr& baseType)
    {
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        result = static_cast<T> (func(src1Val.i, src2Val.i));
        break;
      case parser::SemanticsHandler::UInt:
        result = static_cast<T> (func(src1Val.ui, src2Val.ui));
        break;
      case parser::SemanticsHandler::Float:
        result = static_cast<T> (func (src1Val.f, src2Val.f));
        break;
      case parser::SemanticsHandler::Bool:
        result = static_cast<T> (func(src1Val.b, src2Val.b));
        break;
      default:
        assert (false);
      }
    }

    template<typename Functor>
    bool ConstantFolding::FoldingVisitor::HandleBinaryOp (Functor& func,
                                                          const RegisterPtr& destination,
                                                          const RegisterPtr& source1,
                                                          const RegisterPtr& source2)
    {
      ConstRegsMap::const_iterator src1Const = constRegs.find (source1);
      ConstRegsMap::const_iterator src2Const = constRegs.find (source2);
      if ((src1Const != constRegs.end())
        && (src2Const != constRegs.end()))
      {
        ConstantValPtr src1Val (src1Const->second);
        ConstantValPtr src2Val (src2Const->second);

        unsigned int nComps;
        intermediate::Sequence::TypePtr baseType;

        intermediate::Sequence::TypePtr srcType (source1->GetOriginalType());
        ExtractTypeCompsAndBase (srcType, nComps, baseType);
        unsigned int nCompsDst;
        intermediate::Sequence::TypePtr baseTypeDst;
        ExtractTypeCompsAndBase (destination->GetOriginalType(), nCompsDst, baseTypeDst);

        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        switch (baseTypeDst->GetBaseType())
        {
        case parser::SemanticsHandler::Int:
          for (size_t c = 0; c < nComps; c++)
            Functor2Call (newVal->comp[c].i, func, src1Val->comp[c], src2Val->comp[c], baseType);
          break;
        case parser::SemanticsHandler::UInt:
          for (size_t c = 0; c < nComps; c++)
            Functor2Call (newVal->comp[c].ui, func, src1Val->comp[c], src2Val->comp[c], baseType);
          break;
        case parser::SemanticsHandler::Float:
          for (size_t c = 0; c < nComps; c++)
            Functor2Call (newVal->comp[c].f, func, src1Val->comp[c], src2Val->comp[c], baseType);
          break;
        case parser::SemanticsHandler::Bool:
          for (size_t c = 0; c < nComps; c++)
            Functor2Call (newVal->comp[c].b, func, src1Val->comp[c], src2Val->comp[c], baseType);
          break;
        default:
          assert (false);
        }
        constRegs[destination] = newVal;

        AddConstantOps (destination, newVal, destination->GetOriginalType(), nCompsDst, baseTypeDst);
        return true;
      }
      return false;
    }

    /* Disable some warnings on MSVC for code that's (should) not be executed
    * but is nevertheless generated. */
  #ifdef _MSC_VER
    #pragma warning (push)
    #pragma warning (disable: 4804)
  #endif

    namespace
    {
      class FunctorArith
      {
        intermediate::SequenceVisitor::ArithmeticOp op;

        template<typename T>
        static inline T Modulo (T a, T b)
        { return a % b; }

        static inline float Modulo (float a, float b)
        { return fmodf (a, b); }
      public:
        FunctorArith (intermediate::SequenceVisitor::ArithmeticOp op) : op (op) {}

        template<typename T>
        T operator() (T src1, T src2)
        {
          switch (op)
          {
          case intermediate::SequenceVisitor::Add:	return src1 + src2;
          case intermediate::SequenceVisitor::Sub:	return src1 - src2;
          case intermediate::SequenceVisitor::Mul:	return src1 * src2;
          case intermediate::SequenceVisitor::Div:	return src1 / src2;
          case intermediate::SequenceVisitor::Mod:	return Modulo (src1, src2);
          default:					assert (false); return 0;
          }
        }
      };
    }

  #ifdef _MSC_VER
    #pragma warning (pop)
  #endif

    void ConstantFolding::FoldingVisitor::OpArith (const RegisterPtr& destination,
                                                   ArithmeticOp op,
                                                   const RegisterPtr& source1,
                                                   const RegisterPtr& source2)
    {
      FunctorArith functor (op);
      if (HandleBinaryOp (functor, destination, source1, source2))
        return;

      CommonSequenceVisitor::OpArith (destination, op, source1, source2);
    }

    namespace
    {
      class FunctorLogic
      {
        intermediate::SequenceVisitor::LogicOp op;
      public:
        FunctorLogic (intermediate::SequenceVisitor::LogicOp op) : op (op) {}

        template<typename T>
        T operator() (T src1, T src2)
        {
          switch (op)
          {
          case intermediate::SequenceVisitor::And:	return src1 && src2;
          case intermediate::SequenceVisitor::Or:	return src1 || src2;
          default:					assert (false); return 0;
          }
        }
      };
    }

    void ConstantFolding::FoldingVisitor::OpLogic (const RegisterPtr& destination,
                                                   LogicOp op,
                                                   const RegisterPtr& source1,
                                                   const RegisterPtr& source2)
    {
      FunctorLogic functor (op);
      if (HandleBinaryOp (functor, destination, source1, source2))
        return;

      CommonSequenceVisitor::OpLogic (destination, op, source1, source2);
    }

    template<typename Functor>
    bool ConstantFolding::FoldingVisitor::HandleUnaryOp (Functor& func,
                                                         const RegisterPtr& destination,
                                                         const RegisterPtr& source)
    {
      ConstRegsMap::const_iterator srcConst = constRegs.find (source);
      if (srcConst != constRegs.end())
      {
        ConstantValPtr srcVal (srcConst->second);

        unsigned int nComps;
        intermediate::Sequence::TypePtr baseType;

        intermediate::Sequence::TypePtr srcType (source->GetOriginalType());
        ExtractTypeCompsAndBase (srcType, nComps, baseType);

        ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
        switch (baseType->GetBaseType())
        {
        case parser::SemanticsHandler::Int:
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].i = func (srcVal->comp[c].i);
          break;
        case parser::SemanticsHandler::UInt:
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].ui = func (srcVal->comp[c].ui);
          break;
        case parser::SemanticsHandler::Float:
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].f = func (srcVal->comp[c].f);
          break;
        case parser::SemanticsHandler::Bool:
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].b = func (srcVal->comp[c].b);
          break;
        default:
          assert (false);
        }
        constRegs[destination] = newVal;

        AddConstantOps (destination, newVal, srcType, nComps, baseType);
        return true;
      }
      return false;
    }

    /* Disable some warnings on MSVC for code that's (should) not be executed
     * but is nevertheless generated. */
  #ifdef _MSC_VER
    #pragma warning (push)
    #pragma warning (disable: 4146 4804)
  #endif

    namespace
    {
      class FunctorUnary
      {
        intermediate::SequenceVisitor::UnaryOp op;
      public:
        FunctorUnary (intermediate::SequenceVisitor::UnaryOp op) : op (op) {}

        template<typename T>
        T operator() (T src)
        {
          switch (op)
          {
          case intermediate::SequenceVisitor::Neg:	return -src;
          case intermediate::SequenceVisitor::Not:	return !src;
          case intermediate::SequenceVisitor::Inv:	return ~src;
          default:					assert (false); return 0;
          }
        }

        float operator() (float src)
        {
          switch (op)
          {
          case intermediate::SequenceVisitor::Neg:	return -src;
          case intermediate::SequenceVisitor::Not:	return !src;
          default:					assert (false); return 0;
          }
        }
      };
    }

  #ifdef _MSC_VER
    #pragma warning (pop)
  #endif

    void ConstantFolding::FoldingVisitor::OpUnary (const RegisterPtr& destination,
                                                   UnaryOp op,
                                                   const RegisterPtr& source)
    {
      FunctorUnary functor (op);
      if (HandleUnaryOp (functor, destination, source))
        return;

      CommonSequenceVisitor::OpUnary (destination, op, source);
    }

    namespace
    {
      class FunctorCompare
      {
        intermediate::SequenceVisitor::CompareOp op;
      public:
        FunctorCompare (intermediate::SequenceVisitor::CompareOp op) : op (op) {}

        template<typename T>
        bool operator() (T src1, T src2)
        {
          switch (op)
          {
          case intermediate::SequenceVisitor::Eq:	return src1 == src2;
          case intermediate::SequenceVisitor::NE:	return src1 != src2;
          case intermediate::SequenceVisitor::LE:	return src1 <= src2;
          case intermediate::SequenceVisitor::LT:	return src1 < src2;
          case intermediate::SequenceVisitor::GE:	return src1 >= src2;
          case intermediate::SequenceVisitor::GT:	return src1 > src2;
          default:					assert (false); return 0;
          }
        }
      };
    }

    void ConstantFolding::FoldingVisitor::OpCompare (const RegisterPtr& destination,
                                                     CompareOp op,
                                                     const RegisterPtr& source1,
                                                     const RegisterPtr& source2)
    {
      FunctorCompare functor (op);
      if (HandleBinaryOp (functor, destination, source1, source2))
        return;

      CommonSequenceVisitor::OpCompare (destination, op, source1, source2);
    }

    void ConstantFolding::FoldingVisitor::OpBlock (const intermediate::SequencePtr& seq,
                                                   const Sequence::IdentifierToRegMap& identToRegID_imp,
                                                   const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      /* Not much to do; mapping nested sequence regs to const data is done by
         Clone() & PostVisitSequence() */
      CommonSequenceVisitor::OpBlock (seq, identToRegID_imp, identToRegID_exp);
    }

    void ConstantFolding::FoldingVisitor::OpBranch (const RegisterPtr& conditionReg,
                                                    const intermediate::SequenceOpPtr& seqOpIf,
                                                    const intermediate::SequenceOpPtr& seqOpElse)
    {
      ConstRegsMap::const_iterator condConst = constRegs.find (conditionReg);
      if (condConst != constRegs.end())
      {
        ConstantValPtr condVal (condConst->second);

        intermediate::SequenceOpPtr branchOp;
        if (condVal->comp[0].b)
          branchOp = seqOpIf;
        else
          branchOp = seqOpElse;

        if (branchOp)
        {
          PreVisitOp (branchOp);
          branchOp->Visit (*this);
          PostVisitOp ();
        }

        seqChanged = true;
        return;
      }

      // Save constant regs ...
      ConstRegsMap saveConstRegs (constRegs);
      CommonSequenceVisitor::OpBranch (conditionReg, seqOpIf, seqOpElse);
      // ... and restore - otherwise, the values from the 'else' block would be used as constants
      constRegs = saveConstRegs;
    }

    void ConstantFolding::FoldingVisitor::OpWhile (const RegisterPtr& conditionReg,
                                                   const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                                                   const intermediate::SequenceOpPtr& seqOpBody)
    {
      RegisterPtr conditionRegInitial;
      typedef std::pair<RegisterPtr, RegisterPtr> RegisterPair;
      for(const RegisterPair& loopedReg : loopedRegs)
      {
        if (loopedReg.second == conditionReg)
        {
          conditionRegInitial = loopedReg.first;
          break;
        }
      }
      assert (conditionRegInitial);

      ConstRegsMap::const_iterator condConst = constRegs.find (conditionRegInitial);
      if (condConst != constRegs.end())
      {
        ConstantValPtr condVal (condConst->second);
        if (!condVal->comp[0].b)
        {
          // If initial condition is 'false' loop can be removed.

          // Copy const values for looped regs
          for(const RegisterPair& loopedReg : loopedRegs)
          {
            ConstRegsMap::const_iterator loopedRegConst = constRegs.find (loopedReg.first);
            if (loopedRegConst != constRegs.end())
            {
              constRegs[loopedReg.second] = loopedRegConst->second;
            }
          }

          seqChanged = true;
          return;
        }
      }

      // Save constant regs ...
      ConstRegsMap saveConstRegs (constRegs);
      // ... clear const values for looped regs (ops like "x=x+1" should not be folded!)
      for(const RegisterPair& loopedReg : loopedRegs)
      {
        ConstRegsMap::iterator loopedRegConst = constRegs.find (loopedReg.first);
        if (loopedRegConst != constRegs.end())
        {
          constRegs.erase (loopedRegConst);
        }
      }
      CommonSequenceVisitor::OpWhile (conditionReg, loopedRegs, seqOpBody);
      // ... and restore
      for(const RegisterPair& loopedReg : loopedRegs)
      {
        ConstRegsMap::iterator loopedRegConst = saveConstRegs.find (loopedReg.first);
        if (loopedRegConst != constRegs.end())
        {
          constRegs[loopedReg.first] = saveConstRegs[loopedReg.first];
        }
      }
    }

    void ConstantFolding::FoldingVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      // Not much to do here
      CommonSequenceVisitor::OpReturn (outParamVals);
    }

    void ConstantFolding::FoldingVisitor::OpFunctionCall (const uc::String& funcIdent,
                                                              const std::vector<RegisterPtr>& inParams,
                                                          const std::vector<RegisterPtr>& outParams)
    {
      /* If all input parameters are constant all outputs of the function must be
         constant, too. In that case, the function could be inlined + subjected
         to folding itself.
         However, arbitrary return placements make that tricky.
       */
      CommonSequenceVisitor::OpFunctionCall (funcIdent, inParams, outParams);
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinDot (const RegisterPtr& destination,
                                                            const intermediate::Sequence::TypePtr& srcType,
                                                            const ConstantValPtr& source1,
                                                            const ConstantValPtr& source2)
    {
      unsigned int nComps;
      intermediate::Sequence::TypePtr baseType;
      ExtractTypeCompsAndBase (srcType, nComps, baseType);

      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        {
          int sum (0);
          for (size_t c = 0; c < nComps; c++)
          {
            sum += source1->comp[c].i * source2->comp[c].i;
          }
          newVal->comp[0].i = sum;
        }
        break;
      case parser::SemanticsHandler::UInt:
        {
          unsigned int sum (0);
          for (size_t c = 0; c < nComps; c++)
          {
            sum += source1->comp[c].ui * source2->comp[c].ui;
          }
          newVal->comp[0].ui = sum;
        }
        break;
      case parser::SemanticsHandler::Float:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
          {
            sum += source1->comp[c].f * source2->comp[c].f;
          }
          newVal->comp[0].f = sum;
        }
        break;
      default:
        assert (false);
      }
      constRegs[destination] = newVal;

      AddOpToSequence (MakeConstOp (destination, baseType, newVal->comp[0]));
      seqChanged = true;
      return true;
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinCross (const RegisterPtr& destination,
                                                              const intermediate::Sequence::TypePtr& srcType,
                                                              const ConstantValPtr& source1,
                                                              const ConstantValPtr& source2)
    {
      unsigned int nComps;
      intermediate::Sequence::TypePtr baseType;
      ExtractTypeCompsAndBase (srcType, nComps, baseType);
      assert (nComps == 3);

      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        {
          for (size_t c = 0; c < 3; c++)
          {
            size_t i1 = (c + 1) % 3, i2 = (c + 2) % 3;
            newVal->comp[c].i = source1->comp[i1].i * source2->comp[i2].i
                              - source1->comp[i2].i * source2->comp[i1].i;
          }
        }
        break;
      case parser::SemanticsHandler::UInt:
        {
          for (size_t c = 0; c < 3; c++)
          {
            size_t i1 = (c + 1) % 3, i2 = (c + 2) % 3;
            newVal->comp[c].ui = source1->comp[i1].ui * source2->comp[i2].ui
                               - source1->comp[i2].ui * source2->comp[i1].ui;
          }
        }
        break;
      case parser::SemanticsHandler::Float:
        {
          for (size_t c = 0; c < 3; c++)
          {
            size_t i1 = (c + 1) % 3, i2 = (c + 2) % 3;
            newVal->comp[c].f = source1->comp[i1].f * source2->comp[i2].f
                              - source1->comp[i2].f * source2->comp[i1].f;
          }
        }
        break;
      default:
        assert (false);
      }
      constRegs[destination] = newVal;

      AddConstantOps (destination, newVal, srcType, nComps, baseType);
      seqChanged = true;
      return true;
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinNormalize (const RegisterPtr& destination,
                                                                  const intermediate::Sequence::TypePtr& srcType,
                                                                  const ConstantValPtr& source)
    {
      unsigned int nComps;
      intermediate::Sequence::TypePtr baseType;
      ExtractTypeCompsAndBase (srcType, nComps, baseType);
      unsigned int nCompsDst;
      intermediate::Sequence::TypePtr baseTypeDst;
      ExtractTypeCompsAndBase (destination->GetOriginalType(), nCompsDst, baseTypeDst);

      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
            sum += source->comp[c].i * source->comp[c].i;
          float invnorm = 1.0f/sqrtf (sum);
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].f = source->comp[c].i * invnorm;
        }
        break;
      case parser::SemanticsHandler::UInt:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
            sum += source->comp[c].ui * source->comp[c].ui;
          float invnorm = 1.0f/sqrtf (sum);
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].f = source->comp[c].ui * invnorm;
        }
        break;
      case parser::SemanticsHandler::Float:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
            sum += source->comp[c].f * source->comp[c].f;
          float invnorm = 1.0f/sqrtf (sum);
          for (size_t c = 0; c < nComps; c++)
            newVal->comp[c].f = source->comp[c].f * invnorm;
        }
        break;
      default:
        assert (false);
      }
      constRegs[destination] = newVal;

      AddConstantOps (destination, newVal,
                      destination->GetOriginalType(), nComps, baseTypeDst);
      seqChanged = true;
      return true;
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinLength (const RegisterPtr& destination,
                                                               const intermediate::Sequence::TypePtr& srcType,
                                                               const ConstantValPtr& source)
    {
      unsigned int nComps;
      intermediate::Sequence::TypePtr baseType;
      ExtractTypeCompsAndBase (srcType, nComps, baseType);
      unsigned int nCompsDst;
      intermediate::Sequence::TypePtr baseTypeDst;
      ExtractTypeCompsAndBase (destination->GetOriginalType(), nCompsDst, baseTypeDst);

      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
            sum += source->comp[c].i * source->comp[c].i;
          float len = sqrtf (sum);
          newVal->comp[0].f = len;
        }
        break;
      case parser::SemanticsHandler::UInt:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
            sum += source->comp[c].ui * source->comp[c].ui;
          float len = sqrtf (sum);
          newVal->comp[0].f = len;
        }
        break;
      case parser::SemanticsHandler::Float:
        {
          float sum (0);
          for (size_t c = 0; c < nComps; c++)
            sum += source->comp[c].f * source->comp[c].f;
          float len = sqrtf (sum);
          newVal->comp[0].f = len;
        }
        break;
      default:
        assert (false);
      }
      constRegs[destination] = newVal;

      AddOpToSequence (MakeConstOp (destination, destination->GetOriginalType(), newVal->comp[0]));
      seqChanged = true;
      return true;
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinMatrixMul (const RegisterPtr& destination,
                                                                  const intermediate::Sequence::TypePtr& srcType,
                                                                  const ConstantValPtr& source1,
                                                                  const ConstantValPtr& source2,
                                                                  unsigned int src1Rows, unsigned int src1Cols,
                                                                  /* src2Rows = src1Cols */ unsigned int src2Cols)
    {
      unsigned int nComps;
      intermediate::Sequence::TypePtr baseType;
      ExtractTypeCompsAndBase (srcType, nComps, baseType);
      unsigned int nCompsDst;
      intermediate::Sequence::TypePtr baseTypeDst;
      ExtractTypeCompsAndBase (destination->GetOriginalType(), nCompsDst, baseTypeDst);

      ConstantValPtr newVal = boost::make_shared<ConstantVal> ();
      switch (baseType->GetBaseType())
      {
      case parser::SemanticsHandler::Int:
        {
          for (unsigned int r = 0; r < src1Rows; r++)
          {
            for (unsigned int c = 0; c < src2Cols; c++)
            {
              int sum (0);
              for (unsigned int k = 0; k < src1Cols; k++)
              {
                unsigned int src1Index = r*src1Cols + k;
                unsigned int src2Index = k*src2Cols + c;
                sum += source1->comp[src1Index].i * source2->comp[src2Index].i;
              }
              unsigned int dstIndex = r*src2Cols + c;
              newVal->comp[dstIndex].i = sum;
            }
          }
        }
        break;
      case parser::SemanticsHandler::UInt:
        {
          for (unsigned int r = 0; r < src1Rows; r++)
          {
            for (unsigned int c = 0; c < src2Cols; c++)
            {
              unsigned int sum (0);
              for (unsigned int k = 0; k < src1Cols; k++)
              {
                unsigned int src1Index = r*src1Cols + k;
                unsigned int src2Index = k*src2Cols + c;
                sum += source1->comp[src1Index].ui * source2->comp[src2Index].ui;
              }
              unsigned int dstIndex = r*src2Cols + c;
              newVal->comp[dstIndex].ui = sum;
            }
          }
        }
        break;
      case parser::SemanticsHandler::Float:
        {
          for (unsigned int r = 0; r < src1Rows; r++)
          {
            for (unsigned int c = 0; c < src2Cols; c++)
            {
              float sum (0);
              for (unsigned int k = 0; k < src1Cols; k++)
              {
                unsigned int src1Index = r*src1Cols + k;
                unsigned int src2Index = k*src2Cols + c;
                sum += source1->comp[src1Index].f * source2->comp[src2Index].f;
              }
              unsigned int dstIndex = r*src2Cols + c;
              newVal->comp[dstIndex].f = sum;
            }
          }
        }
        break;
      default:
        assert (false);
      }
      constRegs[destination] = newVal;

      AddConstantOps (destination, newVal, destination->GetOriginalType(), nCompsDst, baseTypeDst);
      seqChanged = true;
      return true;
    }

    namespace
    {
      class FunctorMin
      {
      public:
        template<typename T>
        T operator() (T src1, T src2)
        {
          return std::min (src1, src2);
        }
      };
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinMin (const RegisterPtr& destination,
                                                            const RegisterPtr& source1,
                                                            const RegisterPtr& source2)
    {
      FunctorMin functor;
      return HandleBinaryOp (functor, destination, source1, source2);
    }

    namespace
    {
      class FunctorMax
      {
      public:
        template<typename T>
        T operator() (T src1, T src2)
        {
          return std::max (src1, src2);
        }
      };
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinMax (const RegisterPtr& destination,
                                                            const RegisterPtr& source1,
                                                            const RegisterPtr& source2)
    {
      FunctorMax functor;
      return HandleBinaryOp (functor, destination, source1, source2);
    }

    namespace
    {
      class FunctorPow
      {
      public:
        template<typename T>
        T operator() (T src1, T src2)
        {
          return pow (src1, src2);
        }
      };
      // Work around MSVC having multiple pow() overloads, but none for (unsigned) int
      template<>
      int FunctorPow::operator()<int> (int src1, int src2)
      {
        return static_cast<int> (pow (double (src1), src2));
      }
      template<>
      unsigned int FunctorPow::operator()<unsigned int> (unsigned int src1, unsigned int src2)
      {
        return static_cast<unsigned int> (pow(double(src1), double(src2)));
      }
      // May seem odd, but code for this is generated
      template<>
      bool FunctorPow::operator()<bool> (bool src1, bool src2)
      {
        return static_cast<int> (pow (float (int (src1)), float (int (src2)))) != 0;
      }
    }

    bool ConstantFolding::FoldingVisitor::HandleBuiltinPow (const RegisterPtr& destination,
                                                            const RegisterPtr& source1,
                                                            const RegisterPtr& source2)
    {
      FunctorPow functor;
      return HandleBinaryOp (functor, destination, source1, source2);
    }

    void ConstantFolding::FoldingVisitor::OpBuiltinCall (const RegisterPtr& destination,
                                                         intermediate::BuiltinFunction what,
                                                         const std::vector<RegisterPtr>& inParams)
    {
      ConstantValPtr paramConstVals[2];
      assert (inParams.size() <= 2);
      bool allConst = true;
      for (size_t i = 0; i < inParams.size(); i++)
      {
        ConstRegsMap::const_iterator paramConst = constRegs.find (inParams[i]);
        if (paramConst == constRegs.end())
        {
          allConst = false;
          break;
        }
        paramConstVals[i] = paramConst->second;
      }

      bool handled = false;
      if (allConst)
      {
        switch (what)
        {
        case intermediate::mul:
          {
            unsigned int rows1, cols1;
            switch (inParams[0]->GetOriginalType()->GetTypeClass())
            {
            case parser::SemanticsHandler::Type::Matrix:
              rows1 = inParams[0]->GetOriginalType()->GetMatrixTypeRows();
              cols1 = inParams[0]->GetOriginalType()->GetMatrixTypeCols();
              break;
            case parser::SemanticsHandler::Type::Vector:
              rows1 = 1;
              cols1 = inParams[0]->GetOriginalType()->GetVectorTypeComponents();
              break;
            default:
              assert (false);
            }
            unsigned int rows2, cols2;
            switch (inParams[1]->GetOriginalType()->GetTypeClass())
            {
            case parser::SemanticsHandler::Type::Matrix:
              rows2 = inParams[1]->GetOriginalType()->GetMatrixTypeRows();
              cols2 = inParams[1]->GetOriginalType()->GetMatrixTypeCols();
              break;
            case parser::SemanticsHandler::Type::Vector:
              rows2 = inParams[1]->GetOriginalType()->GetVectorTypeComponents();
              cols2 = 1;
              break;
            default:
              assert (false);
            }
            assert (cols1 == rows2);
            handled = HandleBuiltinMatrixMul (destination,
                                              inParams[0]->GetOriginalType(),
                                              paramConstVals[0], paramConstVals[1],
                                              rows1, cols1, cols2);
          }
          break;
        case intermediate::min:
          handled = HandleBuiltinMin (destination, inParams[0], inParams[1]);
          break;
        case intermediate::max:
          handled = HandleBuiltinMax (destination, inParams[0], inParams[1]);
          break;
        case intermediate::pow:
          handled = HandleBuiltinPow (destination, inParams[0], inParams[1]);
          break;
        default:
          break;
        }
        if (handled)
          return;
      }


      CommonSequenceVisitor::OpBuiltinCall (destination, what, inParams);
    }

    //-----------------------------------------------------------------------

    bool ConstantFolding::FoldConstants (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                                         const intermediate::SequencePtr& inputSeq)
    {
      bool seqChanged (false);
      FoldingVisitor visitor (outputSeqBuilder, seqChanged);
      inputSeq->Visit (visitor);
      return seqChanged;
    }
  } // namespace optimize
} // namespace s1
