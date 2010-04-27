#ifndef __INTERMEDIATE_SEQUENCE_H__
#define __INTERMEDIATE_SEQUENCE_H__

#include "SequenceOp/SequenceOp.h"

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
      
      struct OriginalType
      {
	virtual ~OriginalType() {}
      };
      typedef boost::shared_ptr<OriginalType> OriginalTypePtr;
      
      class Register
      {
	friend class Sequence;
	
	UnicodeString originalName;
	unsigned int generation;
	UnicodeString name;
	
	Register (const UnicodeString& name);
	Register (const Register& other);
      };
      typedef boost::shared_ptr<Register> RegisterPtr;
      
      class RegisterBank
      {
	friend class Sequence;
	
	OriginalTypePtr originalType;
	
	std::vector<RegisterPtr> registers;
	
	unsigned int AddRegister (const UnicodeString& name);
	unsigned int AddRegister (const RegisterPtr& oldReg);
      public:
	RegisterBank (OriginalTypePtr originalType);
      };
      typedef boost::shared_ptr<RegisterBank> RegisterBankPtr;
      
      struct RegisterID
      {
	unsigned int bank;
	unsigned int num;
	
	RegisterID () : bank (~0), num (~0) {}
	RegisterID (unsigned int bank, unsigned int num) : bank (bank), num (num) {}
	
	bool IsValid() const
	{ return (bank != (unsigned int)~0) && (num != (unsigned int)~0); }
	
	bool operator== (const RegisterID& other) const
	{ return (bank == other.bank) && (num == other.num); }
	bool operator!= (const RegisterID& other) const
	{ return (bank != other.bank) || (num != other.num); }
      };
      
      void AddOp (SequenceOpPtr op);
      
      size_t GetNumOps() const { return ops.size(); }
      
      RegisterID AllocateRegister (const std::string& typeStr,
				   const OriginalTypePtr& originalType,
				   const UnicodeString& name);
      RegisterID AllocateRegister (const RegisterID& oldReg);
      
      void Visit (SequenceVisitor& visitor);
    protected:
      std::vector<RegisterBankPtr> registerBanks;
      typedef std::tr1::unordered_map<std::string, unsigned int> TypeToRegBankType;
      TypeToRegBankType typeToRegBank;
    };
    typedef boost::shared_ptr<Sequence> SequencePtr;
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCE_H__
