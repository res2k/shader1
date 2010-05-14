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
    };
    
    class CgGenerator::SequenceCodeGenerator
    {
    protected:
      typedef intermediate::RegisterID RegisterID;
      typedef intermediate::Sequence Sequence;
      
      class SequenceIdentifiersToRegIDsNameResolver : public ImportedNameResolver
      {
	SequenceCodeGenerator* owner;
	const Sequence::IdentifierToRegIDMap& identToRegID;
      public:
	SequenceIdentifiersToRegIDsNameResolver (SequenceCodeGenerator* owner,
						 const Sequence::IdentifierToRegIDMap& identToRegID);
					 
	std::string GetImportedNameIdentifier (const UnicodeString& name);
      };
      
      class CodegenVisitor : public intermediate::SequenceVisitor
      {
	friend class SequenceCodeGenerator;
	
	SequenceCodeGenerator* owner;
	StringsArrayPtr target;
	
	void EmitAssign (const RegisterID& destination,
			 const char* value);
	void EmitFunctionCall (const RegisterID& destination,
			       const char* function,
			       const char* paramsStr);
	void EmitBinary (const RegisterID& destination,
			 const RegisterID& source1,
			 const RegisterID& source2,
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
		      const Sequence::IdentifierToRegIDMap& identToRegID);
      };
      
      const intermediate::Sequence& seq;
      ImportedNameResolver* nameRes;
      StringsArrayPtr strings;
      
      typedef boost::unordered_map<RegisterID, std::string> RegistersToIDMap;
      RegistersToIDMap seenRegisters;
      
      std::string TypeToCgType (const parser::SemanticsHandler::TypePtr& type);
      std::string RegisterNameToCgIdentifier (const UnicodeString& str);
      std::string GetOutputRegisterName (const RegisterID& reg,
					 bool autoAllocate = true);
    public:
      SequenceCodeGenerator (const intermediate::Sequence& seq,
			     ImportedNameResolver* nameRes);
      
      StringsArrayPtr Generate ();
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SEQUENCECODEGENERATOR_H__
