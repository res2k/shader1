#include <boost/cstdint.hpp>

#include "base/hash_UnicodeString.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <sstream>

namespace s1
{
  namespace codegen
  {
    LatexGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::SequenceIdentifiersToRegIDsNameResolver (
      SequenceCodeGenerator* owner, const Sequence::IdentifierToRegIDMap& identToRegID_imp,
      const Sequence::IdentifierToRegIDMap& identToRegID_exp)
      : owner (owner), identToRegID_imp (identToRegID_imp), identToRegID_exp (identToRegID_exp) {}

    std::string LatexGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetImportedNameIdentifier (const UnicodeString& name)
    {
      /*Sequence::IdentifierToRegIDMap::const_iterator regIt = identToRegID_imp.find (name);
      if (regIt != identToRegID_imp.end())
	return owner->GetOutputRegisterName (regIt->second);*/
      return std::string ();
    }
	
    std::string LatexGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::GetExportedNameIdentifier (const UnicodeString& name)
    {
      /*Sequence::IdentifierToRegIDMap::const_iterator regIt = identToRegID_exp.find (name);
      if (regIt != identToRegID_exp.end())
	return owner->GetOutputRegisterName (regIt->second);*/
      return std::string ();
    }
	
    //-----------------------------------------------------------------------
		      
    namespace
    {
      struct ParamHelper
      {
	const char* connector;
	std::string& str;
	bool firstParam;
	
	ParamHelper (std::string& str, const char* connector)
	 : connector (connector), str (str), firstParam (true) {}
	void Add (const std::string& s)
	{
	  if (!firstParam)
	    str.append (connector);
	  else
	    firstParam = false;
	  str.append (s);
	}
	void Reset () { str = ""; firstParam = true; }
      };
    }
    
    //-----------------------------------------------------------------------
    
    LatexGenerator::SequenceCodeGenerator::CodegenVisitor::CodegenVisitor (SequenceCodeGenerator* owner,
									const StringsArrayPtr& target)
     : owner (owner), target (target)
    {
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const RegisterID& destination,
									    const char* value)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string line ("\\sOstatement{");
      line.append (targetName);
      line.append (" &\\gets ");
      line.append (value);
      line.append ("}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const char* destination,
									    const RegisterID& value)
    {
      std::string valueName (owner->GetOutputRegisterName (value));
      std::string line ("\\sOstatement{");
      line.append (destination);
      line.append (" &\\gets ");
      line.append (valueName);
      line.append ("}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterID& destination,
										  const char* function,
										  const char* paramsStr)
    {
      std::string rside;
      rside.append (" \\sOfunccall{");
      rside.append (function);
      rside.append ("}{");
      rside.append (paramsStr);
      rside.append ("}");
      EmitAssign (destination, rside.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitBinary (const RegisterID& destination,
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
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitUnary (const RegisterID& destination,
									   const RegisterID& source,
									   const char* op)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      std::string rside (op);
      rside.append (sourceName);
      EmitAssign (destination, rside.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstBool (const RegisterID& destination,
									     bool value)
    {
      EmitAssign (destination, value ? "\\sOboolconst{true}" : "\\sOboolconst{false}");
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstInt (const RegisterID& destination,
									    int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      std::string valueStr ("\\sOconst{");
      valueStr.append (valueStrStream.str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const RegisterID& destination,
									     unsigned int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      std::string valueStr ("\\sOconst{");
      valueStr.append (valueStrStream.str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const RegisterID& destination,
									      float value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      std::string valueStr ("\\sOconst{");
      valueStr.append (valueStrStream.str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
			      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpAssign (const RegisterID& destination,
									  const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitAssign (destination, sourceName.c_str());
    }
			      
			      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpCast (const RegisterID& destination,
									BaseType destType,
									const RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      std::string valueStr ("\\sOcast{");
      valueStr.append (sourceName.c_str());
      valueStr.append ("}{");
      switch (destType)
      {
      case Bool:
	valueStr.append (typeBool);
	break;
      case Int:
	valueStr.append (typeInt);
	break;
      case UInt:
	valueStr.append (typeUInt);
	break;
      case Float:
	valueStr.append (typeFloat);
	break;
      }
      valueStr.append ("}{");
      EmitAssign (destination, valueStr.c_str());
    }
    
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVector (const RegisterID& destination,
									      BaseType compType,
									      const std::vector<RegisterID>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr, ", ");
      for (std::vector<RegisterID>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string valueStr ("\\sOmakevec{");
      valueStr.append (paramsStr.c_str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
    
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeMatrix (const RegisterID& destination,
      									      BaseType compType,
									      unsigned int matrixRows,
									      unsigned int matrixCols,
									      const std::vector<RegisterID>& sources)
    {
      std::string paramsStr;
      std::string rowStr;
      ParamHelper rows (rowStr, " & ");
      unsigned int n = 0;
      for (std::vector<RegisterID>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	std::string colStr ("\\sOmatrixcol{");
	colStr.append (owner->GetOutputRegisterName (*source));
	colStr.append ("}");
	rows.Add (colStr);
	if (((++n) % matrixCols) == 0)
	{
	  paramsStr.append ("\\sOmatrixrow{");
	  paramsStr.append (rowStr);
	  paramsStr.append ("}");
	  rows.Reset();
	}
      }
      std::string valueStr ("\\sOmakematrix{");
      valueStr.append (paramsStr.c_str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
    

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeArray (const RegisterID& destination,
									     const std::vector<RegisterID>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr, ", ");
      for (std::vector<RegisterID>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string valueStr ("\\sOmakearray{");
      valueStr.append (paramsStr.c_str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
			  
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractArrayElement (const RegisterID& destination,
										       const RegisterID& source,
										       const RegisterID& index)
    {
      std::string sourceName ("\\sOextractarray{");
      sourceName.append (owner->GetOutputRegisterName (source));
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (index));
      sourceName.append ("}");
      EmitAssign (destination, sourceName.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpChangeArrayElement (const RegisterID& destination,
										      const RegisterID& source,
										      const RegisterID& index,
										      const RegisterID& newValue)
    {
      std::string sourceName ("\\sOchangearray{");
      sourceName.append (owner->GetOutputRegisterName (source));
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (index));
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (newValue));
      sourceName.append ("}");
      EmitAssign (destination, sourceName.c_str());
    }

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpGetArrayLength (const RegisterID& destination,
										  const RegisterID& array)
    {
      std::string sourceName ("\\sOarraylen{");
      sourceName.append (owner->GetOutputRegisterName (array));
      sourceName.append ("}");
      EmitAssign (destination, sourceName.c_str());
    }
			 
				    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractVectorComponent (const RegisterID& destination,
											  const RegisterID& source,
											  unsigned int comp)
    {
      std::string sourceName ("\\sOvecextract{");
      sourceName.append (owner->GetOutputRegisterName (source));
      sourceName.append ("}{");
      std::stringstream valueStrStream;
      valueStrStream << comp;
      sourceName.append (valueStrStream.str());
      sourceName.append ("}");
      EmitAssign (destination, sourceName.c_str());
    }
    

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpArith (const RegisterID& destination,
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
    

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpLogic (const RegisterID& destination,
									 LogicOp op,
									 const RegisterID& source1,
									 const RegisterID& source2)
    {
      switch (op)
      {
      case And:
	EmitBinary (destination, source1, source2, "\\land");
	break;
      case Or:
	EmitBinary (destination, source1, source2, "\\lor");
	break;
      }
    }
    


    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnary (const RegisterID& destination,
									 UnaryOp op,
									 const RegisterID& source)
    {
      switch (op)
      {
      case Inv:
	EmitUnary (destination, source, "$sim$");
	break;
      case Neg:
	EmitUnary (destination, source, "-");
	break;
      case Not:
	EmitUnary (destination, source, "\\neg ");
	break;
      }
    }

		      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompare (const RegisterID& destination,
									   CompareOp op,
									   const RegisterID& source1,
									   const RegisterID& source2)
    {
      switch (op)
      {
      case Eq:
	EmitBinary (destination, source1, source2, "=");
	break;
      case NE:
	EmitBinary (destination, source1, source2, "\\neq");
	break;
      case LT:
	EmitBinary (destination, source1, source2, "<");
	break;
      case LE:
	EmitBinary (destination, source1, source2, "\\le");
	break;
      case GT:
	EmitBinary (destination, source1, source2, ">");
	break;
      case GE:
	EmitBinary (destination, source1, source2, "\\ge");
	break;
      }
    }
    
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpBlock (const intermediate::SequencePtr& seq,
								      const Sequence::IdentifierToRegIDMap& identToRegID_imp,
								      const Sequence::IdentifierToRegIDMap& identToRegID_exp,
								      const std::vector<RegisterID>& writtenRegisters)
    {
      #if 0
      // Generate registers for 'exported' variables
      for (std::vector<RegisterID>::const_iterator writtenReg = writtenRegisters.begin();
	   writtenReg != writtenRegisters.end();
	   ++writtenReg)
      {
	owner->GetOutputRegisterName (*writtenReg);
      }
      #endif
      
      SequenceIdentifiersToRegIDsNameResolver nameRes (owner, identToRegID_imp, identToRegID_exp);
      SequenceCodeGenerator codegen (*seq, &nameRes);
      StringsArrayPtr blockStrings (codegen.Generate());
      if (blockStrings->Size() > 0)
      {
	target->AddString ("\\sOblock{");
	target->AddStrings (*blockStrings, 2);
	target->AddString ("}");
      }
    }
		      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpBranch (const RegisterID& conditionReg,
								       const intermediate::SequenceOpPtr& seqOpIf,
								       const intermediate::SequenceOpPtr& seqOpElse)
    {
      #if 0
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
      #endif
      
      std::string ifLine ("\\sOif{");
      ifLine.append (owner->GetOutputRegisterName (conditionReg));
      ifLine.append ("}{");
      target->AddString (ifLine);
      seqOpIf->Visit (*this);
      target->AddString ("}{");
      seqOpElse->Visit (*this);
      target->AddString ("}");
    }

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpWhile (const RegisterID& conditionReg,
								      const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
								      const intermediate::SequenceOpPtr& seqOpBody)
    {
      #if 0
      for (size_t i = 0; i < loopedRegs.size(); i++)
      {
	// Before entering, copy original registers to their writeable counterparts
	EmitAssign (loopedRegs[i].second,
		    owner->GetOutputRegisterName (loopedRegs[i].first).c_str());
      }
      #endif
      
      std::string whileLine ("\\sOwhile{");
      whileLine.append (owner->GetOutputRegisterName (conditionReg));
      whileLine.append ("}{");
      target->AddString (whileLine);
      seqOpBody->Visit (*this);
      target->AddString ("}");
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpReturn (const RegisterID& retValReg)
    {
      std::string retLine ("\\sOreturn{");
      if (retValReg.IsValid())
      {
	retLine.append (owner->GetOutputRegisterName (retValReg));
      }
      retLine.append ("}");
      target->AddString (retLine);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const RegisterID& destination,
										const UnicodeString& funcIdent,
										const std::vector<RegisterID>& inParams,
										const std::vector<RegisterID>& outParams)
    {
      std::string line;
      if (destination.IsValid())
      {
	line.append (owner->GetOutputRegisterName (destination));
	line.append (" \\gets ");
      }
      std::string function;
      funcIdent.toUTF8String (function);
      line.append (" \\sOfunccall{");
      line.append (function);
      line.append ("}{");
      ParamHelper params (line, ", ");
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
      line.append ("}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpBuiltinCall (const RegisterID& destination,
									    intermediate:: BuiltinFunction what,
									    const std::vector<RegisterID>& inParams)
    {
      std::string line ("\\sOstatement{");
      if (destination.IsValid())
      {
	line.append (owner->GetOutputRegisterName (destination));
	line.append (" &\\gets \\sObuiltin");
      }
      switch (what)
      {
      case intermediate::dot:		line.append ("dot");		break;
      case intermediate::cross:		line.append ("cross");		break;
      case intermediate::normalize:	line.append ("normalize");	break;
      case intermediate::length:	line.append ("length");		break;
      case intermediate::mul:		line.append ("mul");		break;
      case intermediate::tex1D:		line.append ("texOneD");	break;
      case intermediate::tex2D:		line.append ("texTwoD");	break;
      case intermediate::tex3D:		line.append ("texThreeD");	break;
      case intermediate::texCUBE:	line.append ("texCUBE");	break;
      case intermediate::min:		line.append ("min");		break;
      case intermediate::max:		line.append ("max");		break;
      case intermediate::pow:		line.append ("pow");		break;
      }
      line.append ("{");
      ParamHelper params (line, "}{");
      for (std::vector<RegisterID>::const_iterator inParam (inParams.begin());
	   inParam != inParams.end();
	   ++inParam)
      {
	params.Add (owner->GetOutputRegisterName (*inParam));
      }
      line.append ("}");
      line.append ("}");
      target->AddString (line);
    }
    
    //-----------------------------------------------------------------------
		      
    LatexGenerator::SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
							       ImportedNameResolver* nameRes)
     : seq (seq), nameRes (nameRes)
    {
    }
    
    StringsArrayPtr LatexGenerator::SequenceCodeGenerator::Generate ()
    {
      strings = boost::make_shared<StringsArray> ();
      seenRegisters.clear();
      
      CodegenVisitor visitor (this, strings);
      
#if 0
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
#endif
      
      // Generate code for actual operations
      seq.Visit (visitor);
      
#if 0
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
#endif
      return strings;
    }
    
    std::string LatexGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterID& reg,
									      const std::string& initializer)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (reg);
      if (regIt != seenRegisters.end())
	return regIt->second;
      
      Sequence::RegisterBankPtr bankPtr;
      Sequence::RegisterPtr regPtr (seq.QueryRegisterPtrFromID (reg, bankPtr));
      
      std::string latexName;// (NameToCgIdentifier (regPtr->GetName()));
      regPtr->GetName().toUTF8String (latexName);
      std::string outStr ("\\sOreg{");
      outStr.append (LatexEscape (latexName));
      outStr.append ("}");
      seenRegisters[reg] = outStr;

      std::string typeStr (TypeString (bankPtr->GetOriginalType()));
      RegisterNameSet& nameSet = registerSets[typeStr];
      nameSet.insert (regPtr->GetName());
      
      return outStr;
    }
  } // namespace codegen
} // namespace s1
