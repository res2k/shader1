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

#include "GLSLSequenceCodeGenerator.h"

#include "GLSLTraits.h"

#include "codegen/glsl/GLSLOptions.h"

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      class SequenceCodeGenerator::GLSLCodegenVisitor : public SequenceCodeGenerator::CodegenVisitor
      {
      public:
        GLSLCodegenVisitor (SequenceCodeGenerator* owner,
                            const StringsArrayPtr& target);

        void OpMatrixLinAlgMul (const RegisterPtr& destination,
                                const RegisterPtr& source1,
                                const RegisterPtr& source2) override;
        void OpArith (const RegisterPtr& destination,
                      ArithmeticOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2) override;
      };

      SequenceCodeGenerator::GLSLCodegenVisitor::GLSLCodegenVisitor (SequenceCodeGenerator* owner,
                                                                     const StringsArrayPtr& target)
        : CodegenVisitor (owner, target)
      {}

      void SequenceCodeGenerator::GLSLCodegenVisitor::OpMatrixLinAlgMul (const RegisterPtr& destination,
                                                                         const RegisterPtr& source1,
                                                                         const RegisterPtr& source2)
      {
        AnnotatingSequenceCodeGenerator::Visitor::OpMatrixLinAlgMul (destination, source1, source2);
        // GLSL: Linear algebraic multiplication of matrices & vectors is handled by '*'
        EmitBinary (destination, source1, source2, "*");
      }

      void SequenceCodeGenerator::GLSLCodegenVisitor::OpArith (const RegisterPtr& destination,
                                                               ArithmeticOp op,
                                                               const RegisterPtr& source1,
                                                               const RegisterPtr& source2)
      {
        // Component-wise multiplication is done via a builtin function in GLSL
        if ((op == ArithmeticOp::Mul)
            && (source1->GetOriginalType ()->GetTypeClass () == parser::SemanticsHandler::Type::Matrix)
            && (source2->GetOriginalType ()->GetTypeClass () == parser::SemanticsHandler::Type::Matrix))
        {
          AnnotatingSequenceCodeGenerator::Visitor::OpArith (destination, op, source1, source2);
          EmitFunctionCall (destination, "matrixCompMult", source1, source2);
          return;
        }

        CodegenVisitor::OpArith (destination, op, source1, source2);
      }

      //---------------------------------------------------------------------

      SequenceCodeGenerator::SequenceCodeGenerator (const intermediate::Sequence& seq,
                                                    sl::ImportedNameResolver* nameRes,
                                                    const intermediate::ProgramFunction::TransferMappings& transferIn,
                                                    const intermediate::ProgramFunction::TransferMappings& transferOut,
                                                    const std::vector<uc::String>& outParams,
                                                    const sl::Traits& traits,
                                                    const Options& options)
        : sl::SequenceCodeGenerator (seq, nameRes, transferIn, transferOut, outParams, traits, options)
      {
      }

      const Options& SequenceCodeGenerator::GetOptions () const
      {
        return static_cast<const Options&> (options);
      }

      std::unique_ptr<SequenceCodeGenerator::CodegenVisitor> SequenceCodeGenerator::CreateVisitor ()
      {
        return std::unique_ptr<CodegenVisitor> (new GLSLCodegenVisitor (this, strings));
      }

      std::unique_ptr<sl::SequenceCodeGenerator>
        SequenceCodeGenerator::CreateForBlock (const intermediate::Sequence& seq,
                                               sl::ImportedNameResolver* nameRes) const
      {
        static intermediate::ProgramFunction::TransferMappings emptyMappings;
        std::unique_ptr<SequenceCodeGenerator> p;
        p.reset (new SequenceCodeGenerator (seq, nameRes,
                                            emptyMappings, emptyMappings,
                                            outParams,
                                            traits, GetOptions ()));
        return std::move (p);
      }

      static format::StaticFormatter FormatTransferID ("v2f_{0}");

      std::vector<SequenceCodeGenerator::InputTransferPair>
        SequenceCodeGenerator::GenerateTransfersIn () const
      {
        std::vector<SequenceCodeGenerator::InputTransferPair> result;
        for (const auto& transfer : transferIn)
        {
          result.emplace_back (transfer.second,
                               FormatTransferID.to<uc::String> (traits.ConvertIdentifier (transfer.first)));
        }
        return result;
      }

      std::vector<SequenceCodeGenerator::OutputTransferPair>
        SequenceCodeGenerator::GenerateTransfersOut () const
      {
        std::vector<SequenceCodeGenerator::OutputTransferPair> result;
        for (const auto& transfer : transferOut)
        {
          result.emplace_back (FormatTransferID.to<uc::String> (traits.ConvertIdentifier (transfer.first)),
                               transfer.second);
        }
        return result;
      }
    } // namespace glsl
  } // namespace codegen
} // namespace s1
