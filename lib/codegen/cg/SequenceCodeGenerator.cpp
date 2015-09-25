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
#include "base/format/uc_String.h"

#include "intermediate/SequenceOp/SequenceOp.h"
#include "SequenceCodeGenerator.h"

#include <boost/call_traits.hpp>
#include <boost/make_shared.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
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

    namespace
    {
      template<typename T>
      struct DebugCommentArgHelper
      {
        static typename boost::call_traits<T>::param_type FormatArg(typename boost::call_traits<T>::param_type value)
        { return value;  }
      };

      template<>
      struct DebugCommentArgHelper<bool>
      {
        static const char* FormatArg(bool value)
        { return value ? "true" : "false";  }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::RegisterPtr>
      {
        static const uc::String& FormatArg(const intermediate::RegisterPtr& value)
        { return value->GetName(); }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::BasicType>
      {
        static const char* FormatArg(intermediate::BasicType value)
        {
          switch (value)
          {
          case s1::intermediate::Bool:
            return "Bool";
          case s1::intermediate::Int:
            return "Int";
          case s1::intermediate::UInt:
            return "UInt";
          case s1::intermediate::Float:
            return "Float";
          default:
            break;
          }
          return "???";
        }
      };

      template<typename T>
      struct DebugCommentArgHelper<std::vector<T> >
      {
        static uc::String FormatArg (const std::vector<T>& vec)
        {
          format::Formatter<> fmtItem ("{0}");
          uc::String result ("[");
          bool first = true;
          for (const T& item : vec)
          {
            uc::String itemStr;
            if (!first) result.append (", ");
            first = false;
            fmtItem (itemStr, DebugCommentArgHelper<T>::FormatArg (item));
            result.append (itemStr);
          }
          result.append ("]");
          return result;
        }
      };

      static format::StaticFormatter FormatPair ("({0}, {1})");

      template<typename T, typename U>
      struct DebugCommentArgHelper<std::pair<T, U> >
      {
        static uc::String FormatArg (const std::pair<T, U>& pair)
        {
          uc::String result;
          FormatPair (result,
            DebugCommentArgHelper<T>::FormatArg (pair.first),
            DebugCommentArgHelper<U>::FormatArg (pair.second));
          return result;
        }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::SequenceVisitor::ArithmeticOp>
      {
        static const char* FormatArg(intermediate::SequenceVisitor::ArithmeticOp value)
        {
          switch (value)
          {
          case intermediate::SequenceVisitor::Add:
            return "Add";
          case intermediate::SequenceVisitor::Sub:
            return "Sub";
          case intermediate::SequenceVisitor::Mul:
            return "Mul";
          case intermediate::SequenceVisitor::Div:
            return "Div";
          case intermediate::SequenceVisitor::Mod:
            return "Mod";
          default:
            break;
          }
          return "???";
        }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::SequenceVisitor::LogicOp>
      {
        static const char* FormatArg(intermediate::SequenceVisitor::LogicOp value)
        {
          switch (value)
          {
          case intermediate::SequenceVisitor::And:
            return "And";
          case intermediate::SequenceVisitor::Or:
            return "Or";
          default:
            break;
          }
          return "???";
        }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::SequenceVisitor::UnaryOp>
      {
        static const char* FormatArg(intermediate::SequenceVisitor::UnaryOp value)
        {
          switch (value)
          {
          case intermediate::SequenceVisitor::Neg:
            return "Neg";
          case intermediate::SequenceVisitor::Inv:
            return "Inv";
          case intermediate::SequenceVisitor::Not:
            return "Not";
          default:
            break;
          }
          return "???";
        }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::SequenceVisitor::CompareOp>
      {
        static const char* FormatArg(intermediate::SequenceVisitor::CompareOp value)
        {
          switch (value)
          {
          case intermediate::SequenceVisitor::Eq:
            return "Eq";
          case intermediate::SequenceVisitor::NE:
            return "NE";
          case intermediate::SequenceVisitor::LT:
            return "LT";
          case intermediate::SequenceVisitor::LE:
            return "LE";
          case intermediate::SequenceVisitor::GT:
            return "GT";
          case intermediate::SequenceVisitor::GE:
            return "GE";
          default:
            break;
          }
          return "???";
        }
      };

      template<>
      struct DebugCommentArgHelper<intermediate::BuiltinFunction>
      {
        static const char* FormatArg(intermediate::BuiltinFunction value)
        {
          switch (value)
          {
          case intermediate::dot:
            return "dot";
          case intermediate::cross:
            return "cross";
          case intermediate::normalize:
            return "normalize";
          case intermediate::length:
            return "length";
          case intermediate::mul:
            return "mul";
          case intermediate::tex1D:
            return "tex1D";
          case intermediate::tex2D:
            return "tex2D";
          case intermediate::tex3D:
            return "tex3D";
          case intermediate::texCUBE:
            return "texCUBE";
          case intermediate::min:
            return "min";
          case intermediate::max:
            return "max";
          case intermediate::pow:
            return "pow";
          default:
            break;
          }
          return "???";
        }
      };
    }

    // TODO: Perhaps rather a run-time option?
#ifdef _DEBUG
#define ENABLE_DEBUG_COMMENTS
#endif

#define _GENERATE_METHOD_PARAM(Z, N, Data)                        \
  BOOST_PP_COMMA() const char* BOOST_PP_CAT(name, N)              \
  BOOST_PP_COMMA() BOOST_PP_CAT(const A, N)& BOOST_PP_CAT(a, N)

#ifdef ENABLE_DEBUG_COMMENTS
  #define _GENERATE_FMT_PLACEHOLDER(Z, N, Data)                                       \
    BOOST_PP_IF(N, ",", "") " {" BOOST_PP_STRINGIZE(BOOST_PP_INC(BOOST_PP_MUL(N, 2))) \
    "}={" BOOST_PP_STRINGIZE(BOOST_PP_MUL(BOOST_PP_INC(N), 2))   "}"
  #define _GENERATE_FMT_ARGUMENT(Z, N, Data)      \
    BOOST_PP_COMMA() BOOST_PP_CAT(name, N)        \
    BOOST_PP_COMMA() DebugCommentArgHelper<BOOST_PP_CAT(A, N)>::FormatArg (BOOST_PP_CAT(a, N))

  #define _DEFINE_DEBUG_COMMENT(Z, ArgNum, Data)                                              \
    template<BOOST_PP_ENUM_PARAMS_Z(Z, BOOST_PP_INC(ArgNum), typename A)>                     \
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::DebugComment (const char* opStr  \
      BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_METHOD_PARAM, _)) const          \
    {                                                                                         \
      static format::StaticFormatter fmt ("// {0} ->"                                         \
        BOOST_PP_REPEAT_FROM_TO_ ## Z (0, BOOST_PP_INC(ArgNum),                               \
          _GENERATE_FMT_PLACEHOLDER, _));                                                     \
      uc::String commentStr;                                                                  \
      fmt (commentStr, opStr                                                                  \
        BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_FMT_ARGUMENT, _));             \
      target->AddString (commentStr);                                                         \
    } 
  #else
  #define _DEFINE_DEBUG_COMMENT(Z, ArgNum, Data)                                              \
    template<BOOST_PP_ENUM_PARAMS_Z(Z, BOOST_PP_INC(ArgNum), typename A)>                     \
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::DebugComment (const char* opStr  \
      BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_METHOD_PARAM, _)) const          \
    {                                                                                         \
    }
  #endif

    BOOST_PP_REPEAT (BOOST_PP_DEC (FORMATTER_MAX_ARGS), _DEFINE_DEBUG_COMMENT, _)

  #undef _DEFINE_DEBUG_COMMENT

  #define _GENERATE_INVOKE_ARG(R, Data, Elem)     \
    BOOST_PP_COMMA() BOOST_PP_STRINGIZE(Elem) BOOST_PP_COMMA() (Elem)
  #define DEBUG_COMMENT(Op, Seq)    \
    DebugComment (Op BOOST_PP_SEQ_FOR_EACH(_GENERATE_INVOKE_ARG, _, Seq));
  
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::DebugComment (const uc::String& str)
    {
      uc::String remainder (str);
      bool firstLine = true;
      uc::String line;
      while (!remainder.isEmpty ())
      {
        line = (firstLine ? "/* " : "   ");
        firstLine = false;
        uc::String::size_type lfPos = remainder.indexOf ('\n');
        if (lfPos == uc::String::npos)
        {
          line.append (remainder);
          line.append (" */");
          target->AddString (line);
          return;
        }
        line.append (remainder.data (), lfPos);
        target->AddString (line);
        remainder = uc::String (remainder, lfPos + 1);
        if (remainder.isEmpty())
          target->AddString (" */");
      }
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
      DEBUG_COMMENT ("ConstBool", (destination)(value));
      const char* valueStr = value ? "true" : "false";
      PseudoAssign (destination, valueStr);
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstInt (const RegisterPtr& destination,
									 int value)
    {
      DEBUG_COMMENT ("ConstInt", (destination)(value));
      std::stringstream valueStrStream;
      valueStrStream << value;
      PseudoAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstUInt (const RegisterPtr& destination,
									  unsigned int value)
    {
      DEBUG_COMMENT ("ConstUInt", (destination)(value));
      std::stringstream valueStrStream;
      valueStrStream << value;
      PseudoAssign (destination, valueStrStream.str().c_str());
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpConstFloat (const RegisterPtr& destination,
									   float value)
    {
      DEBUG_COMMENT ("ConstFloat", (destination)(value));
      std::stringstream valueStrStream;
      valueStrStream << value;
      PseudoAssign (destination, valueStrStream.str().c_str());
    }
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpAssign (const RegisterPtr& destination,
								       const RegisterPtr& source)
    {
      DEBUG_COMMENT ("Assign", (destination)(source));
      std::string sourceName (owner->GetOutputRegisterName (source));
      EmitAssign (destination, sourceName.c_str());
    }
			      
			      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpCast (const RegisterPtr& destination,
								     intermediate::BasicType destType,
								     const RegisterPtr& source)
    {
      DEBUG_COMMENT ("Cast", (destination)(destType)(source));
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
      DEBUG_COMMENT ("MakeVector", (destination)(compType)(sources));
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
          for(const RegisterPtr& source : sources)
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
      DEBUG_COMMENT ("MakeMatrix", (destination)(compType)(matrixRows)(matrixCols)(sources));
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
      DEBUG_COMMENT ("MakeArray", (destination)(sources));
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
      DEBUG_COMMENT ("ExtractArrayElement", (destination)(source)(index));
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
      DEBUG_COMMENT ("ChangeArrayElement", (destination)(source)(index)(newValue));
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
      DEBUG_COMMENT ("GetArrayLength", (destination)(array));
      std::string sourceName (owner->GetOutputRegisterName (array));
      sourceName.append (".length");
      EmitAssign (destination, sourceName.c_str());
    }
			 
				    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
										       const RegisterPtr& source,
										       unsigned int comp)
    {
      DEBUG_COMMENT ("ExtractVectorComponent", (destination)(source)(comp));
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
      DEBUG_COMMENT ("Arith", (destination)(op)(source1)(source2));
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
      DEBUG_COMMENT ("Logic", (destination)(op)(source1)(source2));
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
      DEBUG_COMMENT ("Unary", (destination)(op)(source));
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
      DEBUG_COMMENT ("Compare", (destination)(op)(source1)(source2));
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBlock (const intermediate::SequencePtr& seq,
								      const Sequence::IdentifierToRegMap& identToRegID_imp,
								      const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
    #ifdef ENABLE_DEBUG_COMMENTS
      {
        uc::String impString ("Import map:\n");
        if (identToRegID_imp.empty ())
        {
          impString.append (" <empty>");
        }
        else
        {
          for (const Sequence::IdentifierToRegMap::value_type& impPair : identToRegID_imp)
          {
            uc::String s;
            FormatImpMapEntry (s, impPair.first, DebugCommentArgHelper<RegisterPtr>::FormatArg (impPair.second));
            impString.append (s);
          }
        }
        DebugComment (impString);
      }
    #endif // ENABLE_DEBUG_COMMENTS

      // Generate registers for 'exported' variables
      intermediate::RegisterSet writtenRegisters (seq->GetExportOuterRegs (identToRegID_exp));
      for(const RegisterPtr& writtenReg : writtenRegisters)
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

    #ifdef ENABLE_DEBUG_COMMENTS
      {
        uc::String expString ("Export map:\n");
        if (identToRegID_exp.empty ())
        {
          expString.append (" <empty>");
        }
        else
        {
          for (const Sequence::IdentifierToRegMap::value_type& expPair : identToRegID_exp)
          {
            uc::String s;
            FormatExpMapEntry (s, expPair.first, DebugCommentArgHelper<RegisterPtr>::FormatArg (expPair.second));
            expString.append (s);
          }
        }
        DebugComment (expString);
      }
    #endif // ENABLE_DEBUG_COMMENTS
    }
		      
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpBranch (const RegisterPtr& conditionReg,
								       const intermediate::SequenceOpPtr& seqOpIf,
								       const intermediate::SequenceOpPtr& seqOpElse)
    {
      DEBUG_COMMENT ("If", (conditionReg));
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
      DEBUG_COMMENT ("While", (conditionReg)(loopedRegs));
      for (size_t i = 0; i < loopedRegs.size(); i++)
      {
	// Before entering, copy original registers to their writeable counterparts
	EmitAssign (loopedRegs[i].second,
		    owner->GetOutputRegisterName (loopedRegs[i].first).c_str());
      }
      // Generate registers for each read or written variable
      intermediate::RegisterSet writtenRegisters (seqOpBody->GetWrittenRegisters());
      intermediate::RegisterSet readRegisters (seqOpBody->GetReadRegisters());
      for(const RegisterPtr& reg : writtenRegisters)
      {
	owner->GetOutputRegisterName (reg, rfForceVariable);
      }
      for(const RegisterPtr& reg : readRegisters)
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
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      DEBUG_COMMENT ("Return", (outParamVals));
      assert (outParamVals.size() == owner->outParams.size());
      for (size_t i = 0; i < outParamVals.size(); i++)
	EmitAssign (owner->outParams[i].c_str(), outParamVals[i]);
      target->AddString ("return;");
    }
    
    void CgGenerator::SequenceCodeGenerator::CodegenVisitor::OpFunctionCall (const uc::String& funcIdent,
									     const std::vector<RegisterPtr>& inParams,
									     const std::vector<RegisterPtr>& outParams)
    {
      DEBUG_COMMENT ("FunctionCall", (funcIdent)(inParams)(outParams));
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
      DEBUG_COMMENT ("BuiltinCall", (destination)(what)(inParams));
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
    
    static format::StaticFormatter FormatSeqImpEntry (" \"{0}\" -> {1}\n");
    static format::StaticFormatter FormatSeqExpEntry (" \"{0}\" <- {1}\n");

    StringsArrayPtr CgGenerator::SequenceCodeGenerator::Generate ()
    {
      strings = boost::make_shared<StringsArray> ();
      seenRegisters.clear();
      
      CodegenVisitor visitor (this, strings);

    #ifdef ENABLE_DEBUG_COMMENTS
      {
        const intermediate::Sequence::RegisterImpMappings& imports = seq.GetImports();
        uc::String impString ("Imports:\n");
        if (imports.empty ())
        {
          impString.append (" <empty>");
        }
        else
        {
          for (const intermediate::Sequence::RegisterImpMappings::value_type& imp : imports)
          {
            uc::String s;
            FormatSeqImpEntry (s, imp.first, DebugCommentArgHelper<RegisterPtr>::FormatArg (imp.second));
            impString.append (s);
          }
        }
        visitor.DebugComment (impString);
      }
    #endif // ENABLE_DEBUG_COMMENTS

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
	    endAssignments.emplace_back (transferIdent, transfer->second);
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
	      endAssignments.emplace_back (parentID, exportVar->second);
	  }
	  /* else: no ID, value is undefined; no assign */
	}
      }
      
      // Generate code for actual operations
      seq.Visit (visitor);
      
      for(const EndAssignmentPair& assignment : endAssignments)
      {
	visitor.EmitAssign (assignment.first.c_str(), assignment.second);
      }
      
    #ifdef ENABLE_DEBUG_COMMENTS
      {
        const intermediate::Sequence::RegisterExpMappings& exports = seq.GetExports();
        uc::String expString ("Exports:\n");
        if (exports.empty ())
        {
          expString.append (" <empty>");
        }
        else
        {
          for (const intermediate::Sequence::RegisterExpMappings::value_type& expPair : exports)
          {
            uc::String s;
            FormatSeqExpEntry (s, expPair.first, DebugCommentArgHelper<RegisterPtr>::FormatArg (expPair.second));
            expString.append (s);
          }
        }
        visitor.DebugComment (expString);
      }
    #endif // ENABLE_DEBUG_COMMENTS

      return strings;
    }
    
    std::string CgGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr,
                                                                           unsigned int flags)
    {
      std::string cgName;
      GetOutputRegisterName (regPtr, cgName, std::string (), flags);
      return cgName;
    }

    bool CgGenerator::SequenceCodeGenerator::GetOutputRegisterName (const RegisterPtr& regPtr,
								    std::string& name,
								    const std::string& initializer,
                                                                    unsigned int flags)
    {
      RegistersToIDMap::iterator regIt = seenRegisters.find (regPtr);
      if (regIt != seenRegisters.end())
      {
        name = regIt->second;
        if ((flags & rfForceVariable) != 0)
        {
          // Make sure we have a variable of 'cgName' declared
          std::string cgName (NameToCgIdentifier (regPtr->GetName ()));
          if (cgName != name)
          {
            EmitDeclaration (regPtr->GetOriginalType (), cgName, name);
            seenRegisters[regPtr] = cgName;
            name = cgName;
          }
        }
	return false;
      }
      
      std::string cgName (NameToCgIdentifier (regPtr->GetName ()));
      seenRegisters[regPtr] = cgName;
      
      EmitDeclaration (regPtr->GetOriginalType (), cgName, initializer);
      name = cgName;
      return true;
    }

    static format::StaticFormatter FormatDeclaration ("{0} {1}{2};");
    static format::StaticFormatter FormatDeclarationInit ("{0} {1}{2} = {3};");

    void CgGenerator::SequenceCodeGenerator::EmitDeclaration (
      const intermediate::IntermediateGeneratorSemanticsHandler::TypePtr& type,
      const std::string& name, const std::string& initializer)
    {
      std::string typeSuffix;
      std::string cgType (TypeToCgType (type, typeSuffix));
      std::string declLine;
      if (!initializer.empty ())
      {
        FormatDeclarationInit (declLine, cgType, name, typeSuffix, initializer);
      }
      else
      {
        FormatDeclaration (declLine, cgType, name, typeSuffix);
      }
      strings->AddString (declLine);
    }
  } // namespace codegen
} // namespace s1
