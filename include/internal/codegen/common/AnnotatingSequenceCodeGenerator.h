/*
Shader1
Copyright (c) 2015 Frank Richter


This library is free software; you can redistribute it and/or
modify it under the terms of the wxWindows Library Licence.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Please see the wxWindows Library Licence for more details.
The licence texts can be found in the source distribution files
LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __CODEGEN_ANNOTATINGSEQUENCECODEGENERATOR_H__
#define __CODEGEN_ANNOTATINGSEQUENCECODEGENERATOR_H__

#include "base/format/Formatter.h"
#include "codegen/common/BasicSequenceCodeGenerator.h"

#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repeat.hpp>

namespace s1
{
  namespace codegen
  {
    class AnnotatingSequenceCodeGenerator : public BasicSequenceCodeGenerator
    {
    protected:
      class Visitor : public BasicSequenceCodeGenerator::Visitor
      {
      protected:
        friend class AnnotatingSequenceCodeGenerator;

        // Write a debug comment to the output.
        void DebugComment(const uc::String& str);

#define COMMENT_MAX_ARGS    BOOST_PP_DIV(FORMATTER_MAX_ARGS, 2)

#define _GENERATE_METHOD_PARAM(Z, N, Data)                              \
        BOOST_PP_COMMA() const char* BOOST_PP_CAT(name, N)              \
        BOOST_PP_COMMA() BOOST_PP_CAT(const A, N)& BOOST_PP_CAT(a, N)
#define _DECLARE_DEBUG_COMMENT(Z, ArgNum, Data)                                       \
        template<BOOST_PP_ENUM_PARAMS_Z(Z, BOOST_PP_INC(ArgNum), typename A)>         \
        void DebugComment (const char* opStr                                          \
          BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_METHOD_PARAM, _)) const;

        BOOST_PP_REPEAT(BOOST_PP_DEC(FORMATTER_MAX_ARGS), _DECLARE_DEBUG_COMMENT, _)

#undef _DECLARE_DEBUG_COMMENT
#undef _GENERATE_METHOD_PARAM

      public:
        Visitor (const StringsArrayPtr& target);

        void OpConstBool (const RegisterPtr& destination,
                          bool value) override;
        void OpConstInt (const RegisterPtr& destination,
                         int value) override;
        void OpConstUInt (const RegisterPtr& destination,
                          unsigned int value) override;
        void OpConstFloat (const RegisterPtr& destination,
                           float value) override;

        void OpAssign (const RegisterPtr& destination,
                       const RegisterPtr& source) override;

        void OpCast (const RegisterPtr& destination,
                     intermediate::BasicType destType,
                     const RegisterPtr& source) override;

        void OpMakeVector (const RegisterPtr& destination,
                           intermediate::BasicType compType,
                           const std::vector<RegisterPtr>& sources) override;

        void OpMakeMatrix (const RegisterPtr& destination,
                           intermediate::BasicType compType,
                           unsigned int matrixRows, unsigned int matrixCols,
                           const std::vector<RegisterPtr>& sources) override;

        void OpMakeArray (const RegisterPtr& destination,
                          const std::vector<RegisterPtr>& sources) override;
        void OpExtractArrayElement (const RegisterPtr& destination,
                                    const RegisterPtr& source,
                                    const RegisterPtr& index) override;
        void OpChangeArrayElement (const RegisterPtr& destination,
                                   const RegisterPtr& source,
                                   const RegisterPtr& index,
                                   const RegisterPtr& newValue) override;
        void OpGetArrayLength (const RegisterPtr& destination,
                               const RegisterPtr& array) override;

        void OpExtractVectorComponent (const RegisterPtr& destination,
                                       const RegisterPtr& source,
                                       unsigned int comp) override;

        void OpArith (const RegisterPtr& destination,
                      ArithmeticOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2) override;

        void OpLogic (const RegisterPtr& destination,
                      LogicOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2) override;

        void OpUnary (const RegisterPtr& destination,
                      UnaryOp op,
                      const RegisterPtr& source) override;

        void OpCompare (const RegisterPtr& destination,
                        CompareOp op,
                        const RegisterPtr& source1,
                        const RegisterPtr& source2) override;

        void BeforeOpBlock (const Sequence::IdentifierToRegMap& identToRegID_imp);
        void AfterOpBlock (const Sequence::IdentifierToRegMap& identToRegID_exp);

        void OpBranch (const RegisterPtr& conditionReg,
                       const intermediate::SequenceOpPtr& seqOpIf,
                       const intermediate::SequenceOpPtr& seqOpElse) override;
        void OpWhile (const RegisterPtr& conditionReg,
                      const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                      const intermediate::SequenceOpPtr& seqOpBody) override;

        void OpReturn (const std::vector<RegisterPtr>& outParamVals) override;
        void OpFunctionCall (const uc::String& funcIdent,
                             const std::vector<RegisterPtr>& inParams,
                             const std::vector<RegisterPtr>& outParams) override;
        void OpBuiltinCall (const RegisterPtr& destination,
                            intermediate::BuiltinFunction what,
                            const std::vector<RegisterPtr>& inParams) override;
      };

      void BeforeSequence (Visitor& visitor);
      void AfterSequence (Visitor& visitor);
    public:
      AnnotatingSequenceCodeGenerator (const intermediate::Sequence& seq);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_ANNOTATINGSEQUENCECODEGENERATOR_H__
