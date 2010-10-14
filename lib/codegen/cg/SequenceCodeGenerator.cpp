#include <boost/cstdint.hpp>

#include "base/hash_UnicodeString.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <sstream>

namespace s1
{
  namespace codegen
  {
    CgGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::SequenceIdentifiersToRegIDsNameResolver (
      SequenceCodeGenerator* owner, const Sequence::IdentifierToRegIDMap& identToRegID_imp,
      const Sequence::IdentifierToRegIDMap& identToRegID_exp)
      : owner (owner), identToRegID_imp (identToRegID_imp), identToRegID_exp (identToRegID_exp) {}

    std::string CgGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetImportedNameIdentifier (const UnicodeString& name)
    {
      Sequence::IdentifierToRegIDMap::const_iterator regIt = identToRegID_imp.find (name);
      if (regIt != identToRegID_imp.end())
	return owner->GetOutputRegisterName (regIt->second);
      return std::string ();
    }
	
    std::string CgGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetExportedNameIdentifier (const UnicodeString& name)
    {
      Sequence::IdentifierToRegIDMap::const_iterator regIt = identToRegID_exp.find (name);
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
    }
    
    //-----------------------------------------------------------------------
    
    CgGenerator::SequenceCodeGenerator::CodegenVisitor::CodegenVisitor (SequenceCodeGenerator* owner,
									const StringsArrayPtr& target)
     : owner (owner), target (target)
    {
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const RegisterID& destination,
									 const char* value)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string line (targetName);
      line.append (" = ");
      line.append (value);
      line.append (";");
      target->AddString (line);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const char* destination,
									 const RegisterID& value)
    {
      std::string valueName (owner->GetOutputRegisterName (value));
      std::string line (destination);
      line.append (" = ");
      line.append (valueName);
      line.append (";");
      target->AddString (line);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterID& destination,
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitBinary (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2,
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitUnary (const RegisterID& destination,
									const RegisterID& source,
									const char* op)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      std::string rside (op);
      rside.append (sourceName);
      EmitAssign (destination, rside.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstBool (const RegisterID& destination,
									  bool value)
    {
      EmitAssign (destination, value ? "true" : "false");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstInt (const RegisterID& destination,
									 int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      EmitAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const RegisterID& destination,
									  unsigned int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      EmitAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const RegisterID& destination,
									   float value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      EmitAssign (destination, valueStrStream.str().c_str());
    }
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpAssign (const RegisterID& destination,
								       const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitAssign (destination, sourceName.c_str());
    }
			      
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCast (const RegisterID& destination,
								     BaseType destType,
								     const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      switch (destType)
      {
      case Bool:
	EmitFunctionCall (destination, cgTypeBool, sourceName.c_str());
	break;
      case Int:
	EmitFunctionCall (destination, cgTypeInt, sourceName.c_str());
	break;
      case UInt:
	EmitFunctionCall (destination, cgTypeUInt, sourceName.c_str());
	break;
      case Float:
	EmitFunctionCall (destination, cgTypeFloat, sourceName.c_str());
	break;
      }
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVector (const RegisterID& destination,
									   BaseType compType,
									   const std::vector<RegisterID>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr);
      for (std::vector<RegisterID>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string typeStr;
      switch (compType)
      {
      case Bool:
	typeStr = cgTypeBool;
	break;
      case Int:
	typeStr = cgTypeInt;
	break;
      case UInt:
	typeStr = cgTypeUInt;
	break;
      case Float:
	typeStr = cgTypeFloat;
	break;
      }
      char compNumStr[2];
      snprintf (compNumStr, sizeof (compNumStr), "%u", unsigned (sources.size()));
      typeStr.append (compNumStr);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    
    
				
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeMatrix (const RegisterID& destination,
									   BaseType compType,
									   unsigned int matrixRows,
									   unsigned int matrixCols,
									   const std::vector<RegisterID>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr);
      for (std::vector<RegisterID>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string typeStr;
      switch (compType)
      {
      case Bool:
	typeStr = cgTypeBool;
	break;
      case Int:
	typeStr = cgTypeInt;
	break;
      case UInt:
	typeStr = cgTypeUInt;
	break;
      case Float:
	typeStr = cgTypeFloat;
	break;
      }
      char compNumStr[4];
      snprintf (compNumStr, sizeof (compNumStr), "%ux%u", matrixRows, matrixCols);
      typeStr.append (compNumStr);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeArray (const RegisterID& destination,
									  const std::vector<RegisterID>& sources)
    {
      std::string elementsStr ("{ ");
      ParamHelper elements (elementsStr);
      for (std::vector<RegisterID>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	elements.Add (owner->GetOutputRegisterName (*source));
      }
      elementsStr.append (" }");
      owner->GetOutputRegisterName (destination, elementsStr);
    }
			  
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractArrayElement (const RegisterID& destination,
										    const RegisterID& source,
										    const RegisterID& index)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      sourceName.append ("[");
      sourceName.append (owner->GetOutputRegisterName (index));
      sourceName.append ("]");
      EmitAssign (destination, sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpChangeArrayElement (const RegisterID& destination,
										   const RegisterID& source,
										   const RegisterID& index,
										   const RegisterID& newValue)
    {
      EmitAssign (destination, owner->GetOutputRegisterName (source).c_str());
      std::string changeDest (owner->GetOutputRegisterName (destination));
      changeDest.append ("[");
      changeDest.append (owner->GetOutputRegisterName (index));
      changeDest.append ("]");
      EmitAssign (changeDest.c_str(), newValue);
    }

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpGetArrayLength (const RegisterID& destination,
									       const RegisterID& array)
    {
      std::string sourceName (owner->GetOutputRegisterName (array));
      sourceName.append (".length");
      EmitAssign (destination, sourceName.c_str());
    }
			 
				    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractVectorComponent (const RegisterID& destination,
										       const RegisterID& source,
										       unsigned int comp)
    {
      static const char* const compStr[] = { "x", "y", "z", "w" };
      
      std::string sourceName (owner->GetOutputRegisterName (source));
      sourceName.append (".");
      sourceName.append (compStr[comp]);
      EmitAssign (destination, sourceName.c_str());
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArith (const RegisterID& destination,
								      ArithmeticOp op,
								      const RegisterID& source1,
								      const RegisterID& source2)
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
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpLogic (const RegisterID& destination,
								      LogicOp op,
								      const RegisterID& source1,
								      const RegisterID& source2)
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


    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnary (const RegisterID& destination,
								      UnaryOp op,
								      const RegisterID& source)
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

		      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompare (const RegisterID& destination,
									CompareOp op,
									const RegisterID& source1,
									const RegisterID& source2)
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
								      const Sequence::IdentifierToRegIDMap& identToRegID_imp,
								      const Sequence::IdentifierToRegIDMap& identToRegID_exp,
								      const std::vector<RegisterID>& writtenRegisters)
    {
      // Generate registers for 'exported' variables
      for (std::vector<RegisterID>::const_iterator writtenReg = writtenRegisters.begin();
	   writtenReg != writtenRegisters.end();
	   ++writtenReg)
      {
	owner->GetOutputRegisterName (*writtenReg);
      }
      
      SequenceIdentifiersToRegIDsNameResolver nameRes (owner, identToRegID_imp, identToRegID_exp);
      SequenceCodeGenerator codegen (*seq, &nameRes,
				     intermediate::ProgramFunction::TransferMappings (),
				     intermediate::ProgramFunction::TransferMappings (),
				     owner->outParams);
      StringsArrayPtr blockStrings (codegen.Generate());
      if (blockStrings->Size() > 0)
      {
	target->AddString ("{");
	target->AddStrings (*blockStrings, 2);
	target->AddString ("}");
      }
    }
		      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBranch (const RegisterID& conditionReg,
								       const intermediate::SequenceOpPtr& seqOpIf,
								       const intermediate::SequenceOpPtr& seqOpElse)
    {
      // Generate registers for variables 'exported' by either branch
      {
	intermediate::RegisterIDSet ifRegs (seqOpIf->GetWrittenRegisters());
	for (intermediate::RegisterIDSet::const_iterator writtenReg = ifRegs.begin();
	    writtenReg != ifRegs.end();
	    ++writtenReg)
	{
	  owner->GetOutputRegisterName (*writtenReg);
	}
      }
      {
	intermediate::RegisterIDSet elseRegs (seqOpElse->GetWrittenRegisters());
	for (intermediate::RegisterIDSet::const_iterator writtenReg = elseRegs.begin();
	    writtenReg != elseRegs.end();
	    ++writtenReg)
	{
	  owner->GetOutputRegisterName (*writtenReg);
	}
      }
      
      std::string ifLine ("if (");
      ifLine.append (owner->GetOutputRegisterName (conditionReg));
      ifLine.append (")");
      target->AddString (ifLine);
      seqOpIf->Visit (*this);
      target->AddString ("else");
      seqOpElse->Visit (*this);
    }

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpWhile (const RegisterID& conditionReg,
								      const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
								      const intermediate::SequenceOpPtr& seqOpBody)
    {
      for (size_t i = 0; i < loopedRegs.size(); i++)
      {
	// Before entering, copy original registers to their writeable counterparts
	EmitAssign (loopedRegs[i].second,
		    owner->GetOutputRegisterName (loopedRegs[i].first).c_str());
      }
      
      std::string whileLine ("while (");
      whileLine.append (owner->GetOutputRegisterName (conditionReg));
      whileLine.append (")");
      target->AddString (whileLine);
      seqOpBody->Visit (*this);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpReturn (const std::vector<RegisterID>& outParamVals)
    {
      assert (outParamVals.size() == owner->outParams.size());
      for (size_t i = 0; i < outParamVals.size(); i++)
	EmitAssign (owner->outParams[i].c_str(), outParamVals[i]);
      target->AddString ("return;");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const UnicodeString& funcIdent,
									     const std::vector<RegisterID>& inParams,
									     const std::vector<RegisterID>& outParams)
    {
      std::string line;
      line.append (NameToCgIdentifier (funcIdent));
      line.append (" (");
      ParamHelper params (line);
      for (std::vector<RegisterID>::const_iterator inParam (inParams.begin());
	   inParam != inParams.end();
	   ++inParam)
      {
	params.Add (owner->GetOutputRegisterName (*inParam));
      }
      for (std::vector<RegisterID>::const_iterator outParam (outParams.begin());
	   outParam != outParams.end();
	   ++outParam)
      {
	params.Add (owner->GetOutputRegisterName (*outParam));
      }
      line.append (");");
      target->AddString (line);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBuiltinCall (const RegisterID& destination,
									    intermediate:: BuiltinFunction what,
									    const std::vector<RegisterID>& inParams)
    {
      std::string line;
      if (destination.IsValid())
      {
	line.append (owner->GetOutputRegisterName (destination));
	line.append (" = ");
      }
      switch (what)
      {
      case intermediate::dot:		line.append ("dot");		break;
      case intermediate::cross:		line.append ("cross");		break;
      case intermediate::normalize:	line.append ("normalize");	break;
      case intermediate::length:	line.append ("length");		break;
      case intermediate::mul:		line.append ("mul");		break;
      case intermediate::tex1D:		line.append ("tex1D");		break;
      case intermediate::tex2D:		line.append ("tex2D");		break;
      case intermediate::tex3D:		line.append ("tex3D");		break;
      case intermediate::texCUBE:	line.append ("texCUBE");	break;
      case intermediate::min:		line.append ("min");		break;
      case intermediate::max:		line.append ("max");		break;
      case intermediate::pow:		line.append ("pow");		break;
      }
      line.append (" (");
      ParamHelper params (line);
      for (std::vector<RegisterID>::const_iterator inParam (inParams.begin());
	   inParam != inParams.end();
	   ++inParam)
      {
	params.Add (owner->GetOutputRegisterName (*inParam));
      }
      line.append (");");
      target->AddString (line);
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
	    visitor.EmitAssign (import->second, parentID.c_str());
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
	  visitor.EmitAssign (transfer->second, transferIdent.c_str());
	}
      }
      
      // Generate code for actual operations
      seq.Visit (visitor);
      
      // Generate transfer ops
      {
	for (intermediate::ProgramFunction::TransferMappings::const_iterator transfer = transferOut.begin();
	     transfer != transferOut.end();
	     ++transfer)
	{
	  std::string transferIdent ("v2f."); // @@@ FIXME: hardcoded prefix
	  transferIdent.append (CgGenerator::NameToCgIdentifier (transfer->first));
	  visitor.EmitAssign (transferIdent.c_str(), transfer->second);
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
	    visitor.EmitAssign (parentID.c_str(), exportVar->second);
	  /* else: no ID, value is undefined; no assign */
	}
      }
      return strings;
    }
    
    std::string CgGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterID& reg,
									   const std::string& initializer)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (reg);
      if (regIt != seenRegisters.end())
	return regIt->second;
      
      Sequence::RegisterBankPtr bankPtr;
      Sequence::RegisterPtr regPtr (seq.QueryRegisterPtrFromID (reg, bankPtr));
      
      std::string cgName (NameToCgIdentifier (regPtr->GetName()));
      seenRegisters[reg] = cgName;
      
      std::string typeSuffix;
      std::string cgType (TypeToCgType (bankPtr->GetOriginalType(), typeSuffix));
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
      
      return cgName;
    }
  } // namespace codegen
} // namespace s1
