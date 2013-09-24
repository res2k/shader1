#include "base/common.h"

#include "codegen/CgGenerator.h"

#include "cg/ProgramCodeGenerator.h"

#include <limits>
#include <stdio.h>
#include <unicode/schriter.h>

#include <boost/multiprecision/cpp_int.hpp>

namespace s1
{
  namespace codegen
  {
    const char CgGenerator::cgTypeBool[]	= "bool";
    const char CgGenerator::cgTypeInt[]		= "int";
    const char CgGenerator::cgTypeUInt[]	= "unsigned int";
    const char CgGenerator::cgTypeFloat[]	= "float";
    
    CgGenerator::CgGenerator ()
    {
    }
    
    StringsArrayPtr CgGenerator::Generate (const intermediate::ProgramPtr& program,
					   int frequency)
    {
      ProgramCodeGenerator progGen;
      return progGen.Generate (program, frequency);
    }
    
    std::string CgGenerator::TypeToCgType (const parser::SemanticsHandler::TypePtr& type,
					   std::string& identifierSuffix,
					   const std::string& sizeStr)
    {
      identifierSuffix = "";
      std::string typeStr;
      switch (type->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
	{
	  switch (type->GetBaseType())
	  {
	  case parser::SemanticsHandler::Void:	typeStr = "void"; break;
	  case parser::SemanticsHandler::Bool:	typeStr = cgTypeBool; break;
	  case parser::SemanticsHandler::Int:	typeStr = cgTypeInt; break;
	  case parser::SemanticsHandler::UInt:	typeStr = cgTypeUInt; break;
	  case parser::SemanticsHandler::Float:	typeStr = cgTypeFloat; break;
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
	  std::string newSuffix;
	  std::string typeStr (TypeToCgType (type->GetArrayVectorMatrixBaseType(), newSuffix));
	  identifierSuffix = newSuffix + "[" + sizeStr + "]";
	  return typeStr;
	}
	break;
      case parser::SemanticsHandler::Type::Vector:
	{
	  std::string newSuffix;
	  typeStr = TypeToCgType (type->GetArrayVectorMatrixBaseType(), newSuffix);
	  char compStr[2];
	  snprintf (compStr, sizeof (compStr), "%u", type->GetVectorTypeComponents());
	  typeStr.append (compStr);
	}
	break;
      case parser::SemanticsHandler::Type::Matrix:
	{
	  std::string newSuffix;
	  typeStr = TypeToCgType (type->GetArrayVectorMatrixBaseType(), newSuffix);
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
    
    typedef boost::multiprecision::cpp_int delta_type;
    static size_t rfc3492_adapt (delta_type delta, size_t numpoints, bool firsttime)
    {
      size_t k;

      if (firsttime)
        delta = delta / int (rfc3492_damp);
      else
        delta = delta >> 1;
      /* delta >> 1 is a faster way of doing delta / 2 */
      delta += delta / numpoints;

      for (k = 0;  delta > ((rfc3492_base - rfc3492_tmin) * rfc3492_tmax) / 2;  k += rfc3492_base) {
	delta /= rfc3492_base - rfc3492_tmin;
      }

      return delta_type (k + (rfc3492_base - rfc3492_tmin + 1) * delta / (delta + int (rfc3492_skew))).convert_to<size_t> ();
    }

    std::string CgGenerator::NameToCgIdentifier (const UnicodeString& str)
    {
      std::string basic_str;
      // An implementation of the punycode algorithm, see RFC 3492
      StringCharacterIterator idIt (str);
      while (idIt.hasNext())
      {
	UChar32 ch = idIt.next32PostInc();
	// Valid Cg identifier character?
	if (IsCgIdentifierChar (ch))
	{
	  basic_str.push_back (char (ch));
	}
      }
      // Actual encoding, see RFC 3492 sect. 6.3
      const size_t initial_bias = 72;
      const size_t initial_n 	= 0;
      size_t n = initial_n;
      delta_type delta = 0;
      size_t bias = initial_bias;
      size_t h;
      size_t b;
      // let h = b = the number of basic code points in the input
      h = b = basic_str.size();
      std::string outStr (basic_str);
      outStr.push_back ('_');
      const size_t num_str_chars (str.countChar32());
      while (h < num_str_chars)
      {
	// let m = the minimum {non-basic} code point >= n in the input
	size_t m = INT32_MAX;
	for(idIt.setToStart(); idIt.hasNext();)
	{
	  UChar32 c = idIt.next32PostInc();;
	  if (!IsCgIdentifierChar (c) && (size_t (c) >= n) && (size_t (c) < m))
	    m = c;
	}
	delta = delta + delta_type (m - n) * delta_type (h + 1);
	n = m;
	// for each code point c in the input (in order) ...
	for(idIt.setToStart(); idIt.hasNext();)
	{
	  UChar32 c = idIt.next32PostInc();;
	  if ((size_t (c) < n) || (IsCgIdentifierChar (c)))
	  {
	    // increment delta
	    delta++;
	  }
	  if (size_t (c) == n)
	  {
	    delta_type q = delta;
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
	      size_t digit = t + (delta_type (q - t).convert_to<size_t>() % (rfc3492_base - t));
	      outStr.push_back (rfc3492_encode_digit (digit));
	      q = (q - t) / (rfc3492_base - t);
	    }
	    outStr.push_back (rfc3492_encode_digit (q.convert_to<size_t>()));
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
    
  } // namespace codegen
} // namespace s1
