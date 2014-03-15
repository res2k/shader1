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

#ifndef __CODEGEN_SEQUENCECODEGENERATOR_H__
#define __CODEGEN_SEQUENCECODEGENERATOR_H__

#include "base/format/Formatter.h"
#include "codegen/CgGenerator.h"
#include "codegen/StringsArray.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/Sequence.h"
#include "intermediate/SequenceVisitor.h"

#include <boost/preprocessor/repeat.hpp>
#include <boost/unordered_map.hpp>

namespace s1
{
  namespace codegen
  {
    struct ImportedNameResolver
    {
      virtual ~ImportedNameResolver() {}
      
      virtual std::string GetImportedNameIdentifier (const uc::String& name) = 0;
      virtual std::string GetExportedNameIdentifier (const uc::String& name) = 0;
    };
    
    class CgGenerator::SequenceCodeGenerator
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
					 
	std::string GetImportedNameIdentifier (const uc::String& name);
	std::string GetExportedNameIdentifier (const uc::String& name);
      };
      
      class CodegenVisitor : public intermediate::SequenceVisitor
      {
	friend class SequenceCodeGenerator;
	
	SequenceCodeGenerator* owner;
	StringsArrayPtr target;
	bool emitEmptyBlocks;

        // Write a debug comment to the output.
        void DebugComment(const uc::String& str);

      #define _GENERATE_METHOD_PARAM(Z, N, Data)                        \
        BOOST_PP_COMMA() const char* BOOST_PP_CAT(name, N)              \
        BOOST_PP_COMMA() BOOST_PP_CAT(const A, N)& BOOST_PP_CAT(a, N)
      #define _DECLARE_DEBUG_COMMENT(Z, ArgNum, Data)                                 \
        template<BOOST_PP_ENUM_PARAMS_Z(Z, BOOST_PP_INC(ArgNum), typename A)>         \
        void DebugComment (const char* opStr                                          \
          BOOST_PP_REPEAT_ ## Z (BOOST_PP_INC(ArgNum), _GENERATE_METHOD_PARAM, _)) const;

        BOOST_PP_REPEAT(BOOST_PP_DEC(FORMATTER_MAX_ARGS), _DECLARE_DEBUG_COMMENT, _)

      #undef _DECLARE_DEBUG_COMMENT
      #undef _GENERATE_METHOD_PARAM

	void EmitAssign (const RegisterPtr& destination,
			 const char* value);
        /**
         * Internally store a value for a register.
         * Doesn't necessarily result in an assign and allows 'simple' values
         * to be inlined later on (for tidier generated code).
         */
        void PseudoAssign (const RegisterPtr& destination,
                           const char* value);
	void EmitAssign (const char* destination,
			 const RegisterPtr& source);
	void EmitFunctionCall (const RegisterPtr& destination,
			       const char* function,
			       const char* paramsStr);
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
	void VisitEnd() {}
	
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
				     
	void OpMakeMatrix (const RegisterPtr& destination,
			   intermediate::BasicType compType,
			   unsigned int matrixRows, unsigned int matrixCols,
			   const std::vector<RegisterPtr>& sources);
				     
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
      
      const intermediate::Sequence& seq;
      ImportedNameResolver* nameRes;
      const intermediate::ProgramFunction::TransferMappings& transferIn;
      const intermediate::ProgramFunction::TransferMappings& transferOut;
      StringsArrayPtr strings;
      const std::vector<std::string>& outParams;
      
      typedef boost::unordered_map<RegisterPtr, std::string> RegistersToIDMap;
      RegistersToIDMap seenRegisters;
      // Track original registers for extraction results
      typedef std::pair<RegisterPtr, unsigned int> RegisterOriginPair;
      typedef boost::unordered_map<RegisterPtr, RegisterOriginPair> RegisterOriginsMap;
      RegisterOriginsMap registerOrigins;
      
      std::string GetOutputRegisterName (const RegisterPtr& reg);
      bool GetOutputRegisterName (const RegisterPtr& reg,
				  std::string& name,
				  const std::string& initializer);
    public:
      SequenceCodeGenerator (const intermediate::Sequence& seq,
			     ImportedNameResolver* nameRes,
			     const intermediate::ProgramFunction::TransferMappings& transferIn,
			     const intermediate::ProgramFunction::TransferMappings& transferOut,
			     const std::vector<std::string>& outParams);
      
      StringsArrayPtr Generate ();
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SEQUENCECODEGENERATOR_H__
