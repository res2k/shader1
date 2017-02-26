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

#include "codegen/sl/SLSequenceCodeGenerator.h"

#include "base/format/Formatter.h"
#include "base/format/std_string.h"
#include "base/format/uc_String.h"
#include "codegen/sl/SLTraits.h"
#include "codegen/sl/SLOptions.h"

#include "intermediate/SequenceOp/SequenceOp.h"

#include <boost/make_shared.hpp>
#include <sstream>
#include <stdio.h>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::SequenceIdentifiersToRegIDsNameResolver (
        SequenceCodeGenerator* owner, const Sequence::IdentifierToRegMap& identToRegID_imp,
        const Sequence::IdentifierToRegMap& identToRegID_exp)
        : owner (owner), identToRegID_imp (identToRegID_imp), identToRegID_exp (identToRegID_exp) {}

      uc::String SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetImportedNameIdentifier (const uc::String& name)
      {
        Sequence::IdentifierToRegMap::const_iterator regIt = identToRegID_imp.find (name);
        if (regIt != identToRegID_imp.end ())
          return uc::String (owner->GetOutputRegisterName (regIt->second).c_str ());
        return uc::String ();
      }

      uc::String SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetExportedNameIdentifier (const uc::String& name)
      {
        Sequence::IdentifierToRegMap::const_iterator regIt = identToRegID_exp.find (name);
        if (regIt != identToRegID_exp.end ())
          return uc::String (owner->GetOutputRegisterName (regIt->second).c_str ());
        return uc::String ();
      }

      //-----------------------------------------------------------------------

      namespace
      {
        struct ParamHelper
        {
          std::string& str;
          bool firstParam;

          ParamHelper (std::string& str) : str (str), firstParam (true) {}
          void Add (const std::string& s)
          {
            if (!firstParam)
              str.append (", ");
            else
              firstParam = false;
            str.append (s);
          }
        };

        static const char* const vecCompStr[] = { "x", "y", "z", "w" };

        // Helper to build swizzles strings for consecutive components of the same vector
        class SwizzleHelper
        {
          ParamHelper& params;
          std::string currentName;
          std::string currentComps;
        public:
          SwizzleHelper (ParamHelper& params) : params (params) {}
          ~SwizzleHelper ()
          {
            Flush ();
          }
          // Aggregate swizzle components for subsequent components of the same var
          void Add (const std::string& name, unsigned int comp)
          {
            if (name != currentName)
            {
              Flush ();
              currentName = name;
            }
            currentComps.append (vecCompStr[comp]);
          }
          // Unconditionally write out the current aggregated swizzle
          void Flush ()
          {
            if (!currentName.empty ())
            {
              std::string s (currentName);
              s.append (".");
              s.append (currentComps);
              params.Add (s);
              currentName.clear ();
              currentComps.clear ();
            }
          }
        };
      }

      //-----------------------------------------------------------------------

      SequenceCodeGenerator::CodegenVisitor::CodegenVisitor (SequenceCodeGenerator* owner,
                                                             const StringsArrayPtr& target)
        : AnnotatingVisitor (target), owner (owner), emitEmptyBlocks (false)
      {
      }

      void SequenceCodeGenerator::CodegenVisitor::EmitAssign (const RegisterPtr& destination,
                                                              const char* value)
      {
        std::string targetName;
        if (!owner->GetOutputRegisterName (destination, targetName, value))
        {
          std::string line (targetName);
          line.append (" = ");
          line.append (value);
          line.append (";");
          target->AddString (line);
        }
      }

      void SequenceCodeGenerator::CodegenVisitor::PseudoAssign (const RegisterPtr& destination,
                                                                const char* value)
      {
        RegistersToIDMap::iterator regIt = owner->seenRegisters.find (destination);
        if (regIt != owner->seenRegisters.end ())
        {
          // Register has a name, emit assign
          EmitAssign (destination, value);
        }
        else
        {
          // Set 'name' of register to value...
          owner->seenRegisters[destination] = value;
        }
      }

      static format::StaticFormatter FormatAssign ("{0} = {1};");

      void SequenceCodeGenerator::CodegenVisitor::EmitAssign (const uc::String& destination,
                                                              const RegisterPtr& value)
      {
        target->AddString (FormatAssign.to<uc::String> (destination, owner->GetOutputRegisterName (value).c_str()));
      }

      void SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterPtr& destination,
                                                                    const uc::String& function,
                                                                    const char* paramsStr)
      {
        std::string rside;
        function.toUTF8String (rside);
        rside.append (" (");
        rside.append (paramsStr);
        rside.append (")");
        EmitAssign (destination, rside.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterPtr& destination,
                                                                    const char* function,
                                                                    const char* paramsStr)
      {
        std::string rside;
        rside.append (function);
        rside.append (" (");
        rside.append (paramsStr);
        rside.append (")");
        EmitAssign (destination, rside.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterPtr& destination,
                                                                    const char* function,
                                                                    const RegisterPtr& source1)
      {
        EmitFunctionCall (destination, function, owner->GetOutputRegisterName (source1).c_str ());
      }

      static format::StaticFormatter FormatBinaryArgs ("{0}, {1}");

      void SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterPtr& destination,
                                                                    const char* function,
                                                                    const RegisterPtr& source1,
                                                                    const RegisterPtr& source2)
      {
        EmitFunctionCall (destination, function,
                          FormatBinaryArgs.to<std::string> (owner->GetOutputRegisterName (source1),
                                                            owner->GetOutputRegisterName (source2)).c_str());
      }

      void SequenceCodeGenerator::CodegenVisitor::EmitBinary (const RegisterPtr& destination,
                                                              const RegisterPtr& source1,
                                                              const RegisterPtr& source2,
                                                              const char* op)
      {
        std::string source1Name (owner->GetOutputRegisterName (source1));
        std::string source2Name (owner->GetOutputRegisterName (source2));
        std::string rside (source1Name);
        rside.append (" ");
        rside.append (op);
        rside.append (" ");
        rside.append (source2Name);
        EmitAssign (destination, rside.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::EmitUnary (const RegisterPtr& destination,
                                                             const RegisterPtr& source,
                                                             const char* op)
      {
        std::string sourceName (owner->GetOutputRegisterName (source));
        std::string rside (op);
        rside.append (sourceName);
        EmitAssign (destination, rside.c_str ());
      }

      const char* SequenceCodeGenerator::CodegenVisitor::SamplingFunction (intermediate::SequenceVisitor::SampleTextureOp op) const
      {
        switch (op)
        {
        case intermediate::SequenceVisitor::tex1D:    return "tex1D";
        case intermediate::SequenceVisitor::tex2D:    return "tex2D";
        case intermediate::SequenceVisitor::tex3D:    return "tex3D";
        case intermediate::SequenceVisitor::texCUBE:  return "texCUBE";
        }
        return nullptr;
      }

      void SequenceCodeGenerator::CodegenVisitor::OpConstBool (const RegisterPtr& destination,
                                                               bool value)
      {
        AnnotatingVisitor::OpConstBool (destination, value);
        const char* valueStr = value ? "true" : "false";
        PseudoAssign (destination, valueStr);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpConstInt (const RegisterPtr& destination,
                                                              int value)
      {
        AnnotatingVisitor::OpConstInt (destination, value);
        std::stringstream valueStrStream;
        valueStrStream << value;
        PseudoAssign (destination, valueStrStream.str ().c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const RegisterPtr& destination,
                                                               unsigned int value)
      {
        AnnotatingVisitor::OpConstUInt (destination, value);
        std::stringstream valueStrStream;
        valueStrStream << value;
        PseudoAssign (destination, valueStrStream.str ().c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const RegisterPtr& destination,
                                                                float value)
      {
        AnnotatingVisitor::OpConstFloat (destination, value);
        std::stringstream valueStrStream;
        valueStrStream << value;
        PseudoAssign (destination, valueStrStream.str ().c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpAssign (const RegisterPtr& destination,
                                                            const RegisterPtr& source)
      {
        AnnotatingVisitor::OpAssign (destination, source);
        std::string sourceName (owner->GetOutputRegisterName (source));
        EmitAssign (destination, sourceName.c_str ());
      }


      void SequenceCodeGenerator::CodegenVisitor::OpCast (const RegisterPtr& destination,
                                                          intermediate::BasicType destType,
                                                          const RegisterPtr& source)
      {
        AnnotatingVisitor::OpCast (destination, destType, source);
        // Check if the destination register type matches the source register type
        bool typeMatch (false);
        if (source->GetOriginalType ()->GetTypeClass ()
            == parser::SemanticsHandler::Type::Base)
        {
          switch (source->GetOriginalType ()->GetBaseType ())
          {
          case parser::SemanticsHandler::Bool:    typeMatch = (destType == intermediate::Bool);  break;
          case parser::SemanticsHandler::Int:     typeMatch = (destType == intermediate::Int);   break;
          case parser::SemanticsHandler::UInt:    typeMatch = (destType == intermediate::UInt);  break;
          case parser::SemanticsHandler::Float:   typeMatch = (destType == intermediate::Float); break;
          default: break;
          }
        }
        std::string sourceName (owner->GetOutputRegisterName (source));
        // If the types match just perform a simple assignment
        if (typeMatch)
        {
          PseudoAssign (destination, sourceName.c_str ());
          return;
        }
        // Types don't match: actually generate cast
        EmitFunctionCall (destination, owner->traits.TypeString (destType), sourceName.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpMakeVector (const RegisterPtr& destination,
                                                                intermediate::BasicType compType,
                                                                const std::vector<RegisterPtr>& sources)
      {
        AnnotatingVisitor::OpMakeVector (destination, compType, sources);
        std::string paramsStr;
        bool one_source (true);
        {
          RegisterPtr lastSource (sources[0]);
          for (size_t i = 1; i < sources.size (); i++)
          {
            RegisterPtr source (sources[i]);
            one_source &= (source == lastSource);
            lastSource = source;
          }
        }
        if (one_source)
        {
          paramsStr = owner->GetOutputRegisterName (sources[0]);
        }
        else
        {
          ParamHelper params (paramsStr);
          {
            SwizzleHelper swizzles (params);
            for (const RegisterPtr& source : sources)
            {
              RegisterOriginsMap::const_iterator originIt (owner->registerOrigins.find (source));
              if (originIt != owner->registerOrigins.end ())
              {
                const RegisterOriginPair& origin (originIt->second);
                swizzles.Add (owner->GetOutputRegisterName (origin.first), origin.second);
              }
              else
              {
                swizzles.Flush ();
                params.Add (owner->GetOutputRegisterName (source));
              }
            }
          }
        }
        EmitFunctionCall (destination,
                          owner->traits.FormatVector (compType, unsigned (sources.size ())),
                          paramsStr.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpVectorDot (const RegisterPtr& destination,
                                                               const RegisterPtr& source1,
                                                               const RegisterPtr& source2)
      {
        AnnotatingVisitor::OpVectorDot (destination, source1, source2);
        EmitFunctionCall (destination, "dot", source1, source2);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpVectorCross (const RegisterPtr& destination,
                                                                 const RegisterPtr& source1,
                                                                 const RegisterPtr& source2)
      {
        AnnotatingVisitor::OpVectorCross (destination, source1, source2);
        EmitFunctionCall (destination, "cross", source1, source2);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpVectorNormalize (const RegisterPtr& destination,
                                                                     const RegisterPtr& source)
      {
        AnnotatingVisitor::OpVectorNormalize (destination, source);
        EmitFunctionCall (destination, "normalize", source);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpVectorLength (const RegisterPtr& destination,
                                                                  const RegisterPtr& source)
      {
        AnnotatingVisitor::OpVectorLength (destination, source);
        EmitFunctionCall (destination, "length", source);
      }

      static format::StaticFormatter FormatMatrix ("{0}{1}x{2}");

      void SequenceCodeGenerator::CodegenVisitor::OpMakeMatrix (const RegisterPtr& destination,
                                                                intermediate::BasicType compType,
                                                                unsigned int matrixRows,
                                                                unsigned int matrixCols,
                                                                const std::vector<RegisterPtr>& sources)
      {
        AnnotatingVisitor::OpMakeMatrix (destination, compType, matrixRows, matrixCols, sources);
        std::string paramsStr;
        ParamHelper params (paramsStr);
        for (std::vector<RegisterPtr>::const_iterator source (sources.begin ());
        source != sources.end ();
          ++source)
        {
          params.Add (owner->GetOutputRegisterName (*source));
        }
        EmitFunctionCall (destination,
                          owner->traits.FormatMatrix (compType, matrixRows, matrixCols),
                          paramsStr.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpMatrixLinAlgMul (const RegisterPtr& destination,
                                                                     const RegisterPtr& source1,
                                                                     const RegisterPtr& source2)
      {
        AnnotatingVisitor::OpMatrixLinAlgMul (destination, source1, source2);
        EmitFunctionCall (destination, "mul",
                          FormatBinaryArgs.to<std::string> (owner->GetOutputRegisterName (source1),
                                                            owner->GetOutputRegisterName (source2)).c_str());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpMakeArray (const RegisterPtr& destination,
                                                               const std::vector<RegisterPtr>& sources)
      {
        AnnotatingVisitor::OpMakeArray (destination, sources);
        std::string elementsStr ("{ ");
        ParamHelper elements (elementsStr);
        for (std::vector<RegisterPtr>::const_iterator source (sources.begin ());
        source != sources.end ();
          ++source)
        {
          elements.Add (owner->GetOutputRegisterName (*source));
        }
        elementsStr.append (" }");
        std::string dummy;
        owner->GetOutputRegisterName (destination, dummy, elementsStr);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpExtractArrayElement (const RegisterPtr& destination,
                                                                         const RegisterPtr& source,
                                                                         const RegisterPtr& index)
      {
        AnnotatingVisitor::OpExtractArrayElement (destination, source, index);
        std::string sourceName (owner->GetOutputRegisterName (source));
        sourceName.append ("[");
        sourceName.append (owner->GetOutputRegisterName (index));
        sourceName.append ("]");
        EmitAssign (destination, sourceName.c_str ());
      }

      void SequenceCodeGenerator::CodegenVisitor::OpChangeArrayElement (const RegisterPtr& destination,
                                                                        const RegisterPtr& source,
                                                                        const RegisterPtr& index,
                                                                        const RegisterPtr& newValue)
      {
        AnnotatingVisitor::OpChangeArrayElement (destination, source, index, newValue);
        EmitAssign (destination, owner->GetOutputRegisterName (source).c_str ());
        std::string changeDest (owner->GetOutputRegisterName (destination));
        changeDest.append ("[");
        changeDest.append (owner->GetOutputRegisterName (index));
        changeDest.append ("]");
        EmitAssign (changeDest.c_str (), newValue);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpGetArrayLength (const RegisterPtr& destination,
                                                                    const RegisterPtr& array)
      {
        AnnotatingVisitor::OpGetArrayLength (destination, array);
        std::string sourceName (owner->GetOutputRegisterName (array));
        sourceName.append (".length");
        EmitAssign (destination, sourceName.c_str ());
      }


      void SequenceCodeGenerator::CodegenVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
                                                                            const RegisterPtr& source,
                                                                            unsigned int comp)
      {
        AnnotatingVisitor::OpExtractVectorComponent (destination, source, comp);
        std::string sourceName (owner->GetOutputRegisterName (source));
        sourceName.append (".");
        sourceName.append (vecCompStr[comp]);

        RegistersToIDMap::iterator regIt = owner->seenRegisters.find (destination);
        if (regIt != owner->seenRegisters.end ())
        {
          // Register has a name, emit assign
          EmitAssign (destination, sourceName.c_str ());
        }
        else
        {
          // Set 'name' of register to constant value...
          owner->seenRegisters[destination] = sourceName.c_str ();
        }
        owner->registerOrigins[destination] = std::make_pair (source, comp);
      }


      void SequenceCodeGenerator::CodegenVisitor::OpArith (const RegisterPtr& destination,
                                                           ArithmeticOp op,
                                                           const RegisterPtr& source1,
                                                           const RegisterPtr& source2)
      {
        AnnotatingVisitor::OpArith (destination, op, source1, source2);
        switch (op)
        {
        case Add:
          EmitBinary (destination, source1, source2, "+");
          break;
        case Sub:
          EmitBinary (destination, source1, source2, "-");
          break;
        case Mul:
          EmitBinary (destination, source1, source2, "*");
          break;
        case Div:
          EmitBinary (destination, source1, source2, "/");
          break;
        case Mod:
          EmitBinary (destination, source1, source2, "%");
          break;
        }
      }


      void SequenceCodeGenerator::CodegenVisitor::OpLogic (const RegisterPtr& destination,
                                                           LogicOp op,
                                                           const RegisterPtr& source1,
                                                           const RegisterPtr& source2)
      {
        AnnotatingVisitor::OpLogic (destination, op, source1, source2);
        switch (op)
        {
        case And:
          EmitBinary (destination, source1, source2, "&&");
          break;
        case Or:
          EmitBinary (destination, source1, source2, "||");
          break;
        }
      }


      void SequenceCodeGenerator::CodegenVisitor::OpUnary (const RegisterPtr& destination,
                                                           UnaryOp op,
                                                           const RegisterPtr& source)
      {
        AnnotatingVisitor::OpUnary (destination, op, source);
        switch (op)
        {
        case Inv:
          EmitUnary (destination, source, "~");
          break;
        case Neg:
          EmitUnary (destination, source, "-");
          break;
        case Not:
          EmitUnary (destination, source, "!");
          break;
        }
      }


      void SequenceCodeGenerator::CodegenVisitor::OpCompare (const RegisterPtr& destination,
                                                             CompareOp op,
                                                             const RegisterPtr& source1,
                                                             const RegisterPtr& source2)
      {
        AnnotatingVisitor::OpCompare (destination, op, source1, source2);
        switch (op)
        {
        case Eq:
          EmitBinary (destination, source1, source2, "==");
          break;
        case NE:
          EmitBinary (destination, source1, source2, "!=");
          break;
        case LT:
          EmitBinary (destination, source1, source2, "<");
          break;
        case LE:
          EmitBinary (destination, source1, source2, "<=");
          break;
        case GT:
          EmitBinary (destination, source1, source2, ">");
          break;
        case GE:
          EmitBinary (destination, source1, source2, ">=");
          break;
        }
      }

      static format::StaticFormatter FormatImpMapEntry (" {1} -> \"{0}\"\n");
      static format::StaticFormatter FormatExpMapEntry (" {1} <- \"{0}\"\n");

      void SequenceCodeGenerator::CodegenVisitor::OpBlock (const intermediate::SequencePtr& seq,
                                                           const Sequence::IdentifierToRegMap& identToRegID_imp,
                                                           const Sequence::IdentifierToRegMap& identToRegID_exp)
      {
        AnnotatingVisitor::BeforeOpBlock (identToRegID_imp);

        // Generate registers for 'exported' variables
        intermediate::RegisterSet writtenRegisters (seq->GetExportOuterRegs (identToRegID_exp));
        for (const RegisterPtr& writtenReg : writtenRegisters)
        {
          owner->GetOutputRegisterName (writtenReg);
        }

        SequenceIdentifiersToRegIDsNameResolver nameRes (owner, identToRegID_imp, identToRegID_exp);
        intermediate::ProgramFunction::TransferMappings emptyMappings;
        auto codegen = owner->CreateForBlock (*seq, &nameRes);
        StringsArrayPtr blockStrings (codegen->Generate ());
        if (emitEmptyBlocks || (blockStrings->Size () > 0))
        {
          target->AddString ("{");
          target->AddStrings (*blockStrings, 2);
          target->AddString ("}");
        }

        AnnotatingVisitor::AfterOpBlock (identToRegID_exp);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpBranch (const RegisterPtr& conditionReg,
                                                            const intermediate::SequenceOpPtr& seqOpIf,
                                                            const intermediate::SequenceOpPtr& seqOpElse)
      {
        AnnotatingVisitor::OpBranch (conditionReg, seqOpIf, seqOpElse);
        // Generate registers for variables 'exported' by either branch
        {
          intermediate::RegisterSet ifRegs (seqOpIf->GetWrittenRegisters ());
          for (intermediate::RegisterSet::const_iterator writtenReg = ifRegs.begin ();
          writtenReg != ifRegs.end ();
            ++writtenReg)
          {
            owner->GetOutputRegisterName (*writtenReg);
          }
        }
        {
          intermediate::RegisterSet elseRegs (seqOpElse->GetWrittenRegisters ());
          for (intermediate::RegisterSet::const_iterator writtenReg = elseRegs.begin ();
          writtenReg != elseRegs.end ();
            ++writtenReg)
          {
            owner->GetOutputRegisterName (*writtenReg);
          }
        }

        bool oldEmitEmptyBlocks = emitEmptyBlocks;
        emitEmptyBlocks = true;
        std::string ifLine ("if (");
        ifLine.append (owner->GetOutputRegisterName (conditionReg));
        ifLine.append (")");
        target->AddString (ifLine);
        seqOpIf->Visit (*this);
        target->AddString ("else");
        seqOpElse->Visit (*this);
        emitEmptyBlocks = oldEmitEmptyBlocks;
      }

      void SequenceCodeGenerator::CodegenVisitor::OpWhile (const RegisterPtr& conditionReg,
                                                           const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                                                           const intermediate::SequenceOpPtr& seqOpBody)
      {
        AnnotatingVisitor::OpWhile (conditionReg, loopedRegs, seqOpBody);
        for (size_t i = 0; i < loopedRegs.size (); i++)
        {
          // Before entering, copy original registers to their writeable counterparts
          EmitAssign (loopedRegs[i].second,
                      owner->GetOutputRegisterName (loopedRegs[i].first).c_str ());
        }
        // Generate registers for each read or written variable
        intermediate::RegisterSet writtenRegisters (seqOpBody->GetWrittenRegisters ());
        intermediate::RegisterSet readRegisters (seqOpBody->GetReadRegisters ());
        for (const RegisterPtr& reg : writtenRegisters)
        {
          owner->GetOutputRegisterName (reg, rfForceVariable);
        }
        for (const RegisterPtr& reg : readRegisters)
        {
          owner->GetOutputRegisterName (reg, rfForceVariable);
        }

        bool oldEmitEmptyBlocks = emitEmptyBlocks;
        emitEmptyBlocks = true;
        std::string whileLine ("while (");
        whileLine.append (owner->GetOutputRegisterName (conditionReg));
        whileLine.append (")");
        target->AddString (whileLine);
        seqOpBody->Visit (*this);
        emitEmptyBlocks = oldEmitEmptyBlocks;
      }

      void SequenceCodeGenerator::CodegenVisitor::OpSampleTexture (const RegisterPtr& destination,
                                                                   SampleTextureOp what,
                                                                   const RegisterPtr& sampler,
                                                                   const RegisterPtr& coord)
      {
        AnnotatingVisitor::OpSampleTexture (destination, what, sampler, coord);
        const char* functionName = SamplingFunction (what);
        S1_ASSERT(functionName, S1_ASSERT_RET_VOID);
        EmitFunctionCall (destination, functionName, sampler, coord);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
      {
        AnnotatingVisitor::OpReturn (outParamVals);
        assert (outParamVals.size () == owner->outParams.size ());
        for (size_t i = 0; i < outParamVals.size (); i++)
          EmitAssign (owner->outParams[i], outParamVals[i]);
        target->AddString ("return;");
      }

      void SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const uc::String& funcIdent,
                                                                  const std::vector<RegisterPtr>& inParams,
                                                                  const std::vector<RegisterPtr>& outParams)
      {
        AnnotatingVisitor::OpFunctionCall (funcIdent, inParams, outParams);
        std::string line;
        owner->traits.ConvertIdentifier (funcIdent).toUTF8String (line);
        line.append (" (");
        ParamHelper params (line);
        for (std::vector<RegisterPtr>::const_iterator inParam (inParams.begin ());
        inParam != inParams.end ();
          ++inParam)
        {
          params.Add (owner->GetOutputRegisterName (*inParam));
        }
        for (std::vector<RegisterPtr>::const_iterator outParam (outParams.begin ());
        outParam != outParams.end ();
          ++outParam)
        {
          params.Add (owner->GetOutputRegisterName (*outParam));
        }
        line.append (");");
        target->AddString (line);
      }

      void SequenceCodeGenerator::CodegenVisitor::OpBuiltinCall (const RegisterPtr& destination,
                                                                 intermediate::BuiltinFunction what,
                                                                 const std::vector<RegisterPtr>& inParams)
      {
        AnnotatingVisitor::OpBuiltinCall (destination, what, inParams);
        std::string exprStr;
        switch (what)
        {
        case intermediate::min:		exprStr.append ("min");		break;
        case intermediate::max:		exprStr.append ("max");		break;
        case intermediate::pow:		exprStr.append ("pow");		break;
        }
        exprStr.append (" (");
        ParamHelper params (exprStr);
        for (std::vector<RegisterPtr>::const_iterator inParam (inParams.begin ());
        inParam != inParams.end ();
          ++inParam)
        {
          params.Add (owner->GetOutputRegisterName (*inParam));
        }
        exprStr.append (")");

        std::string destName;
        if (!destination || !owner->GetOutputRegisterName (destination, destName, exprStr))
        {
          std::string line;
          if (!destName.empty ())
          {
            line.append (destName);
            line.append (" = ");
          }
          line.append (exprStr);
          line.append (";");
          target->AddString (line);
        }
      }

      //-----------------------------------------------------------------------

      SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
                                                    ImportedNameResolver* nameRes,
                                                    const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                    const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                    const std::vector<uc::String>& outParams,
                                                    const Traits& traits,
                                                    const Options& options)
        : AnnotatingSequenceCodeGenerator (seq), nameRes (nameRes), transferIn (transferIn), transferOut (transferOut), outParams (outParams),
        traits (traits), options (options)
      {
      }

      StringsArrayPtr SequenceCodeGenerator::Generate ()
      {
        strings = boost::make_shared<StringsArray> ();
        seenRegisters.clear ();

        auto visitor = CreateVisitor ();
        visitor->SetDebugCommentsEnabled (options.GetDebugAnnotationFlag ());

        BeforeSequence (*visitor);

        // 'Import' variables from parent generator
        {
          const intermediate::Sequence::RegisterImpMappings& imports = seq.GetImports ();
          for (intermediate::Sequence::RegisterImpMappings::const_iterator import = imports.begin ();
          import != imports.end ();
            ++import)
          {
            uc::String parentID = nameRes->GetImportedNameIdentifier (import->first);
            if (parentID.size () > 0)
            {
              parentID.toUTF8String (seenRegisters[import->second]);
            }
            /* else: no ID, value is undefined; leave undefined in this block, too */
          }
        }
        // Generate transfer ops
        {
          std::vector<InputTransferPair> inTransferMappings = GenerateTransfersIn ();
          for (const auto& inMapping : inTransferMappings)
          {
            std::string transferIdent;
            inMapping.second.toUTF8String (transferIdent);
            seenRegisters.emplace (inMapping.first, transferIdent.c_str ());
          }
        }
        typedef std::pair<std::string, RegisterPtr> EndAssignmentPair;
        std::vector<EndAssignmentPair> endAssignments;
        // Generate transfer ops
        {
          std::vector<OutputTransferPair> outTransferMappings = GenerateTransfersOut ();
          for (const auto& outMapping : outTransferMappings)
          {
            std::string transferIdent;
            outMapping.first.toUTF8String (transferIdent);
            RegistersToIDMap::iterator regIt = seenRegisters.find (outMapping.second);
            if (regIt == seenRegisters.end ())
              // Register has no name yet, override variable name with transfer value name
              seenRegisters.emplace (outMapping.second, transferIdent.c_str ());
            else
              // Assign at end
              endAssignments.emplace_back (transferIdent, outMapping.second);
          }
        }
        // 'Export' variables to outer scope
        {
          const intermediate::Sequence::RegisterExpMappings& exports = seq.GetExports ();
          for (intermediate::Sequence::RegisterExpMappings::const_iterator exportVar = exports.begin ();
          exportVar != exports.end ();
            ++exportVar)
          {
            uc::String parentID = nameRes->GetExportedNameIdentifier (exportVar->first);
            if (parentID.size () > 0)
            {
              std::string parentID_s;
              parentID.toUTF8String (parentID_s);
              RegistersToIDMap::iterator regIt = seenRegisters.find (exportVar->second);
              if (regIt == seenRegisters.end ())
                // Register has no name yet, override variable name with output value name
                seenRegisters[exportVar->second] = parentID_s;
              else
                // Assign at end
                endAssignments.emplace_back (parentID_s, exportVar->second);
            }
            /* else: no ID, value is undefined; no assign */
          }
        }

        // Generate code for actual operations
        seq.Visit (*visitor);

        for (const EndAssignmentPair& assignment : endAssignments)
        {
          visitor->EmitAssign (assignment.first.c_str (), assignment.second);
        }

        AfterSequence (*visitor);

        return strings;
      }

      std::string SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr,
                                                                unsigned int flags)
      {
        std::string cgName;
        GetOutputRegisterName (regPtr, cgName, std::string (), flags);
        return cgName;
      }

      bool SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr,
                                                         std::string& name,
                                                         const std::string& initializer,
                                                         unsigned int flags)
      {
        RegistersToIDMap::iterator regIt = seenRegisters.find (regPtr);
        if (regIt != seenRegisters.end ())
        {
          name = regIt->second;
          if ((flags & rfForceVariable) != 0)
          {
            // Make sure we have a variable of 'cgName' declared
            std::string slName;
            traits.ConvertIdentifier (regPtr->GetName ()).toUTF8String (slName);
            if (slName != name)
            {
              EmitDeclaration (regPtr->GetOriginalType (), slName, name);
              seenRegisters[regPtr] = slName;
              name = slName;
            }
          }
          return false;
        }

        std::string slName;
        traits.ConvertIdentifier (regPtr->GetName ()).toUTF8String (slName);
        seenRegisters[regPtr] = slName;

        EmitDeclaration (regPtr->GetOriginalType (), slName, initializer);
        name = slName;
        return true;
      }

      static format::StaticFormatter FormatDeclaration ("{0} {1}{2};");
      static format::StaticFormatter FormatDeclarationInit ("{0} {1}{2} = {3};");

      void SequenceCodeGenerator::EmitDeclaration (
        const intermediate::IntermediateGeneratorSemanticsHandler::TypePtr& type,
        const std::string& name, const std::string& initializer)
      {
        auto typeStrings = traits.TypeString (type, nullptr);
        uc::String declLine;
        if (!initializer.empty ())
        {
          FormatDeclarationInit (declLine, typeStrings.first, name.c_str(), typeStrings.second, initializer.c_str());
        }
        else
        {
          FormatDeclaration (declLine, typeStrings.first, name.c_str(), typeStrings.second);
        }
        strings->AddString (declLine);
      }

      std::unique_ptr<SequenceCodeGenerator::CodegenVisitor> SequenceCodeGenerator::CreateVisitor ()
      {
        return std::unique_ptr<CodegenVisitor> (new CodegenVisitor (this, strings));
      }
    } // namespace sl
  } // namespace codegen
} // namespace s1
