#ifndef __INTERMEDIATE_SEQUENCE_H__
#define __INTERMEDIATE_SEQUENCE_H__

#include "RegisterID.h"
#include "SequenceOp/SequenceOp.h"
#include "parser/SemanticsHandler.h"

#include <base/unordered_map>
#include <unicode/unistr.h>
#include <vector>

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor;
    
    class Sequence
    {
      typedef std::vector<SequenceOpPtr> OpsVector;
      OpsVector ops;
    public:
      typedef parser::SemanticsHandler::TypePtr TypePtr;
      
      class Register
      {
      protected:
	friend class Sequence;
	
	UnicodeString originalName;
	unsigned int generation;
	UnicodeString name;
	
	Register (const UnicodeString& name);
	Register (const Register& other);
      public:
	const UnicodeString& GetName() const { return name; }
      };
      typedef boost::shared_ptr<Register> RegisterPtr;
      
      class RegisterBank
      {
	friend class Sequence;
	
	TypePtr originalType;
	
	std::vector<RegisterPtr> registers;
	
	unsigned int AddRegister (const UnicodeString& name);
	unsigned int AddRegister (const RegisterPtr& oldReg);
      public:
	RegisterBank (const TypePtr& originalType);
	
	const TypePtr& GetOriginalType () const { return originalType; }
      };
      typedef boost::shared_ptr<RegisterBank> RegisterBankPtr;
      
      void AddOp (SequenceOpPtr op);
      
      size_t GetNumOps() const { return ops.size(); }
      
      RegisterID AllocateRegister (const std::string& typeStr,
				   const TypePtr& originalType,
				   const UnicodeString& name);
      RegisterID AllocateRegister (const RegisterID& oldReg);
      
      RegisterPtr QueryRegisterFromID (const RegisterID& id, RegisterBankPtr& bank) const;
      
      void Visit (SequenceVisitor& visitor) const;
    protected:
      std::vector<RegisterBankPtr> registerBanks;
      typedef std::tr1::unordered_map<std::string, unsigned int> TypeToRegBankType;
      TypeToRegBankType typeToRegBank;
    };
    typedef boost::shared_ptr<Sequence> SequencePtr;
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCE_H__
