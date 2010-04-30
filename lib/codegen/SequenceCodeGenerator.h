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
    class CgGenerator::SequenceCodeGenerator
    {
    protected:
      typedef intermediate::Sequence Sequence;
      class CodegenVisitor : public intermediate::SequenceVisitor
      {
	SequenceCodeGenerator* owner;
	StringsArrayPtr target;
	
	void EmitAssign (const Sequence::RegisterID& destination,
			 const char* value);
	void EmitFunctionCall (const Sequence::RegisterID& destination,
			       const char* function,
			       const char* paramsStr);
	void EmitBinary (const Sequence::RegisterID& destination,
			 const Sequence::RegisterID& source1,
			 const Sequence::RegisterID& source2,
			 const char* op);
      public:
	CodegenVisitor (SequenceCodeGenerator* owner,
			const StringsArrayPtr& target);
	
	void OpConstBool (const Sequence::RegisterID& destination,
			  bool value);
	void OpConstInt (const Sequence::RegisterID& destination,
			 int value);
	void OpConstUInt (const Sequence::RegisterID& destination,
			  unsigned int value);
	void OpConstFloat (const Sequence::RegisterID& destination,
			   float value);
				  
	void OpAssign (const Sequence::RegisterID& destination,
		       const Sequence::RegisterID& source);
				  
	void OpCastToBool (const Sequence::RegisterID& destination,
			   const Sequence::RegisterID& source);
	void OpCastToInt (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source);
	void OpCastToUInt (const Sequence::RegisterID& destination,
			   const Sequence::RegisterID& source);
	void OpCastToFloat (const Sequence::RegisterID& destination,
			    const Sequence::RegisterID& source);

	void OpArithAdd (const Sequence::RegisterID& destination,
			 const Sequence::RegisterID& source1,
			 const Sequence::RegisterID& source2);
	void OpArithSub (const Sequence::RegisterID& destination,
			 const Sequence::RegisterID& source1,
			 const Sequence::RegisterID& source2);
	void OpArithMul (const Sequence::RegisterID& destination,
			 const Sequence::RegisterID& source1,
			 const Sequence::RegisterID& source2);
	void OpArithDiv (const Sequence::RegisterID& destination,
			 const Sequence::RegisterID& source1,
			 const Sequence::RegisterID& source2);
	void OpArithMod (const Sequence::RegisterID& destination,
			 const Sequence::RegisterID& source1,
			 const Sequence::RegisterID& source2);

	void OpCompareEq (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source1,
			  const Sequence::RegisterID& source2);
	void OpCompareNE (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source1,
			  const Sequence::RegisterID& source2);
	void OpCompareLT (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source1,
			  const Sequence::RegisterID& source2);
	void OpCompareLE (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source1,
			  const Sequence::RegisterID& source2);
	void OpCompareGT (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source1,
			  const Sequence::RegisterID& source2);
	void OpCompareGE (const Sequence::RegisterID& destination,
			  const Sequence::RegisterID& source1,
			  const Sequence::RegisterID& source2);
      };
      
      const intermediate::Sequence& seq;
      StringsArrayPtr strings;
      
      typedef boost::unordered_map<Sequence::RegisterID, std::string> RegistersToIDMap;
      RegistersToIDMap seenRegisters;
      
      std::string TypeToCgType (const parser::SemanticsHandler::TypePtr& type);
      std::string RegisterNameToCgIdentifier (const UnicodeString& str);
      std::string GetOutputRegisterName (const Sequence::RegisterID& reg,
					 bool autoAllocate = true);
    public:
      SequenceCodeGenerator (const intermediate::Sequence& seq);
      
      StringsArrayPtr Generate ();
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_SEQUENCECODEGENERATOR_H__
