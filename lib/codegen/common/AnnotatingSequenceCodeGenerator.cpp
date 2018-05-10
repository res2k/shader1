/*
    Shader1
    Copyright (c) 2015-2017 Frank Richter


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
#include "codegen/common/AnnotatingSequenceCodeGenerator.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"

#include <boost/call_traits.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace codegen
  {
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

      DECLARE_STATIC_FORMATTER(FormatPair, "({0}, {1})");

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
      struct DebugCommentArgHelper<intermediate::SequenceVisitor::SampleTextureOp>
      {
        static const char* FormatArg(intermediate::SequenceVisitor::SampleTextureOp value)
        {
          switch (value)
          {
          case intermediate::SequenceVisitor::tex1D:
            return "tex1D";
          case intermediate::SequenceVisitor::tex2D:
            return "tex2D";
          case intermediate::SequenceVisitor::tex3D:
            return "tex3D";
          case intermediate::SequenceVisitor::texCUBE:
            return "texCUBE";
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

    AnnotatingSequenceCodeGenerator::Visitor::Visitor (const StringsArrayPtr& target)
      : BasicSequenceCodeGenerator::Visitor (target)
    {}

#define _GENERATE_METHOD_PARAM(Z, N, Data)                        \
  BOOST_PP_COMMA() const char* BOOST_PP_CAT(name, N)              \
  BOOST_PP_COMMA() BOOST_PP_CAT(const A, N)& BOOST_PP_CAT(a, N)

#define _GENERATE_FMT_PLACEHOLDER(Z, N, Data)                                         \
    BOOST_PP_IF(N, ",", "") " {" BOOST_PP_STRINGIZE(BOOST_PP_INC(BOOST_PP_MUL(N, 2))) \
    "}={" BOOST_PP_STRINGIZE(BOOST_PP_MUL(BOOST_PP_INC(N), 2))   "}"
#define _GENERATE_FMT_ARGUMENT(Z, N, Data)        \
    BOOST_PP_COMMA() BOOST_PP_CAT(name, N)        \
    BOOST_PP_COMMA() DebugCommentArgHelper<BOOST_PP_CAT(A, N)>::FormatArg (BOOST_PP_CAT(a, N))

#define _DEFINE_DEBUG_COMMENT(Z, ArgNum, Data)                                                \
    DECLARE_STATIC_FORMATTER(BOOST_PP_CAT(DebugComment_fmt_, ArgNum), "// {0} ->"             \
      BOOST_PP_REPEAT_FROM_TO_ ## Z (0, BOOST_PP_INC(ArgNum),                                 \
        _GENERATE_FMT_PLACEHOLDER, _));                                                       \
                                                                                              \
    template<BOOST_PP_ENUM_PARAMS_Z(Z, BOOST_PP_INC(ArgNum), typename A)>                     \
    void AnnotatingSequenceCodeGenerator::Visitor::DebugComment (const char* opStr            \
      BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_METHOD_PARAM, _)) const          \
    {                                                                                         \
      uc::String commentStr;                                                                  \
      BOOST_PP_CAT(DebugComment_fmt_, ArgNum) (commentStr, opStr                              \
        BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_FMT_ARGUMENT, _));             \
      target->AddString (commentStr);                                                         \
    }

    BOOST_PP_REPEAT (BOOST_PP_DEC (COMMENT_MAX_ARGS), _DEFINE_DEBUG_COMMENT, _)

#undef _DEFINE_DEBUG_COMMENT

#define _GENERATE_INVOKE_ARG(R, Data, Elem)     \
    BOOST_PP_COMMA() BOOST_PP_STRINGIZE(Elem) BOOST_PP_COMMA() (Elem)
#define DEBUG_COMMENT(Op, Seq)                                                                \
    if (debugCommentsEnabled)                                                                 \
    {                                                                                         \
      DebugComment (Op BOOST_PP_SEQ_FOR_EACH(_GENERATE_INVOKE_ARG, _, Seq));                  \
    }

    void AnnotatingSequenceCodeGenerator::Visitor::DebugComment (const uc::String& str)
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

    void AnnotatingSequenceCodeGenerator::Visitor::OpConstBool (const RegisterPtr& destination,
                                                                bool value)
    {
      DEBUG_COMMENT ("ConstBool", (destination)(value));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpConstInt (const RegisterPtr& destination,
                                                               int value)
    {
      DEBUG_COMMENT ("ConstInt", (destination)(value));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpConstUInt (const RegisterPtr& destination,
                                                                unsigned int value)
    {
      DEBUG_COMMENT ("ConstUInt", (destination)(value));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpConstFloat (const RegisterPtr& destination,
                                                                 float value)
    {
      DEBUG_COMMENT ("ConstFloat", (destination)(value));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpAssign (const RegisterPtr& destination,
                                                             const RegisterPtr& source)
    {
      DEBUG_COMMENT ("Assign", (destination)(source));
    }


    void AnnotatingSequenceCodeGenerator::Visitor::OpCast (const RegisterPtr& destination,
                                                           intermediate::BasicType destType,
                                                           const RegisterPtr& source)
    {
      DEBUG_COMMENT ("Cast", (destination)(destType)(source));
    }

    DECLARE_STATIC_FORMATTER(FormatVector, "{0}{1}");

    void AnnotatingSequenceCodeGenerator::Visitor::OpMakeVector (const RegisterPtr& destination,
                                                                 intermediate::BasicType compType,
                                                                 const std::vector<RegisterPtr>& sources)
    {
      DEBUG_COMMENT ("MakeVector", (destination)(compType)(sources));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpVectorDot (const RegisterPtr& destination,
                                                                const RegisterPtr& source1,
                                                                const RegisterPtr& source2)
    {
      DEBUG_COMMENT ("VectorDot", (destination)(source1)(source2));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpVectorCross (const RegisterPtr& destination,
                                                                  const RegisterPtr& source1,
                                                                  const RegisterPtr& source2)
    {
      DEBUG_COMMENT ("VectorCross", (destination)(source1)(source2));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpVectorNormalize (const RegisterPtr& destination,
                                                                      const RegisterPtr& source)
    {
      DEBUG_COMMENT ("VectorNormalize", (destination)(source));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpVectorLength (const RegisterPtr& destination,
                                                                   const RegisterPtr& source)
    {
      DEBUG_COMMENT ("VectorLength", (destination)(source));
    }

    DECLARE_STATIC_FORMATTER(FormatMatrix, "{0}{1}x{2}");

    void AnnotatingSequenceCodeGenerator::Visitor::OpMakeMatrix (const RegisterPtr& destination,
                                                                 intermediate::BasicType compType,
                                                                 unsigned int matrixRows,
                                                                 unsigned int matrixCols,
                                                                 const std::vector<RegisterPtr>& sources)
    {
      DEBUG_COMMENT ("MakeMatrix", (destination)(compType)(matrixRows)(matrixCols)(sources));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpMatrixLinAlgMul (const RegisterPtr& destination,
                                                                      const RegisterPtr& source1,
                                                                      const RegisterPtr& source2)
    {
      DEBUG_COMMENT ("MatrixLinAlgMul", (destination)(source1)(source2));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpMakeArray (const RegisterPtr& destination,
                                                                const std::vector<RegisterPtr>& sources)
    {
      DEBUG_COMMENT ("MakeArray", (destination)(sources));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpExtractArrayElement (const RegisterPtr& destination,
                                                                          const RegisterPtr& source,
                                                                          const RegisterPtr& index)
    {
      DEBUG_COMMENT ("ExtractArrayElement", (destination)(source)(index));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpChangeArrayElement (const RegisterPtr& destination,
                                                                         const RegisterPtr& source,
                                                                         const RegisterPtr& index,
                                                                         const RegisterPtr& newValue)
    {
      DEBUG_COMMENT ("ChangeArrayElement", (destination)(source)(index)(newValue));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpGetArrayLength (const RegisterPtr& destination,
                                                                     const RegisterPtr& array)
    {
      DEBUG_COMMENT ("GetArrayLength", (destination)(array));
    }


    void AnnotatingSequenceCodeGenerator::Visitor::OpExtractVectorComponent (const RegisterPtr& destination,
                                                                             const RegisterPtr& source,
                                                                             unsigned int comp)
    {
      DEBUG_COMMENT ("ExtractVectorComponent", (destination)(source)(comp));
    }


    void AnnotatingSequenceCodeGenerator::Visitor::OpArith (const RegisterPtr& destination,
                                                            ArithmeticOp op,
                                                            const RegisterPtr& source1,
                                                            const RegisterPtr& source2)
    {
      DEBUG_COMMENT ("Arith", (destination)(op)(source1)(source2));
    }


    void AnnotatingSequenceCodeGenerator::Visitor::OpLogic (const RegisterPtr& destination,
                                                            LogicOp op,
                                                            const RegisterPtr& source1,
                                                            const RegisterPtr& source2)
    {
      DEBUG_COMMENT ("Logic", (destination)(op)(source1)(source2));
    }


    void AnnotatingSequenceCodeGenerator::Visitor::OpUnary (const RegisterPtr& destination,
                                                            UnaryOp op,
                                                            const RegisterPtr& source)
    {
      DEBUG_COMMENT ("Unary", (destination)(op)(source));
    }


    void AnnotatingSequenceCodeGenerator::Visitor::OpCompare (const RegisterPtr& destination,
                                                              CompareOp op,
                                                              const RegisterPtr& source1,
                                                              const RegisterPtr& source2)
    {
      DEBUG_COMMENT ("Compare", (destination)(op)(source1)(source2));
    }

    DECLARE_STATIC_FORMATTER(FormatImpMapEntry, " {1} -> \"{0}\"\n");
    DECLARE_STATIC_FORMATTER(FormatExpMapEntry, " {1} <- \"{0}\"\n");

    void AnnotatingSequenceCodeGenerator::Visitor::BeforeOpBlock (const Sequence::IdentifierToRegMap& identToRegID_imp)
    {
      if (debugCommentsEnabled)
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
    }


    void AnnotatingSequenceCodeGenerator::Visitor::AfterOpBlock (const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      if (debugCommentsEnabled)
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
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpBranch (const RegisterPtr& conditionReg,
                                                             const intermediate::SequenceOpPtr& seqOpIf,
                                                             const intermediate::SequenceOpPtr& seqOpElse)
    {
      DEBUG_COMMENT ("If", (conditionReg));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpWhile (const RegisterPtr& conditionReg,
                                                            const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                                                            const intermediate::SequenceOpPtr& seqOpBody)
    {
      DEBUG_COMMENT ("While", (conditionReg)(loopedRegs));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpSampleTexture (const RegisterPtr& destination,
                                                                    SampleTextureOp what,
                                                                    const RegisterPtr& sampler,
                                                                    const RegisterPtr& coord)
    {
      DEBUG_COMMENT ("SampleTexture", (what)(sampler)(coord));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      DEBUG_COMMENT ("Return", (outParamVals));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpFunctionCall (const uc::String& funcIdent,
                                                                   const std::vector<RegisterPtr>& inParams,
                                                                   const std::vector<RegisterPtr>& outParams)
    {
      DEBUG_COMMENT ("FunctionCall", (funcIdent)(inParams)(outParams));
    }

    void AnnotatingSequenceCodeGenerator::Visitor::OpBuiltinCall (const RegisterPtr& destination,
                                                                  intermediate:: BuiltinFunction what,
                                                                  const std::vector<RegisterPtr>& inParams)
    {
      DEBUG_COMMENT ("BuiltinCall", (destination)(what)(inParams));
    }

    //-----------------------------------------------------------------------

    AnnotatingSequenceCodeGenerator::AnnotatingSequenceCodeGenerator (const intermediate::Sequence& seq)
      : BasicSequenceCodeGenerator (seq)
    {
    }

    DECLARE_STATIC_FORMATTER(FormatSeqImpEntry, " \"{0}\" -> {1}\n");
    DECLARE_STATIC_FORMATTER(FormatSeqExpEntry, " \"{0}\" <- {1}\n");

    void AnnotatingSequenceCodeGenerator::BeforeSequence (Visitor& visitor)
    {
      if (!visitor.GetDebugCommentsEnabled ()) return;

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

    void AnnotatingSequenceCodeGenerator::AfterSequence (Visitor& visitor)
    {
      if (!visitor.GetDebugCommentsEnabled ()) return;

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
  } // namespace codegen
} // namespace s1
