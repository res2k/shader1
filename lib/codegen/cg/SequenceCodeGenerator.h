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
      typedef intermediate::RegisterID RegisterID;
      typedef intermediate::Sequence Sequence;
      
      class SequenceIdentifiersToRegIDsNameResolver : public ImportedNameResolver
      {
	SequenceCodeGenerator* owner;
	const Sequence::IdentifierToRegIDMap& identToRegID_imp;
	const Sequence::IdentifierToRegIDMap& identToRegID_exp;
      public:
	SequenceIdentifiersToRegIDsNameResolver (SequenceCodeGenerator* owner,
						 const Sequence::IdentifierToRegIDMap& identToRegID_imp,
						 const Sequence::IdentifierToRegIDMap& identToRegID_exp);
					 
	std::string GetImportedNameIdentifier (const UnicodeString& name);
	std::string GetExportedNameIdentifier (const UnicodeString& name);
      };
      
      class CodegenVisitor : public intermediate::SequenceVisitor
      {
	friend class SequenceCodeGenerator;
	
	SequenceCodeGenerator* owner;
	StringsArrayPtr target;
	
	void EmitAssign (const RegisterID& destination,
			 const char* value);
	void EmitAssign (const char* destination,
			 const RegisterID& source);
	void EmitFunctionCall (const RegisterID& destination,
			       const char* function,
			       const char* paramsStr);
	void EmitBinary (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2,
			 const char* op);
	void EmitUnary (const RegisterID& destination,
			const RegisterID& source,
			const char* op);
      public:
	CodegenVisitor (SequenceCodeGenerator* owner,
			const StringsArrayPtr& target);
	
	void OpConstBool (const RegisterID& destination,
			  bool value);
	void OpConstInt (const RegisterID& destination,
			 int value);
	void OpConstUInt (const RegisterID& destination,
			  unsigned int value);
	void OpConstFloat (const RegisterID& destination,
			   float value);
				  
	void OpAssign (const RegisterID& destination,
		       const RegisterID& source);
				  
	void OpCast (const RegisterID& destination,
		     BaseType destType,
		     const RegisterID& source);

	void OpMakeVector (const RegisterID& destination,
			   BaseType compType,
			   const std::vector<RegisterID>& sources);
				     
	void OpMakeMatrix (const RegisterID& destination,
			   BaseType compType,
			   unsigned int matrixRows, unsigned int matrixCols,
			   const std::vector<RegisterID>& sources);
				     
	void OpMakeArray (const RegisterID& destination,
			  const std::vector<RegisterID>& sources);
	void OpExtractArrayElement (const RegisterID& destination,
				    const RegisterID& source,
				    const RegisterID& index);
	void OpChangeArrayElement (const RegisterID& destination,
				   const RegisterID& source,
				   const RegisterID& index,
				   const RegisterID& newValue);
	void OpGetArrayLength (const RegisterID& destination,
			       const RegisterID& array);

	void OpExtractVectorComponent (const RegisterID& destination,
				       const RegisterID& source,
				       unsigned int comp);
				      
	void OpArith (const RegisterID& destination,
		      ArithmeticOp op,
		      const RegisterID& source1,
		      const RegisterID& source2);

	void OpLogic (const RegisterID& destination,
		      LogicOp op,
		      const RegisterID& source1,
		      const RegisterID& source2);

	void OpUnary (const RegisterID& destination,
		      UnaryOp op,
		      const RegisterID& source);
			       
	void OpCompare (const RegisterID& destination,
			CompareOp op,
			const RegisterID& source1,
			const RegisterID& source2);
			  
	void OpBlock (const intermediate::SequencePtr& seq,
		      const Sequence::IdentifierToRegIDMap& identToRegID_imp,
		      const Sequence::IdentifierToRegIDMap& identToRegID_exp,
		      const std::vector<RegisterID>& writtenRegisters);
		      
	void OpBranch (const RegisterID& conditionReg,
		       const intermediate::SequenceOpPtr& seqOpIf,
		       const intermediate::SequenceOpPtr& seqOpElse);
	void OpWhile (const RegisterID& conditionReg,
		      const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
		      const intermediate::SequenceOpPtr& seqOpBody);
		      
	void OpReturn (const RegisterID& retValReg);
	void OpFunctionCall (const RegisterID& destination,
			     const UnicodeString& funcIdent,
			     const std::vector<RegisterID>& inParams,
			     const std::vector<RegisterID>& outParams);
	void OpBuiltinCall (const RegisterID& destination,
			    intermediate::BuiltinFunction what,
			    const std::vector<RegisterID>& inParams);
      };
      
      const intermediate::Sequence& seq;
      ImportedNameResolver* nameRes;
      const intermediate::ProgramFunction::TransferMappings& transferIn;
      const intermediate::ProgramFunction::TransferMappings& transferOut;
      StringsArrayPtr strings;
      
      typedef boost::unordered_map<RegisterID, std::string> RegistersToIDMap;
      RegistersToIDMap seenRegisters;
      
      std::string GetOutputRegisterName (const RegisterID& reg,
					 const std::string& initializer = std::string());
    public:
      SequenceCodeGenerator (const intermediate::Sequence& seq,
			     ImportedNameResolver* nameRes,
			     const intermediate::ProgramFunction::TransferMappings& transferIn,
			     const intermediate::ProgramFunction::TransferMappings& transferOut);
      
      StringsArrayPtr Generate ();
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SEQUENCECODEGENERATOR_H__
