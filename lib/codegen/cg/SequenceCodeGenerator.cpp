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

#include "base/format/Formatter.h"
#include "base/format/std_string.h"

#include "intermediate/SequenceOp/SequenceOp.h"
#include "SequenceCodeGenerator.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <sstream>
#include <stdio.h>

#include "base/format/Formatter.txx"

namespace s1
{
  namespace codegen
  {
    CgGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::SequenceIdentifiersToRegIDsNameResolver (
      SequenceCodeGenerator* owner, const Sequence::IdentifierToRegMap& identToRegID_imp,
      const Sequence::IdentifierToRegMap& identToRegID_exp)
      : owner (owner), identToRegID_imp (identToRegID_imp), identToRegID_exp (identToRegID_exp) {}

    std::string CgGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetImportedNameIdentifier (const uc::String& name)
    {
      Sequence::IdentifierToRegMap::const_iterator regIt = identToRegID_imp.find (name);
      if (regIt != identToRegID_imp.end())
	return owner->GetOutputRegisterName (regIt->second);
      return std::string ();
    }
	
    std::string CgGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetExportedNameIdentifier (const uc::String& name)
    {
      Sequence::IdentifierToRegMap::const_iterator regIt = identToRegID_exp.find (name);
      if (regIt != identToRegID_exp.end())
	return owner->GetOutputRegisterName (regIt->second);
      return std::string ();
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
            Flush();
            currentName = name;
          }
          currentComps.append (vecCompStr[comp]);
        }
        // Unconditionally write out the current aggregated swizzle
        void Flush ()
        {
          if (!currentName.empty())
          {
            std::string s (currentName);
            s.append (".");
            s.append (currentComps);
            params.Add (s);
            currentName.clear();
            currentComps.clear();
          }
        }
      };
    }
    
    //-----------------------------------------------------------------------

    CgGenerator::SequenceCodeGenerator::CodegenVisitor::CodegenVisitor (SequenceCodeGenerator* owner,
									const StringsArrayPtr& target)
     : owner (owner), target (target), emitEmptyBlocks (false)
    {
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const RegisterPtr& destination,
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

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::PseudoAssign (const RegisterPtr& destination,
                                                                           const char* value)
    {
      RegistersToIDMap::iterator regIt = owner->seenRegisters.find (destination);
      if (regIt != owner->seenRegisters.end())
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const char* destination,
									 const RegisterPtr& value)
    {
      std::string valueName (owner->GetOutputRegisterName (value));
      std::string line (destination);
      line.append (" = ");
      line.append (valueName);
      line.append (";");
      target->AddString (line);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterPtr& destination,
									       const char* function,
									       const char* paramsStr)
    {
      std::string rside;
      rside.append (function);
      rside.append (" (");
      rside.append (paramsStr);
      rside.append (")");
      EmitAssign (destination, rside.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitBinary (const RegisterPtr& destination,
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
      EmitAssign (destination, rside.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitUnary (const RegisterPtr& destination,
									const RegisterPtr& source,
									const char* op)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      std::string rside (op);
      rside.append (sourceName);
      EmitAssign (destination, rside.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstBool (const RegisterPtr& destination,
									  bool value)
    {
      const char* valueStr = value ? "true" : "false";
      PseudoAssign (destination, valueStr);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstInt (const RegisterPtr& destination,
									 int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      PseudoAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const RegisterPtr& destination,
									  unsigned int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      PseudoAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const RegisterPtr& destination,
									   float value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      PseudoAssign (destination, valueStrStream.str().c_str());
    }
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpAssign (const RegisterPtr& destination,
								       const RegisterPtr& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitAssign (destination, sourceName.c_str());
    }
			      
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCast (const RegisterPtr& destination,
								     intermediate::BasicType destType,
								     const RegisterPtr& source)
    {
      // Check if the destination register type matches the source register type
      bool typeMatch (false);
      if (destination->GetOriginalType()->GetTypeClass()
          == parser::SemanticsHandler::Type::Base)
      {
        switch (destination->GetOriginalType()->GetBaseType())
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
        PseudoAssign (destination, sourceName.c_str());
        return;
      }
      // Types don't match: actually generate cast
      switch (destType)
      {
      case intermediate::Bool:
	EmitFunctionCall (destination, cgTypeBool, sourceName.c_str());
	break;
      case intermediate::Int:
	EmitFunctionCall (destination, cgTypeInt, sourceName.c_str());
	break;
      case intermediate::UInt:
	EmitFunctionCall (destination, cgTypeUInt, sourceName.c_str());
	break;
      case intermediate::Float:
	EmitFunctionCall (destination, cgTypeFloat, sourceName.c_str());
	break;
      }
    }

    static format::StaticFormatter FormatVector ("{0}{1}");

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVector (const RegisterPtr& destination,
									   intermediate::BasicType compType,
									   const std::vector<RegisterPtr>& sources)
    {
      std::string paramsStr;
      bool one_source (true);
      {
        RegisterPtr lastSource (sources[0]);
        for (size_t i = 1; i < sources.size(); i++)
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
          BOOST_FOREACH(const RegisterPtr& source, sources)
          {
            RegisterOriginsMap::const_iterator originIt (owner->registerOrigins.find (source));
            if (originIt != owner->registerOrigins.end())
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
      const char* baseStr (nullptr);
      switch (compType)
      {
      case intermediate::Bool:
	baseStr = cgTypeBool;
	break;
      case intermediate::Int:
	baseStr = cgTypeInt;
	break;
      case intermediate::UInt:
	baseStr = cgTypeUInt;
	break;
      case intermediate::Float:
	baseStr = cgTypeFloat;
	break;
      }
      std::string typeStr;
      FormatVector (typeStr, baseStr, unsigned (sources.size()));
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    
    static format::StaticFormatter FormatMatrix ("{0}{1}x{2}");
				
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeMatrix (const RegisterPtr& destination,
									   intermediate::BasicType compType,
									   unsigned int matrixRows,
									   unsigned int matrixCols,
									   const std::vector<RegisterPtr>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr);
      for (std::vector<RegisterPtr>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      const char* baseStr (nullptr);
      switch (compType)
      {
      case intermediate::Bool:
        baseStr = cgTypeBool;
        break;
      case intermediate::Int:
        baseStr = cgTypeInt;
        break;
      case intermediate::UInt:
        baseStr = cgTypeUInt;
        break;
      case intermediate::Float:
        baseStr = cgTypeFloat;
        break;
      }
      std::string typeStr;
      FormatMatrix (typeStr, baseStr, matrixRows, matrixCols);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeArray (const RegisterPtr& destination,
									  const std::vector<RegisterPtr>& sources)
    {
      std::string elementsStr ("{ ");
      ParamHelper elements (elementsStr);
      for (std::vector<RegisterPtr>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	elements.Add (owner->GetOutputRegisterName (*source));
      }
      elementsStr.append (" }");
      std::string dummy;
      owner->GetOutputRegisterName (destination, dummy, elementsStr);
    }
			  
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractArrayElement (const RegisterPtr& destination,
										    const RegisterPtr& source,
										    const RegisterPtr& index)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      sourceName.append ("[");
      sourceName.append (owner->GetOutputRegisterName (index));
      sourceName.append ("]");
      EmitAssign (destination, sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpChangeArrayElement (const RegisterPtr& destination,
										   const RegisterPtr& source,
										   const RegisterPtr& index,
										   const RegisterPtr& newValue)
    {
      EmitAssign (destination, owner->GetOutputRegisterName (source).c_str());
      std::string changeDest (owner->GetOutputRegisterName (destination));
      changeDest.append ("[");
      changeDest.append (owner->GetOutputRegisterName (index));
      changeDest.append ("]");
      EmitAssign (changeDest.c_str(), newValue);
    }

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpGetArrayLength (const RegisterPtr& destination,
									       const RegisterPtr& array)
    {
      std::string sourceName (owner->GetOutputRegisterName (array));
      sourceName.append (".length");
      EmitAssign (destination, sourceName.c_str());
    }
			 
				    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
										       const RegisterPtr& source,
										       unsigned int comp)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      sourceName.append (".");
      sourceName.append (vecCompStr[comp]);
      
      RegistersToIDMap::iterator regIt = owner->seenRegisters.find (destination);
      if (regIt != owner->seenRegisters.end())
      {
	// Register has a name, emit assign
	EmitAssign (destination, sourceName.c_str());
      }
      else
      {
	// Set 'name' of register to constant value...
	owner->seenRegisters[destination] = sourceName.c_str();
      }
      owner->registerOrigins[destination] = std::make_pair (source, comp);
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArith (const RegisterPtr& destination,
								      ArithmeticOp op,
								      const RegisterPtr& source1,
								      const RegisterPtr& source2)
    {
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
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpLogic (const RegisterPtr& destination,
								      LogicOp op,
								      const RegisterPtr& source1,
								      const RegisterPtr& source2)
    {
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


    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnary (const RegisterPtr& destination,
								      UnaryOp op,
								      const RegisterPtr& source)
    {
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

		      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompare (const RegisterPtr& destination,
									CompareOp op,
									const RegisterPtr& source1,
									const RegisterPtr& source2)
    {
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
	EmitBinary (destination, source1, source2, ">=");
	break;
      case GE:
	EmitBinary (destination, source1, source2, ">");
	break;
      }
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBlock (const intermediate::SequencePtr& seq,
								      const Sequence::IdentifierToRegMap& identToRegID_imp,
								      const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      // Generate registers for 'exported' variables
      intermediate::RegisterSet writtenRegisters (seq->GetExportOuterRegs (identToRegID_exp));
      BOOST_FOREACH(const RegisterPtr& writtenReg, writtenRegisters)
      {
	owner->GetOutputRegisterName (writtenReg);
      }
      
      SequenceIdentifiersToRegIDsNameResolver nameRes (owner, identToRegID_imp, identToRegID_exp);
      intermediate::ProgramFunction::TransferMappings emptyMappings;
      SequenceCodeGenerator codegen (*seq, &nameRes,
				     emptyMappings, emptyMappings,
				     owner->outParams);
      StringsArrayPtr blockStrings (codegen.Generate());
      if (emitEmptyBlocks || (blockStrings->Size() > 0))
      {
	target->AddString ("{");
	target->AddStrings (*blockStrings, 2);
	target->AddString ("}");
      }
    }
		      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBranch (const RegisterPtr& conditionReg,
								       const intermediate::SequenceOpPtr& seqOpIf,
								       const intermediate::SequenceOpPtr& seqOpElse)
    {
      // Generate registers for variables 'exported' by either branch
      {
	intermediate::RegisterSet ifRegs (seqOpIf->GetWrittenRegisters());
	for (intermediate::RegisterSet::const_iterator writtenReg = ifRegs.begin();
	    writtenReg != ifRegs.end();
	    ++writtenReg)
	{
	  owner->GetOutputRegisterName (*writtenReg);
	}
      }
      {
	intermediate::RegisterSet elseRegs (seqOpElse->GetWrittenRegisters());
	for (intermediate::RegisterSet::const_iterator writtenReg = elseRegs.begin();
	    writtenReg != elseRegs.end();
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

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpWhile (const RegisterPtr& conditionReg,
								      const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
								      const intermediate::SequenceOpPtr& seqOpBody)
    {
      for (size_t i = 0; i < loopedRegs.size(); i++)
      {
	// Before entering, copy original registers to their writeable counterparts
	EmitAssign (loopedRegs[i].second,
		    owner->GetOutputRegisterName (loopedRegs[i].first).c_str());
      }
      // Generate registers for each read or written variable
      intermediate::RegisterSet writtenRegisters (seqOpBody->GetWrittenRegisters());
      intermediate::RegisterSet readRegisters (seqOpBody->GetReadRegisters());
      BOOST_FOREACH(const RegisterPtr& reg, writtenRegisters)
      {
	owner->GetOutputRegisterName (reg);
      }
      BOOST_FOREACH(const RegisterPtr& reg, readRegisters)
      {
	owner->GetOutputRegisterName (reg);
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      assert (outParamVals.size() == owner->outParams.size());
      for (size_t i = 0; i < outParamVals.size(); i++)
	EmitAssign (owner->outParams[i].c_str(), outParamVals[i]);
      target->AddString ("return;");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const uc::String& funcIdent,
									     const std::vector<RegisterPtr>& inParams,
									     const std::vector<RegisterPtr>& outParams)
    {
      std::string line;
      line.append (NameToCgIdentifier (funcIdent));
      line.append (" (");
      ParamHelper params (line);
      for (std::vector<RegisterPtr>::const_iterator inParam (inParams.begin());
	   inParam != inParams.end();
	   ++inParam)
      {
	params.Add (owner->GetOutputRegisterName (*inParam));
      }
      for (std::vector<RegisterPtr>::const_iterator outParam (outParams.begin());
	   outParam != outParams.end();
	   ++outParam)
      {
	params.Add (owner->GetOutputRegisterName (*outParam));
      }
      line.append (");");
      target->AddString (line);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBuiltinCall (const RegisterPtr& destination,
									    intermediate:: BuiltinFunction what,
									    const std::vector<RegisterPtr>& inParams)
    {
      std::string exprStr;
      switch (what)
      {
      case intermediate::dot:		exprStr.append ("dot");		break;
      case intermediate::cross:		exprStr.append ("cross");	break;
      case intermediate::normalize:	exprStr.append ("normalize");	break;
      case intermediate::length:	exprStr.append ("length");	break;
      case intermediate::mul:		exprStr.append ("mul");		break;
      case intermediate::tex1D:		exprStr.append ("tex1D");	break;
      case intermediate::tex2D:		exprStr.append ("tex2D");	break;
      case intermediate::tex3D:		exprStr.append ("tex3D");	break;
      case intermediate::texCUBE:	exprStr.append ("texCUBE");	break;
      case intermediate::min:		exprStr.append ("min");		break;
      case intermediate::max:		exprStr.append ("max");		break;
      case intermediate::pow:		exprStr.append ("pow");		break;
      }
      exprStr.append (" (");
      ParamHelper params (exprStr);
      for (std::vector<RegisterPtr>::const_iterator inParam (inParams.begin());
	   inParam != inParams.end();
	   ++inParam)
      {
	params.Add (owner->GetOutputRegisterName (*inParam));
      }
      exprStr.append (")");

      std::string destName;
      if (!destination || !owner->GetOutputRegisterName (destination, destName, exprStr))
      {
	std::string line;
	if (!destName.empty())
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
		      
    CgGenerator::SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
							       ImportedNameResolver* nameRes,
							       const intermediate::ProgramFunction::TransferMappings& transferIn,
							       const intermediate::ProgramFunction::TransferMappings& transferOut,
							       const std::vector<std::string>& outParams)
     : seq (seq), nameRes (nameRes), transferIn (transferIn), transferOut (transferOut), outParams (outParams)
    {
    }
    
    StringsArrayPtr CgGenerator::SequenceCodeGenerator::Generate ()
    {
      strings = boost::make_shared<StringsArray> ();
      seenRegisters.clear();
      
      CodegenVisitor visitor (this, strings);
      
      // 'Import' variables from parent generator
      {
	const intermediate::Sequence::RegisterImpMappings& imports = seq.GetImports();
	for (intermediate::Sequence::RegisterImpMappings::const_iterator import = imports.begin();
	      import != imports.end();
	      ++import)
	{
	  std::string parentID = nameRes->GetImportedNameIdentifier (import->first);
	  if (parentID.size() > 0)
	  {
	    seenRegisters[import->second] = parentID.c_str();
	  }
	  /* else: no ID, value is undefined; leave undefined in this block, too */
	}
      }
      // Generate transfer ops
      {
	for (intermediate::ProgramFunction::TransferMappings::const_iterator transfer = transferIn.begin();
	     transfer != transferIn.end();
	     ++transfer)
	{
	  std::string transferIdent ("v2f."); // @@@ FIXME: hardcoded prefix
	  transferIdent.append (CgGenerator::NameToCgIdentifier (transfer->first));
	  seenRegisters[transfer->second] = transferIdent.c_str();
	}
      }
      typedef std::pair<std::string, RegisterPtr> EndAssignmentPair;
      std::vector<EndAssignmentPair> endAssignments;
      // Generate transfer ops
      {
	for (intermediate::ProgramFunction::TransferMappings::const_iterator transfer = transferOut.begin();
	     transfer != transferOut.end();
	     ++transfer)
	{
	  std::string transferIdent ("v2f."); // @@@ FIXME: hardcoded prefix
	  transferIdent.append (CgGenerator::NameToCgIdentifier (transfer->first));
	  RegistersToIDMap::iterator regIt = seenRegisters.find (transfer->second);
	  if (regIt == seenRegisters.end())
	    // Register has no name yet, override variable name with transfer value name
	    seenRegisters[transfer->second] = transferIdent.c_str();
	  else
	    // Assign at end
	    endAssignments.push_back (std::make_pair (transferIdent, transfer->second));
	}
      }
      // 'Export' variables to outer scope
      {
	const intermediate::Sequence::RegisterExpMappings& exports = seq.GetExports();
	for (intermediate::Sequence::RegisterExpMappings::const_iterator exportVar = exports.begin();
	      exportVar != exports.end();
	      ++exportVar)
	{
	  std::string parentID = nameRes->GetExportedNameIdentifier (exportVar->first);
	  if (parentID.size() > 0)
	  {
	    RegistersToIDMap::iterator regIt = seenRegisters.find (exportVar->second);
	    if (regIt == seenRegisters.end())
	      // Register has no name yet, override variable name with output value name
	      seenRegisters[exportVar->second] = parentID.c_str();
	    else
	      // Assign at end
	      endAssignments.push_back (std::make_pair (parentID, exportVar->second));
	  }
	  /* else: no ID, value is undefined; no assign */
	}
      }
      
      // Generate code for actual operations
      seq.Visit (visitor);
      
      BOOST_FOREACH(const EndAssignmentPair& assignment, endAssignments)
      {
	visitor.EmitAssign (assignment.first.c_str(), assignment.second);
      }
      
      return strings;
    }
    
    std::string CgGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr)
    {
      std::string cgName;
      GetOutputRegisterName (regPtr, cgName, std::string ());
      return cgName;
    }

    bool CgGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr,
								    std::string& name,
								    const std::string& initializer)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (regPtr);
      if (regIt != seenRegisters.end())
      {
	name = regIt->second;
	return false;
      }
      
      std::string cgName (NameToCgIdentifier (regPtr->GetName()));
      seenRegisters[regPtr] = cgName;
      
      std::string typeSuffix;
      std::string cgType (TypeToCgType (regPtr->GetOriginalType(), typeSuffix));
      std::string declLine (cgType);
      declLine.append (" ");
      declLine.append (cgName);
      declLine.append (typeSuffix);
      if (!initializer.empty())
      {
	declLine.append (" = ");
	declLine.append (initializer);
      }
      declLine.append (";");
      strings->AddString (declLine);
      
      name = cgName;
      return true;
    }
  } // namespace codegen
} // namespace s1
