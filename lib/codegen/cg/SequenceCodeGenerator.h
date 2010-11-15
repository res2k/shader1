#ifndef __CODEGEN_SEQUENCECODEGENERATOR_H__
#define __CODEGEN_SEQUENCECODEGENERATOR_H__

#include "codegen/CgGenerator.h"
#include "codegen/StringsArray.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/Sequence.h"
#include "intermediate/SequenceVisitor.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  namespace codegen
  {
    struct ImportedNameResolver
    {
      virtual ~ImportedNameResolver() {}
      
      virtual std::string GetImportedNameIdentifier (const UnicodeString& name) = 0;
      virtual std::string GetExportedNameIdentifier (const UnicodeString& name) = 0;
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
					 
	std::string GetImportedNameIdentifier (const UnicodeString& name);
	std::string GetExportedNameIdentifier (const UnicodeString& name);
      };
      
      class CodegenVisitor : public intermediate::SequenceVisitor
      {
	friend class SequenceCodeGenerator;
	
	SequenceCodeGenerator* owner;
	StringsArrayPtr target;
	bool emitEmptyBlocks;
	
	void EmitAssign (const RegisterPtr& destination,
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
			
	void SetVisitedOp (const intermediate::SequenceOpPtr& op) {}
	
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
	void OpFunctionCall (const UnicodeString& funcIdent,
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
      
      std::string GetOutputRegisterName (const RegisterPtr& reg,
					 const std::string& initializer = std::string());
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
