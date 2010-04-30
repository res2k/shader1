#include <boost/cstdint.hpp>

#include "SequenceCodeGenerator.h"

#include <boost/make_shared.hpp>
#include <unicode/schriter.h>
#include <sstream>

namespace s1
{
  namespace codegen
  {
    CgGenerator::SequenceCodeGenerator::CodegenVisitor::CodegenVisitor (SequenceCodeGenerator* owner,
									const StringsArrayPtr& target)
     : owner (owner), target (target)
    {
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitAssign (const Sequence::RegisterID& destination,
									 const char* value)
    {
      std::string targetName (owner->GetOutputRegisterName (destination));
      std::string line (targetName);
      line.append (" = ");
      line.append (value);
      line.append (";");
      target->AddString (line);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitFunctionCall (const Sequence::RegisterID& destination,
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::EmitBinary (const Sequence::RegisterID& destination,
									 const Sequence::RegisterID& source1,
									 const Sequence::RegisterID& source2,
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstBool (const Sequence::RegisterID& destination,
									  bool value)
    {
      EmitAssign (destination, value ? "true" : "false");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstInt (const Sequence::RegisterID& destination,
									 int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      EmitAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const Sequence::RegisterID& destination,
									  unsigned int value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      EmitAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const Sequence::RegisterID& destination,
									   float value)
    {
      std::stringstream valueStrStream;
      valueStrStream << value;
      EmitAssign (destination, valueStrStream.str().c_str());
    }
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpAssign (const Sequence::RegisterID& destination,
								       const Sequence::RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitAssign (destination, sourceName.c_str());
    }
			      
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToBool (const Sequence::RegisterID& destination,
									   const Sequence::RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "bool", sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToInt (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "int", sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToUInt (const Sequence::RegisterID& destination,
									   const Sequence::RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "unsigned", sourceName.c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCastToFloat (const Sequence::RegisterID& destination,
									    const Sequence::RegisterID& source)
    {
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitFunctionCall (destination, "float", sourceName.c_str());
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithAdd (const Sequence::RegisterID& destination,
									 const Sequence::RegisterID& source1,
									 const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "+");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithSub (const Sequence::RegisterID& destination,
									 const Sequence::RegisterID& source1,
									 const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "-");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithMul (const Sequence::RegisterID& destination,
									 const Sequence::RegisterID& source1,
									 const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "*");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithDiv (const Sequence::RegisterID& destination,
									 const Sequence::RegisterID& source1,
									 const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "/");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpArithMod (const Sequence::RegisterID& destination,
									 const Sequence::RegisterID& source1,
									 const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "%");
    }
    

    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareEq (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source1,
									  const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "==");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareNE (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source1,
									  const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "!=");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareLT (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source1,
									  const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "<");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareLE (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source1,
									  const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, "<=");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareGT (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source1,
									  const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, ">");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCompareGE (const Sequence::RegisterID& destination,
									  const Sequence::RegisterID& source1,
									  const Sequence::RegisterID& source2)
    {
      EmitBinary (destination, source1, source2, ">=");
    }
    
		      
    //-----------------------------------------------------------------------
		      
    CgGenerator::SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq)
     : seq (seq)
    {
    }
    
    StringsArrayPtr CgGenerator::SequenceCodeGenerator::Generate ()
    {
      strings = boost::make_shared<StringsArray> ();
      seenRegisters.clear();
      
      CodegenVisitor visitor (this, strings);
      seq.Visit (visitor);
      return strings;
    }
    
    std::string CgGenerator::SequenceCodeGenerator::TypeToCgType (const parser::SemanticsHandler::TypePtr& type)
    {
      std::string typeStr;
      switch (type->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
	{
	  switch (type->GetBaseType())
	  {
	  case parser::SemanticsHandler::Void:	typeStr = "void"; break;
	  case parser::SemanticsHandler::Bool:	typeStr = "bool"; break;
	  case parser::SemanticsHandler::Int:	typeStr = "int"; break;
	  case parser::SemanticsHandler::UInt:	typeStr = "unsigned"; break;
	  case parser::SemanticsHandler::Float:	typeStr = "float"; break;
	  }
	}
	break;
      case parser::SemanticsHandler::Type::Sampler:
	{
	  switch (type->GetSamplerType())
	  {
	  case parser::SemanticsHandler::_1D:	typeStr = "sampler1D"; break;
	  case parser::SemanticsHandler::_2D:	typeStr = "sampler2D"; break;
	  case parser::SemanticsHandler::_3D:	typeStr = "sampler3D"; break;
	  case parser::SemanticsHandler::CUBE:	typeStr = "samplerCUBE"; break;
	  }
	}
	break;
      case parser::SemanticsHandler::Type::Array:
	{
	  // FIXME: In Cg, array dimensions come after var name
	  return TypeToCgType (type->GetArrayVectorMatrixBaseType());
	}
	break;
      case parser::SemanticsHandler::Type::Vector:
	{
	  typeStr = TypeToCgType (type->GetArrayVectorMatrixBaseType());
	  char compStr[2];
	  snprintf (compStr, sizeof (compStr), "%u", type->GetVectorTypeComponents());
	  typeStr.append (compStr);
	}
	break;
      case parser::SemanticsHandler::Type::Matrix:
	{
	  typeStr = TypeToCgType (type->GetArrayVectorMatrixBaseType());
	  char compStr[4];
	  snprintf (compStr, sizeof (compStr), "%ux%u", type->GetMatrixTypeRows(), type->GetMatrixTypeCols());
	  typeStr.append (compStr);
	}
	break;
      }
      return typeStr;
    }
    
    static bool IsCgIdentifierChar (UChar32 ch)
    {
    return (((ch >= 'a') && (ch <= 'z'))
	    || ((ch >= 'A') && (ch <= 'Z'))
	    || ((ch >= '0') && (ch <= '9'))
	    || (ch == '_'));
    }
    
    static std::string FallbackUCEncode (const UnicodeString& str)
    {
      std::string out_str;
      StringCharacterIterator idIt (str);
      while (idIt.hasNext())
      {
	UChar32 ch = idIt.next32PostInc();
	// Valid Cg identifier character?
	if (ch == '_')
	{
	  out_str.append ("__");
	}
	else if (IsCgIdentifierChar (ch))
	{
	  char s[2] = {ch, 0};
	  out_str.append (s);
	}
	else
	{
	  char formatted[11];
	  snprintf (formatted, sizeof (formatted), "_%x_", ch);
	  out_str.append (formatted);
	}
      }
      return out_str;
    }
    
    static char rfc3492_encode_digit (size_t d)
    {
      return d + 22 + 75 * (d < 26);
      /*  0..25 map to ASCII a..z or A..Z */
      /* 26..35 map to ASCII 0..9         */
    }
    
    enum
    {
      rfc3492_base = 36,
      rfc3492_tmin = 1,
      rfc3492_tmax = 26,
      rfc3492_skew = 38,
      rfc3492_damp = 700
    };
    
    static size_t rfc3492_adapt (size_t delta, size_t numpoints, bool firsttime)
    {
      size_t k;

      delta = firsttime ? delta / rfc3492_damp : delta >> 1;
      /* delta >> 1 is a faster way of doing delta / 2 */
      delta += delta / numpoints;

      for (k = 0;  delta > ((rfc3492_base - rfc3492_tmin) * rfc3492_tmax) / 2;  k += rfc3492_base) {
	delta /= rfc3492_base - rfc3492_tmin;
      }

      return k + (rfc3492_base - rfc3492_tmin + 1) * delta / (delta + rfc3492_skew);
    }

    std::string CgGenerator::SequenceCodeGenerator::RegisterNameToCgIdentifier (const UnicodeString& str)
    {
      std::string basic_str;
      UChar32 minNonBasicCP = (UChar32)~0;
      // An implementation of the punycode algorithm, see RFC 3492
      StringCharacterIterator idIt (str);
      while (idIt.hasNext())
      {
	UChar32 ch = idIt.next32PostInc();
	// Valid Cg identifier character?
	if (IsCgIdentifierChar (ch))
	{
	  char s[2] = {ch, 0};
	  basic_str.append (s);
	}
	else if (ch < minNonBasicCP)
	  minNonBasicCP = ch;
      }
      // Actual encoding, see RFC 3492 sect. 6.3
      const size_t initial_bias = 72;
      const size_t initial_n 	= 0;
      size_t n = initial_n;
      size_t delta = 0;
      size_t bias = initial_bias;
      size_t h;
      size_t b;
      // let h = b = the number of basic code points in the input
      h = b = basic_str.size();
      std::string outStr (basic_str);
      outStr.append ("_");
      while (h < size_t (str.length()))
      {
	// let m = the minimum {non-basic} code point >= n in the input
	size_t m = minNonBasicCP;
	if (m - n > ((size_t)~0 - delta) / (h + 1))
	  return FallbackUCEncode (str);
	delta = delta + (m - n) * (h + 1);
	n = m;
	// for each code point c in the input (in order) ...
	for(idIt.setToStart(); idIt.hasNext();)
	{
	  UChar32 c = idIt.next32PostInc();;
	  if ((size_t (c) < n) || (IsCgIdentifierChar (c)))
	  {
	    // increment delta, fail on overflow
	    delta++;
	    if (delta == 0)
	      return FallbackUCEncode (str);
	  }
	  if (size_t (c) == n)
	  {
	    size_t q = delta;
	    //for k = base to infinity in steps of base 
	    for (size_t k = rfc3492_base; ; k += rfc3492_base)
	    {
	      size_t t; 
	      if (k <= bias /*+ rfc3492_tmin*/)
		t = rfc3492_tmin;
	      else if (k >= bias + rfc3492_tmax)
		t = rfc3492_tmax;
	      else
		t = k - bias;
	      if (q < t) break;
	      size_t digit = t + ((q - t) % (rfc3492_base - t));
	      char s[2] = {rfc3492_encode_digit (digit), 0};
	      outStr.append (s);
	      q = (q - t) / (rfc3492_base - t);
	    }
	    char s[2] = {rfc3492_encode_digit (q), 0};
	    outStr.append (s);
	    bias = rfc3492_adapt (delta, h + 1, h == b);
	    delta = 0;
	    h++;
	  }
	}
	delta++;
	n++;
      }
      return outStr;
    }
    
    std::string CgGenerator::SequenceCodeGenerator::GetOutputRegisterName (const Sequence::RegisterID& reg,
									   bool autoAllocate)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (reg);
      if (regIt != seenRegisters.end())
	return regIt->second;
      
      if (!autoAllocate) return std::string ();
      
      Sequence::RegisterBankPtr bankPtr;
      Sequence::RegisterPtr regPtr (seq.QueryRegisterFromID (reg, bankPtr));
      
      std::string cgName (RegisterNameToCgIdentifier (regPtr->GetName()));
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
