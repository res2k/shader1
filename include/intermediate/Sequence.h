#ifndef __INTERMEDIATE_SEQUENCE_H__
#define __INTERMEDIATE_SEQUENCE_H__

#include "forwarddecl.h"
#include "RegisterID.h"
#include "SequenceOp/SequenceOp.h"
#include "parser/SemanticsHandler.h"

#include "base/unordered_map"
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
      public:
	Register (const UnicodeString& name);
	Register (const Register& other);
	Register (const Register& other, int copyHack);
	
	const UnicodeString& GetName() const { return name; }
	void StealName (Register& other);
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
	RegisterBank (const RegisterBank& other);
	
	const TypePtr& GetOriginalType () const { return originalType; }
      };
      typedef boost::shared_ptr<RegisterBank> RegisterBankPtr;
      
      /** \name Sequence manipulation
       * @{ */
      /// Add an operation
      void AddOp (SequenceOpPtr op);
      /// Insert an operation
      void InsertOp (size_t before, SequenceOpPtr op);
      /// Get number of operations
      size_t GetNumOps() const { return ops.size(); }
      /// Get a specific op
      SequenceOpPtr GetOp(size_t index) const { return ops[index]; }
      /// Clear sequence
      void Clear ();
      /** @} */
      
      RegisterID AllocateRegister (const std::string& typeStr,
				   const TypePtr& originalType,
				   const UnicodeString& name);
      RegisterID AllocateRegister (const RegisterID& oldReg);
      
      RegisterPtr QueryRegisterPtrFromID (const RegisterID& id, RegisterBankPtr& bank) const;
      RegisterPtr QueryRegisterPtrFromID (const RegisterID& id) const
      { RegisterBankPtr dummyBank; return QueryRegisterPtrFromID (id, dummyBank); }
      
      void SetIdentifierRegisterID (const UnicodeString& identifier, RegisterID regID);
      RegisterID GetIdentifierRegisterID (const UnicodeString& identifier) const;
      
      typedef std::tr1::unordered_map<UnicodeString, RegisterID> IdentifierToRegIDMap;
      /// Get current identifiers-to-register-ID map
      const IdentifierToRegIDMap& GetIdentifierToRegisterIDMap () const
      { return identToRegID; }
      
      /// Deep copy register banks setup from another sequence
      void CopyRegisterBanks (const SequencePtr& other);
      
      void Visit (SequenceVisitor& visitor) const;
      
      typedef std::vector<std::pair<UnicodeString, RegisterID> > RegisterImpMappings;
      const RegisterImpMappings& GetImports () const { return imports; }
      void AddImport (const UnicodeString& parentRegName,
		      const RegisterID& localID);
      typedef std::tr1::unordered_map<UnicodeString, RegisterID> RegisterExpMappings;
      const RegisterExpMappings& GetExports () const { return exports; }
      RegisterExpMappings& GetExports () { return exports; }
      void SetExport (const UnicodeString& parentRegName,
		      const RegisterID& localID);
		      
      void CleanUnusedImportsExports ();
    protected:
      std::vector<RegisterBankPtr> registerBanks;
      typedef std::tr1::unordered_map<std::string, unsigned int> TypeToRegBankType;
      TypeToRegBankType typeToRegBank;
      IdentifierToRegIDMap identToRegID;
      
      RegisterImpMappings imports;
      RegisterExpMappings exports;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCE_H__
