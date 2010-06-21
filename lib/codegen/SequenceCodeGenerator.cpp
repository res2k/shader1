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
			      
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToBool (const RegisterID& destination,
									   const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "bool", sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToInt (const RegisterID& destination,
									  const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "int", sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToUInt (const RegisterID& destination,
									   const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "unsigned", sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToFloat (const RegisterID& destination,
									    const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "float", sourceName.c_str());
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVectorBool (const RegisterID& destination,
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
      std::string typeStr ("bool");
      char compNumStr[2];
      snprintf (compNumStr, sizeof (compNumStr), "%u", unsigned (sources.size()));
      typeStr.append (compNumStr);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVectorInt (const RegisterID& destination,
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
      std::string typeStr ("int");
      char compNumStr[2];
      snprintf (compNumStr, sizeof (compNumStr), "%u", unsigned (sources.size()));
      typeStr.append (compNumStr);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVectorUInt (const RegisterID& destination,
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
      std::string typeStr ("unsigned");
      char compNumStr[2];
      snprintf (compNumStr, sizeof (compNumStr), "%u", unsigned (sources.size()));
      typeStr.append (compNumStr);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVectorFloat (const RegisterID& destination,
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
      std::string typeStr ("float");
      char compNumStr[2];
      snprintf (compNumStr, sizeof (compNumStr), "%u", unsigned (sources.size()));
      typeStr.append (compNumStr);
      EmitFunctionCall (destination, typeStr.c_str(), paramsStr.c_str());
    }
    
				
				
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithAdd (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "+");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithSub (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "-");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithMul (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "*");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithDiv (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "/");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithMod (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "%");
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpLogicAnd (const RegisterID& destination,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "&&");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpLogicOr (const RegisterID& destination,
									const RegisterID& source1,
									const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "||");
    }


    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnaryInv (const RegisterID& destination,
									 const RegisterID& source)
    {
      EmitUnary (destination, source, "~");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnaryNeg (const RegisterID& destination,
									 const RegisterID& source)
    {
      EmitUnary (destination, source, "-");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnaryNot (const RegisterID& destination,
									 const RegisterID& source)
    {
      EmitUnary (destination, source, "!");
    }
    

		      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareEq (const RegisterID& destination,
									  const RegisterID& source1,
									  const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "==");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareNE (const RegisterID& destination,
									  const RegisterID& source1,
									  const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "!=");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareLT (const RegisterID& destination,
									  const RegisterID& source1,
									  const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "<");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareLE (const RegisterID& destination,
									  const RegisterID& source1,
									  const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "<=");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareGT (const RegisterID& destination,
									  const RegisterID& source1,
									  const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, ">");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareGE (const RegisterID& destination,
									  const RegisterID& source1,
									  const RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, ">=");
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
      SequenceCodeGenerator codegen (*seq, &nameRes);
      StringsArrayPtr blockStrings (codegen.Generate());
      target->AddString ("{");
      target->AddStrings (*blockStrings, 2);
      target->AddString ("}");
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpReturn (const RegisterID& retValReg)
    {
      std::string retLine ("return");
      if (retValReg.IsValid())
      {
	retLine.append (" ");
	retLine.append (owner->GetOutputRegisterName (retValReg));
      }
      retLine.append (";");
      target->AddString (retLine);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const RegisterID& destination,
									     const UnicodeString& funcIdent,
									     const std::vector<RegisterID>& inParams,
									     const std::vector<RegisterID>& outParams)
    {
      std::string line;
      if (destination.IsValid())
      {
	line.append (owner->GetOutputRegisterName (destination));
	line.append (" = ");
      }
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
    
    //-----------------------------------------------------------------------
		      
    CgGenerator::SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
							       ImportedNameResolver* nameRes)
     : seq (seq), nameRes (nameRes)
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
      
      // Generate code for actual operations
      seq.Visit (visitor);
      
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
									   bool autoAllocate)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (reg);
      if (regIt != seenRegisters.end())
	return regIt->second;
      
      if (!autoAllocate) return std::string ();
      
      Sequence::RegisterBankPtr bankPtr;
      Sequence::RegisterPtr regPtr (seq.QueryRegisterPtrFromID (reg, bankPtr));
      
      std::string cgName (NameToCgIdentifier (regPtr->GetName()));
      seenRegisters[reg] = cgName;
      
      std::string cgType (TypeToCgType (bankPtr->GetOriginalType()));
      std::string declLine (cgType);
      declLine.append (" ");
      declLine.append (cgName);
      declLine.append (";");
      strings->AddString (declLine);
      
      return cgName;
    }
  } // namespace codegen
} // namespace s1
