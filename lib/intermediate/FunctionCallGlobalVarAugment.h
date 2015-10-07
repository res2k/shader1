/*
    Shader1
    Copyright (c) 2010-2015 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

/**\file
 * Sequence visitor to augment function calls with global variables as arguments
 */

#ifndef __FUNCTIONCALLGLOBALVARAUGMENT_H__
#define __FUNCTIONCALLGLOBALVARAUGMENT_H__

#include "intermediate/CloningSequenceVisitor.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    class FunctionCallGlobalVarAugment : public CloningSequenceVisitor
    {
    public:
      FunctionCallGlobalVarAugment (const SequenceBuilderPtr& newSequenceBuilder,
                                    const SequencePtr& oldSeq,
                                    const std::vector<IntermediateGeneratorSemanticsHandler::NamePtr>& globals,
                                    int level = 0);

      RegisterPtr MapRegisterIn (const RegisterPtr& reg);
      RegisterPtr MapRegisterOut (const RegisterPtr& reg);
      void AddOpToSequence (const SequenceOpPtr& seqOp);
      void PreVisitOp (const SequenceOpPtr& op);
      void VisitEnd ();

      void OpWhile (const RegisterPtr& conditionReg,
                    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                    const SequenceOpPtr& seqOpBody);

      void OpReturn (const std::vector<RegisterPtr>& outParamVals);
      void OpFunctionCall (const uc::String& funcIdent,
        const std::vector<RegisterPtr>& inParams,
        const std::vector<RegisterPtr>& outParams);
    protected:
      /// Nesting level, used to generate unique register names
      int level;
      /// All globals
      std::vector<IntermediateGeneratorSemanticsHandler::NamePtr> globals;
      /// Names of global vars that are read
      std::vector<uc::String> globalVarNamesIn;
      /// Names of global vars that are (also) written
      std::vector<uc::String> globalVarNamesOut;

      typedef boost::unordered_map<uc::String, RegisterPtr> GlobalVarRegsMap;
      /// Map of global name to (input) register
      GlobalVarRegsMap globalVarRegsIn;
      /// Map of global name to (output) register
      GlobalVarRegsMap globalVarRegsOut;
      /// Previous set of global var output registers
      GlobalVarRegsMap globalVarRegsOutPrev;
      typedef boost::unordered_map<uc::String, uc::String> OriginalNameToGlobalMap;
      /// Map of original register name to global name
      OriginalNameToGlobalMap originalNameToGlobal;
      bool loopBlock = false;

      virtual void NestedBlock (CloningSequenceVisitor* handlingVisitor,
                                const SequencePtr& seq,
                                const Sequence::IdentifierToRegMap& identToReg_imp,
                                const Sequence::IdentifierToRegMap& identToReg_exp);

      virtual CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
        const SequencePtr& oldSequence,
        const RegisterMap& regMap);
      virtual CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
        const RegisterMap& regMap);
    };
  } // namespace intermediate
} // namespace s1

#endif // __FUNCTIONCALLGLOBALVARAUGMENT_H__
