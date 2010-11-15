#include <boost/cstdint.hpp>

#include "base/hash_UnicodeString.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <sstream>

namespace s1
{
  namespace codegen
  {
    LatexGenerator::SequenceCodeGenerator::SequenceIdentifiersToRegIDsNameResolver::SequenceIdentifiersToRegIDsNameResolver (
      SequenceCodeGenerator* owner, const Sequence::IdentifierToRegMap& identToReg_imp,
      const Sequence::IdentifierToRegMap& identToReg_exp)
      : owner (owner), identToRegID_imp (identToReg_imp), identToRegID_exp (identToReg_exp) {}

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
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const RegisterPtr& destination,
									    const char* value)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string line ("\\sOstmt{\\sOassign{");
      line.append (targetName);
      line.append ("}{");
      line.append (value);
      line.append ("}}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const char* destination,
									    const RegisterPtr& value)
    {
      std::string valueName (owner->GetOutputRegisterName (value));
      std::string line ("\\sOstmt{\\sOassign{");
      line.append (destination);
      line.append ("}{");
      line.append (valueName);
      line.append ("}}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const RegisterPtr& destination,
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
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitBinary (const RegisterPtr& destination,
									    const RegisterPtr& source1,
									    const RegisterPtr& source2,
									    const char* op)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string source1Name (owner->GetOutputRegisterName (source1));
      std::string source2Name (owner->GetOutputRegisterName (source2));
      std::string line ("\\sOstmt{\\sO");
      line.append (op);
      line.append ("{");
      line.append (targetName);
      line.append ("}{");
      line.append (source1Name);
      line.append ("}{");
      line.append (source2Name);
      line.append ("}}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::EmitUnary (const RegisterPtr& destination,
									   const RegisterPtr& source,
									   const char* op)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      std::string rside (op);
      rside.append (sourceName);
      EmitAssign (destination, rside.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstBool (const RegisterPtr& destination,
									     bool value)
    {
      EmitAssign (destination, value ? "\\sOboolconst{true}" : "\\sOboolconst{false}");
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstInt (const RegisterPtr& destination,
									    int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      std::string valueStr ("\\sOconst{");
      valueStr.append (valueStrStream.str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const RegisterPtr& destination,
									     unsigned int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      std::string valueStr ("\\sOconst{");
      valueStr.append (valueStrStream.str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const RegisterPtr& destination,
									      float value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      std::string valueStr ("\\sOconst{");
      valueStr.append (valueStrStream.str());
      valueStr.append ("}");
      EmitAssign (destination, valueStr.c_str());
    }
			      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpAssign (const RegisterPtr& destination,
									  const RegisterPtr& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitAssign (destination, sourceName.c_str());
    }
			      
			      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpCast (const RegisterPtr& destination,
									intermediate::BasicType destType,
									const RegisterPtr& source)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string sourceName (owner->GetOutputRegisterName (source));
      std::string valueStr ("\\sOstmt{\\sOcast{");
      switch (destType)
      {
      case intermediate::Bool:
	valueStr.append (typeBool);
	break;
      case intermediate::Int:
	valueStr.append (typeInt);
	break;
      case intermediate::UInt:
	valueStr.append (typeUInt);
	break;
      case intermediate::Float:
	valueStr.append (typeFloat);
	break;
      }
      valueStr.append ("}{");
      valueStr.append (targetName.c_str());
      valueStr.append ("}{");
      valueStr.append (sourceName.c_str());
      valueStr.append ("}}");
      target->AddString (valueStr);
    }
    
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeVector (const RegisterPtr& destination,
									      intermediate::BasicType compType,
									      const std::vector<RegisterPtr>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr, " \\sOcomma ");
      for (std::vector<RegisterPtr>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string valueStr ("\\sOstmt{\\sOmakevec{");
      valueStr.append (targetName.c_str());
      valueStr.append ("}{");
      valueStr.append (paramsStr.c_str());
      valueStr.append ("}}");
      target->AddString (valueStr);
    }
    
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeMatrix (const RegisterPtr& destination,
      									      intermediate::BasicType compType,
									      unsigned int matrixRows,
									      unsigned int matrixCols,
									      const std::vector<RegisterPtr>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr, " \\sOcomma ");
      for (std::vector<RegisterPtr>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string valueStr ("\\sOstmt{\\sOmakematrix{");
      valueStr.append (targetName.c_str());
      valueStr.append ("}{");
      valueStr.append (paramsStr.c_str());
      valueStr.append ("}}");
      target->AddString (valueStr);
    }
    

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpMakeArray (const RegisterPtr& destination,
									     const std::vector<RegisterPtr>& sources)
    {
      std::string paramsStr;
      ParamHelper params (paramsStr, " \\sOcomma ");
      for (std::vector<RegisterPtr>::const_iterator source (sources.begin());
	   source != sources.end();
	   ++source)
      {
	params.Add (owner->GetOutputRegisterName (*source));
      }
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string valueStr ("\\sOstmt{\\sOmakearray{");
      valueStr.append (targetName.c_str());
      valueStr.append ("}{");
      valueStr.append (paramsStr.c_str());
      valueStr.append ("}}");
      target->AddString (valueStr);
    }
			  
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractArrayElement (const RegisterPtr& destination,
										       const RegisterPtr& source,
										       const RegisterPtr& index)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string sourceName ("\\sOstmt{\\sOgetelem{");
      sourceName.append (targetName.c_str());
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (source));
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (index));
      sourceName.append ("}}");
      target->AddString (sourceName);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpChangeArrayElement (const RegisterPtr& destination,
										      const RegisterPtr& source,
										      const RegisterPtr& index,
										      const RegisterPtr& newValue)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string sourceName ("\\sOstmt{\\sOsetelem{");
      sourceName.append (targetName.c_str());
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (source));
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (index));
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (newValue));
      sourceName.append ("}}");
      target->AddString (sourceName);
    }

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpGetArrayLength (const RegisterPtr& destination,
										  const RegisterPtr& array)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string sourceName ("\\sOstmt{\\sOarraylen{");
      sourceName.append (targetName.c_str());
      sourceName.append ("}{");
      sourceName.append (owner->GetOutputRegisterName (array));
      sourceName.append ("}}");
      target->AddString (sourceName);
    }
			 
				    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
											  const RegisterPtr& source,
											  unsigned int comp)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string valueStr ("\\sOstmt{\\sOvecextract{");
      valueStr.append (targetName.c_str());
      valueStr.append ("}{");
      valueStr.append (owner->GetOutputRegisterName (source));
      valueStr.append ("}{");
      std::stringstream valueStrStream;
      valueStrStream << comp;
      valueStr.append (valueStrStream.str());
      valueStr.append ("}}");
      target->AddString (valueStr);
    }
    

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpArith (const RegisterPtr& destination,
									 ArithmeticOp op,
									 const RegisterPtr& source1,
									 const RegisterPtr& source2)
    {
      switch (op)
      {
      case Add:
	EmitBinary (destination, source1, source2, "add");
	break;
      case Sub:
	EmitBinary (destination, source1, source2, "sub");
	break;
      case Mul:
	EmitBinary (destination, source1, source2, "mul");
	break;
      case Div:
	EmitBinary (destination, source1, source2, "div");
	break;
      case Mod:
	EmitBinary (destination, source1, source2, "mod");
	break;
      }
    }
    

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpLogic (const RegisterPtr& destination,
									 LogicOp op,
									 const RegisterPtr& source1,
									 const RegisterPtr& source2)
    {
      switch (op)
      {
      case And:
	EmitBinary (destination, source1, source2, "and");
	break;
      case Or:
	EmitBinary (destination, source1, source2, "or");
	break;
      }
    }
    


    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpUnary (const RegisterPtr& destination,
									 UnaryOp op,
									 const RegisterPtr& source)
    {
      const char* opStr;
      switch (op)
      {
      case Inv:
	opStr = "inv";
	break;
      case Neg:
	opStr = "neg";
	break;
      case Not:
	opStr = "not";
	break;
      }
      
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string valueStr ("\\sOstmt{\\sO");
      valueStr.append (opStr);
      valueStr.append ("{");
      valueStr.append (targetName.c_str());
      valueStr.append ("}{");
      valueStr.append (owner->GetOutputRegisterName (source));
      valueStr.append ("}}");
      target->AddString (valueStr);
    }

		      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompare (const RegisterPtr& destination,
									   CompareOp op,
									   const RegisterPtr& source1,
									   const RegisterPtr& source2)
    {
      switch (op)
      {
      case Eq:
	EmitBinary (destination, source1, source2, "cmpeq");
	break;
      case NE:
	EmitBinary (destination, source1, source2, "cmpne");
	break;
      case LT:
	EmitBinary (destination, source1, source2, "cmplt");
	break;
      case LE:
	EmitBinary (destination, source1, source2, "cmple");
	break;
      case GT:
	EmitBinary (destination, source1, source2, "cmpgt");
	break;
      case GE:
	EmitBinary (destination, source1, source2, "cmpge");
	break;
      }
    }
    
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpBlock (const intermediate::SequencePtr& seq,
								      const Sequence::IdentifierToRegMap& identToRegID_imp,
								      const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      #if 0
      // Generate registers for 'exported' variables
      for (std::vector<RegisterPtr>::const_iterator writtenReg = writtenRegisters.begin();
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
	target->AddString ("\\sOnestseq{");
	const Sequence::RegisterImpMappings& seqImports = seq->GetImports();
	for (Sequence::RegisterImpMappings::const_iterator imp = seqImports.begin();
	     imp != seqImports.end();
	     ++imp)
	{
	  Sequence::IdentifierToRegMap::const_iterator impName = identToRegID_imp.find (imp->first);
	  if (impName == identToRegID_imp.end()) continue;
	  
	  std::string nestStr ("\\sOnestmapIn{");
	  nestStr.append (owner->GetOutputRegisterName (impName->second));
	  nestStr.append ("}{");
	  nestStr.append (codegen.GetOutputRegisterName (imp->second));
	  nestStr.append ("}");
	  target->AddString (nestStr);
	}
	target->AddStrings (*blockStrings, 2);
	const Sequence::RegisterExpMappings& seqExports = seq->GetExports();
	for (Sequence::RegisterExpMappings::const_iterator exp = seqExports.begin();
	     exp != seqExports.end();
	     ++exp)
	{
	  Sequence::IdentifierToRegMap::const_iterator expName = identToRegID_exp.find (exp->first);
	  if (expName == identToRegID_exp.end()) continue;
	  
	  std::string nestStr ("\\sOnestmapOut{");
	  nestStr.append (owner->GetOutputRegisterName (expName->second));
	  nestStr.append ("}{");
	  nestStr.append (codegen.GetOutputRegisterName (exp->second));
	  nestStr.append ("}");
	  target->AddString (nestStr);
	}
	target->AddString ("}");
      }
    }
		      
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpBranch (const RegisterPtr& conditionReg,
									  const intermediate::SequenceOpPtr& seqOpIf,
									  const intermediate::SequenceOpPtr& seqOpElse)
    {
      #if 0
      // Generate registers for variables 'exported' by either branch
      {
	intermediate::RegisterPtrSet ifRegs (seqOpIf->GetWrittenRegisters());
	for (intermediate::RegisterPtrSet::const_iterator writtenReg = ifRegs.begin();
	    writtenReg != ifRegs.end();
	    ++writtenReg)
	{
	  owner->GetOutputRegisterName (*writtenReg);
	}
      }
      {
	intermediate::RegisterPtrSet elseRegs (seqOpElse->GetWrittenRegisters());
	for (intermediate::RegisterPtrSet::const_iterator writtenReg = elseRegs.begin();
	    writtenReg != elseRegs.end();
	    ++writtenReg)
	{
	  owner->GetOutputRegisterName (*writtenReg);
	}
      }
      #endif
      
      std::string ifLine ("\\sObranch{");
      ifLine.append (owner->GetOutputRegisterName (conditionReg));
      ifLine.append ("}{");
      target->AddString (ifLine);
      seqOpIf->Visit (*this);
      target->AddString ("}{");
      seqOpElse->Visit (*this);
      target->AddString ("}");
    }

    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpWhile (const RegisterPtr& conditionReg,
								      const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
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
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      target->AddString ("\\sOreturn{}");
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const UnicodeString& funcIdent,
										const std::vector<RegisterPtr>& inParams,
										const std::vector<RegisterPtr>& outParams)
    {
      std::string line;
      std::string function;
      funcIdent.toUTF8String (function);
      line.append (" \\sOfunccall{");
      line.append (function);
      line.append ("}{");
      ParamHelper params (line, ", ");
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
      line.append ("}");
      target->AddString (line);
    }
    
    void LatexGenerator::SequenceCodeGenerator::CodegenVisitor::OpBuiltinCall (const RegisterPtr& destination,
									    intermediate:: BuiltinFunction what,
									    const std::vector<RegisterPtr>& inParams)
    {
      std::string line ("\\sOstmt{\\sObuiltin");
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
      if (destination)
      {
	line.append (owner->GetOutputRegisterName (destination));
	line.append ("}{");
      }
      ParamHelper params (line, "}{");
      for (std::vector<RegisterPtr>::const_iterator inParam (inParams.begin());
	   inParam != inParams.end();
	   ++inParam)
      {
	params.Add (owner->GetOutputRegisterName (*inParam));
      }
      line.append ("}}");
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
    
    std::string LatexGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr,
									      const std::string& initializer)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (regPtr);
      if (regIt != seenRegisters.end())
	return regIt->second;
      
      std::string latexName;// (NameToCgIdentifier (regPtr->GetName()));
      regPtr->GetName().toUTF8String (latexName);
      std::string outStr ("\\sOreg{");
      outStr.append (LatexEscape (latexName));
      outStr.append ("}");
      seenRegisters[regPtr] = outStr;

      std::string typeStr (TypeString (regPtr->GetOriginalType()));
      RegisterNameSet& nameSet = registerSets[typeStr];
      nameSet.insert (regPtr->GetName());
      
      return outStr;
    }
  } // namespace codegen
} // namespace s1
