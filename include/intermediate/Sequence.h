#ifndef __INTERMEDIATE_SEQUENCE_H__
#define __INTERMEDIATE_SEQUENCE_H__

#include "forwarddecl.h"
#include "parser/SemanticsHandler.h"

#include <boost/unordered_set.hpp>
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
	
	TypePtr originalType;
      public:
	Register (const UnicodeString& name, const TypePtr& originalType);
	Register (const Register& other);
	
	const UnicodeString& GetName() const { return name; }
	void StealName (Register& other);
	
	const TypePtr& GetOriginalType () const { return originalType; }
      };
      typedef boost::shared_ptr<Register> RegisterPtr;
      typedef boost::unordered_set<RegisterPtr> RegisterSet;
      
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
      
      RegisterPtr AllocateRegister (const TypePtr& originalType,
				    const UnicodeString& name);
      RegisterPtr AllocateRegister (const RegisterPtr& oldReg);
      
      void SetIdentifierRegister (const UnicodeString& identifier, const RegisterPtr& regID);
      RegisterPtr GetIdentifierRegister (const UnicodeString& identifier) const;
      
      typedef std::tr1::unordered_map<UnicodeString, RegisterPtr> IdentifierToRegMap;
      /// Get current identifiers-to-register map
      const IdentifierToRegMap& GetIdentifierToRegisterMap () const
      { return identToReg; }
      
      void Visit (SequenceVisitor& visitor) const;
      
      typedef std::vector<std::pair<UnicodeString, RegisterPtr> > RegisterImpMappings;
      const RegisterImpMappings& GetImports () const { return imports; }
      void AddImport (const UnicodeString& parentRegName,
		      const RegisterPtr& localReg);
      typedef std::tr1::unordered_map<UnicodeString, RegisterPtr> RegisterExpMappings;
      const RegisterExpMappings& GetExports () const { return exports; }
      RegisterExpMappings& GetExports () { return exports; }
      void SetExport (const UnicodeString& parentRegName,
		      const RegisterPtr& localReg);

      RegisterSet GetAllReadRegisters() const;
      RegisterSet GetAllWrittenRegisters() const;
		      
      void CleanUnusedImportsExports ();
    protected:
      IdentifierToRegMap identToReg;
      
      RegisterImpMappings imports;
      RegisterExpMappings exports;
    };
    
    typedef Sequence::RegisterPtr RegisterPtr;
    typedef Sequence::RegisterSet RegisterSet;
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCE_H__
