#ifndef __CODEGEN_SEQUENCECODEGENERATOR_H__
#define __CODEGEN_SEQUENCECODEGENERATOR_H__

#include "codegen/CgGenerator.h"
#include "codegen/StringsArray.h"
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
				  
	void OpCastToBool (const RegisterID& destination,
			   const RegisterID& source);
	void OpCastToInt (const RegisterID& destination,
			  const RegisterID& source);
	void OpCastToUInt (const RegisterID& destination,
			   const RegisterID& source);
	void OpCastToFloat (const RegisterID& destination,
			    const RegisterID& source);

	void OpMakeVectorBool (const RegisterID& destination,
			       const std::vector<RegisterID>& sources);
	void OpMakeVectorInt (const RegisterID& destination,
			      const std::vector<RegisterID>& sources);
	void OpMakeVectorUInt (const RegisterID& destination,
			       const std::vector<RegisterID>& sources);
	void OpMakeVectorFloat (const RegisterID& destination,
				const std::vector<RegisterID>& sources);
				     
	void OpMakeMatrixBool (const RegisterID& destination,
			       unsigned int matrixRows, unsigned int matrixCols,
			       const std::vector<RegisterID>& sources);
	void OpMakeMatrixInt (const RegisterID& destination,
			      unsigned int matrixRows, unsigned int matrixCols,
			      const std::vector<RegisterID>& sources);
	void OpMakeMatrixUInt (const RegisterID& destination,
			       unsigned int matrixRows, unsigned int matrixCols,
			       const std::vector<RegisterID>& sources);
	void OpMakeMatrixFloat (const RegisterID& destination,
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
				      
	void OpArithAdd (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2);
	void OpArithSub (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2);
	void OpArithMul (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2);
	void OpArithDiv (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2);
	void OpArithMod (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2);

	void OpLogicAnd (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2);
	void OpLogicOr (const RegisterID& destination,
			const RegisterID& source1,
			const RegisterID& source2);

	void OpUnaryInv (const RegisterID& destination,
			 const RegisterID& source);
	void OpUnaryNeg (const RegisterID& destination,
			 const RegisterID& source);
	void OpUnaryNot (const RegisterID& destination,
			 const RegisterID& source);
			       
	void OpCompareEq (const RegisterID& destination,
			  const RegisterID& source1,
			  const RegisterID& source2);
	void OpCompareNE (const RegisterID& destination,
			  const RegisterID& source1,
			  const RegisterID& source2);
	void OpCompareLT (const RegisterID& destination,
			  const RegisterID& source1,
			  const RegisterID& source2);
	void OpCompareLE (const RegisterID& destination,
			  const RegisterID& source1,
			  const RegisterID& source2);
	void OpCompareGT (const RegisterID& destination,
			  const RegisterID& source1,
			  const RegisterID& source2);
	void OpCompareGE (const RegisterID& destination,
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
      StringsArrayPtr strings;
      
      typedef boost::unordered_map<RegisterID, std::string> RegistersToIDMap;
      RegistersToIDMap seenRegisters;
      
      std::string GetOutputRegisterName (const RegisterID& reg,
					 const std::string& initializer = std::string());
    public:
      SequenceCodeGenerator (const intermediate::Sequence& seq,
			     ImportedNameResolver* nameRes);
      
      StringsArrayPtr Generate ();
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SEQUENCECODEGENERATOR_H__
