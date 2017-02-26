/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __CODEGEN_SL_SEQUENCECODEGENERATOR_H__
#define __CODEGEN_SL_SEQUENCECODEGENERATOR_H__

#include "base/format/Formatter.h"
#include "codegen/common/AnnotatingSequenceCodeGenerator.h"
#include "codegen/common/StringsArray.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/Sequence.h"
#include "intermediate/SequenceVisitor.h"

#include <boost/preprocessor/repeat.hpp>
#include <boost/unordered_map.hpp>

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      class Options;
      struct Traits;

      struct ImportedNameResolver
      {
        virtual ~ImportedNameResolver () {}

        virtual uc::String GetImportedNameIdentifier (const uc::String& name) = 0;
        virtual uc::String GetExportedNameIdentifier (const uc::String& name) = 0;
      };

      class SequenceCodeGenerator : public AnnotatingSequenceCodeGenerator
      {
      protected:
        typedef intermediate::RegisterPtr RegisterPtr;
        typedef intermediate::Sequence Sequence;

        class SequenceIdentifiersToRegIDsNameResolver : public ImportedNameResolver
        {
          SequenceCodeGenerator* owner;
          const Sequence::IdentifierToRegMap& identToRegID_imp;
          const Sequence::IdentifierToRegMap& identToRegID_exp;
        public:
          SequenceIdentifiersToRegIDsNameResolver (SequenceCodeGenerator* owner,
                                                   const Sequence::IdentifierToRegMap& identToRegID_imp,
                                                   const Sequence::IdentifierToRegMap& identToRegID_exp);

          uc::String GetImportedNameIdentifier (const uc::String& name) override;
          uc::String GetExportedNameIdentifier (const uc::String& name) override;
        };

        class CodegenVisitor : public AnnotatingSequenceCodeGenerator::Visitor
        {
        protected:
          typedef AnnotatingSequenceCodeGenerator::Visitor AnnotatingVisitor;
          friend class SequenceCodeGenerator;

          SequenceCodeGenerator* owner;
          bool emitEmptyBlocks;

          void EmitAssign (const RegisterPtr& destination,
                           const char* value);
          /**
           * Internally store a value for a register.
           * Doesn't necessarily result in an assign and allows 'simple' values
           * to be inlined later on (for tidier generated code).
           */
          void PseudoAssign (const RegisterPtr& destination,
                             const char* value);
          void EmitAssign (const uc::String& destination,
                           const RegisterPtr& source);
          void EmitFunctionCall (const RegisterPtr& destination,
                                 const uc::String& function,
                                 const char* paramsStr);
          void EmitFunctionCall (const RegisterPtr& destination,
                                 const char* function,
                                 const char* paramsStr);
          void EmitFunctionCall (const RegisterPtr& destination,
                                 const char* function,
                                 const RegisterPtr& source1);
          void EmitFunctionCall (const RegisterPtr& destination,
                                 const char* function,
                                 const RegisterPtr& source1,
                                 const RegisterPtr& source2);
          void EmitBinary (const RegisterPtr& destination,
                           const RegisterPtr& source1,
                           const RegisterPtr& source2,
                           const char* op);
          void EmitUnary (const RegisterPtr& destination,
                          const RegisterPtr& source,
                          const char* op);
        public:
          CodegenVisitor (SequenceCodeGenerator* owner,
                          const StringsArrayPtr& target);

          void PreVisitOp (const intermediate::SequenceOpPtr& op) {}
          void PostVisitOp () {}
          void VisitEnd () {}

          void OpConstBool (const RegisterPtr& destination,
                            bool value);
          void OpConstInt (const RegisterPtr& destination,
                           int value);
          void OpConstUInt (const RegisterPtr& destination,
                            unsigned int value);
          void OpConstFloat (const RegisterPtr& destination,
                             float value);

          void OpAssign (const RegisterPtr& destination,
                         const RegisterPtr& source);

          void OpCast (const RegisterPtr& destination,
                       intermediate::BasicType destType,
                       const RegisterPtr& source);

          void OpMakeVector (const RegisterPtr& destination,
                             intermediate::BasicType compType,
                             const std::vector<RegisterPtr>& sources);
          void OpVectorDot (const RegisterPtr& destination,
                            const RegisterPtr& source1,
                            const RegisterPtr& source2) override;
          void OpVectorCross (const RegisterPtr& destination,
                              const RegisterPtr& source1,
                              const RegisterPtr& source2) override;
          void OpVectorNormalize (const RegisterPtr& destination,
                                  const RegisterPtr& source) override;
          void OpVectorLength (const RegisterPtr& destination,
                               const RegisterPtr& source) override;

          void OpMakeMatrix (const RegisterPtr& destination,
                             intermediate::BasicType compType,
                             unsigned int matrixRows, unsigned int matrixCols,
                             const std::vector<RegisterPtr>& sources);
          void OpMatrixLinAlgMul (const RegisterPtr& destination,
                                  const RegisterPtr& source1,
                                  const RegisterPtr& source2) override;

          void OpMakeArray (const RegisterPtr& destination,
                            const std::vector<RegisterPtr>& sources);
          void OpExtractArrayElement (const RegisterPtr& destination,
                                      const RegisterPtr& source,
                                      const RegisterPtr& index);
          void OpChangeArrayElement (const RegisterPtr& destination,
                                     const RegisterPtr& source,
                                     const RegisterPtr& index,
                                     const RegisterPtr& newValue);
          void OpGetArrayLength (const RegisterPtr& destination,
                                 const RegisterPtr& array);

          void OpExtractVectorComponent (const RegisterPtr& destination,
                                         const RegisterPtr& source,
                                         unsigned int comp);

          void OpArith (const RegisterPtr& destination,
                        ArithmeticOp op,
                        const RegisterPtr& source1,
                        const RegisterPtr& source2);

          void OpLogic (const RegisterPtr& destination,
                        LogicOp op,
                        const RegisterPtr& source1,
                        const RegisterPtr& source2);

          void OpUnary (const RegisterPtr& destination,
                        UnaryOp op,
                        const RegisterPtr& source);

          void OpCompare (const RegisterPtr& destination,
                          CompareOp op,
                          const RegisterPtr& source1,
                          const RegisterPtr& source2);

          void OpBlock (const intermediate::SequencePtr& seq,
                        const Sequence::IdentifierToRegMap& identToRegID_imp,
                        const Sequence::IdentifierToRegMap& identToRegID_exp);

          void OpBranch (const RegisterPtr& conditionReg,
                         const intermediate::SequenceOpPtr& seqOpIf,
                         const intermediate::SequenceOpPtr& seqOpElse);
          void OpWhile (const RegisterPtr& conditionReg,
                        const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                        const intermediate::SequenceOpPtr& seqOpBody);

          void OpReturn (const std::vector<RegisterPtr>& outParamVals);
          void OpFunctionCall (const uc::String& funcIdent,
                               const std::vector<RegisterPtr>& inParams,
                               const std::vector<RegisterPtr>& outParams);
          void OpBuiltinCall (const RegisterPtr& destination,
                              intermediate::BuiltinFunction what,
                              const std::vector<RegisterPtr>& inParams);
        };

        ImportedNameResolver* nameRes;
        const intermediate::ProgramFunction::TransferMappings& transferIn;
        const intermediate::ProgramFunction::TransferMappings& transferOut;
        const std::vector<uc::String>& outParams;
        const Traits& traits;
        // Code generation options
        const Options& options;

        typedef boost::unordered_map<RegisterPtr, std::string> RegistersToIDMap;
        RegistersToIDMap seenRegisters;
        // Track original registers for extraction results
        typedef std::pair<RegisterPtr, unsigned int> RegisterOriginPair;
        typedef boost::unordered_map<RegisterPtr, RegisterOriginPair> RegisterOriginsMap;
        RegisterOriginsMap registerOrigins;

        /// Flags for GetOutputRegisterName() methods
        enum
        {
          /// Always force a variable to be declared
          rfForceVariable = 0x1
        };
        std::string GetOutputRegisterName (const RegisterPtr& reg, unsigned int flags = 0);
        bool GetOutputRegisterName (const RegisterPtr& reg,
                                    std::string& name,
                                    const std::string& initializer,
                                    unsigned int flags = 0);
        void EmitDeclaration (const intermediate::IntermediateGeneratorSemanticsHandler::TypePtr& type,
                              const std::string& name,
                              const std::string& initializer);

        /// Create an instance of the codegen visitor.
        virtual std::unique_ptr<CodegenVisitor> CreateVisitor ();

        virtual std::unique_ptr<SequenceCodeGenerator> CreateForBlock (const intermediate::Sequence& seq,
                                                                       ImportedNameResolver* nameRes) const = 0;

        // Generate input transfers: any accesses to a register will be redirected to the paired identifier
        typedef std::pair<RegisterPtr, uc::String> InputTransferPair;
        virtual std::vector<InputTransferPair> GenerateTransfersIn () const = 0;

        // Generate output transfers: at the end, generate assignments to the identifier from the paired register
        typedef std::pair<uc::String, RegisterPtr> OutputTransferPair;
        virtual std::vector<OutputTransferPair> GenerateTransfersOut () const = 0;
      public:
        SequenceCodeGenerator (const intermediate::Sequence& seq,
                               ImportedNameResolver* nameRes,
                               const intermediate::ProgramFunction::TransferMappings& transferIn,
                               const intermediate::ProgramFunction::TransferMappings& transferOut,
                               const std::vector<uc::String>& outParams,
                               const Traits& traits,
                               const Options& options);

        StringsArrayPtr Generate () override;
      };
    } // namespace sl
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SL_SEQUENCECODEGENERATOR_H__
