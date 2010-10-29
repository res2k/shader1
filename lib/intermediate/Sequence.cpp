#include "base/common.h"

#include <boost/functional/hash.hpp>
#include <boost/make_shared.hpp>

#include "base/hash_UnicodeString.h"
#include "intermediate/Sequence.h"

#include <unicode/ustdio.h>

namespace s1
{
  namespace intermediate
  {
    Sequence::Register::Register (const UnicodeString& name)
     : originalName (name), generation (0), name (name)
    {}
    
    Sequence::Register::Register (const Sequence::Register& other)
     : originalName (other.originalName), generation (other.generation+1),
       name (other.originalName)
    {
      UChar generationSuffix[charsToFormatUint + 2];
      u_snprintf (generationSuffix, sizeof (generationSuffix)/sizeof (UChar),
		  ".%u", generation);
      name.append (generationSuffix);
    }
    
    Sequence::Register::Register (const Sequence::Register& other, int copyHack)
     : originalName (other.originalName), generation (other.generation),
       name (other.name)
    {
    }
    
    void Sequence::Register::StealName (Register& other)
    {
      originalName = other.originalName;
      name = other.name;
      generation = other.generation;
      other.originalName.insert (0, "STOLEN!");
      other.name.insert (0, "STOLEN!");
    }
    
    // ----------------------------------------------------------------------

    Sequence::RegisterBank::RegisterBank (const TypePtr& originalType)
     : originalType (originalType) {}
     
    Sequence::RegisterBank::RegisterBank (const RegisterBank& other)
     : originalType (other.originalType)
    {
      for (std::vector<RegisterPtr>::const_iterator reg = other.registers.begin();
	   reg != other.registers.end();
	   ++reg)
      {
	RegisterPtr newReg (boost::make_shared<Register> (**reg, 1));
	registers.push_back (newReg);
      }
    }

    unsigned int Sequence::RegisterBank::AddRegister (const UnicodeString& name)
    {
      unsigned int num = registers.size();
      registers.push_back (boost::make_shared<Register> (name));
      return num;
    }
    
    unsigned int Sequence::RegisterBank::AddRegister (const RegisterPtr& oldReg)
    {
      unsigned int num = registers.size();
      registers.push_back (boost::make_shared<Register> (*oldReg));
      return num;
    }
    
    // ----------------------------------------------------------------------
    
    void Sequence::AddOp (SequenceOpPtr op)
    {
      ops.push_back (op);
    }
    
    void Sequence::InsertOp (size_t before, SequenceOpPtr op)
    {
      ops.insert (ops.begin() + before, op);
    }
      
    void Sequence::Clear ()
    {
      ops.clear();
      registerBanks.clear();
      typeToRegBank.clear();
      identToRegID.clear();
      imports.clear();;
      exports.clear();;
    }
      
    RegisterID Sequence::AllocateRegister (const std::string& typeStr,
					   const TypePtr& originalType,
					   const UnicodeString& name)
    {
      unsigned int bank;
      TypeToRegBankType::iterator bankIt = typeToRegBank.find (typeStr);
      if (bankIt != typeToRegBank.end())
	bank = bankIt->second;
      else
      {
	bank = registerBanks.size();
	registerBanks.push_back (RegisterBankPtr (new RegisterBank (originalType)));
	typeToRegBank[typeStr] = bank;
      }
      RegisterBankPtr bankPtr = registerBanks[bank];
      
      unsigned int regNum = bankPtr->AddRegister (UnicodeString (name));
      return RegisterID (bank, regNum);
    }
    
    RegisterID Sequence::AllocateRegister (const RegisterID& oldReg)
    {
      if (!oldReg.IsValid()) return RegisterID ();
      
      RegisterBankPtr bankPtr = registerBanks[oldReg.bank];
      RegisterPtr regPtr = bankPtr->registers[oldReg.num];
      
      unsigned int regNum = bankPtr->AddRegister (regPtr);
      return RegisterID (oldReg.bank, regNum);
    }

    Sequence::RegisterPtr Sequence::QueryRegisterPtrFromID (const RegisterID& id, RegisterBankPtr& bank) const
    {
      if (!id.IsValid()) return RegisterPtr ();
      
      bank = registerBanks[id.bank];
      RegisterPtr regPtr = bank->registers[id.num];
      
      return regPtr;
    }
      
    void Sequence::SetIdentifierRegisterID (const UnicodeString& identifier, RegisterID regID)
    {
      identToRegID[identifier] = regID;
    }
    
    RegisterID Sequence::GetIdentifierRegisterID (const UnicodeString& identifier) const
    {
      IdentifierToRegIDMap::const_iterator regIt = identToRegID.find (identifier);
      if (regIt != identToRegID.end())
	return regIt->second;
      return RegisterID ();
    }
      
    void Sequence::CopyRegisterBanks (const SequencePtr& other)
    {
      registerBanks.clear();
      for (std::vector<RegisterBankPtr>::const_iterator regBank = other->registerBanks.begin();
	   regBank != other->registerBanks.end();
	   ++regBank)
      {
	RegisterBankPtr newBank = boost::make_shared<RegisterBank> (**regBank);
	registerBanks.push_back (newBank);
      }
    }
      
    void Sequence::Visit (SequenceVisitor& visitor) const
    {
      for (OpsVector::const_iterator op = ops.begin(); op != ops.end(); ++op)
      {
	(*op)->Visit (visitor);
      }
    }
    
    void Sequence::AddImport (const UnicodeString& parentRegName,
			      const RegisterID& localID)
    {
      imports.push_back (std::make_pair (parentRegName, localID));
    }
    
    void Sequence::SetExport (const UnicodeString& parentRegName,
			      const RegisterID& localID)
    {
      exports[parentRegName] = localID;
    }
    
    void Sequence::CleanUnusedImportsExports ()
    {
      RegisterIDSet allReadRegisters, allWrittenRegisters;
      for (OpsVector::const_iterator op = ops.begin(); op != ops.end(); ++op)
      {
	const RegisterIDSet& opRead = (*op)->GetReadRegisters();
	allReadRegisters.insert (opRead.begin(), opRead.end());
	const RegisterIDSet& opWritten = (*op)->GetWrittenRegisters();
	allWrittenRegisters.insert (opWritten.begin(), opWritten.end());
      }
      
      {
	RegisterImpMappings::iterator it = imports.begin();
	while (it != imports.end())
	{
	  if (allReadRegisters.count (it->second) == 0)
	    it = imports.erase (it);
	  else
	    ++it;
	}
      }
      {
	RegisterExpMappings::iterator it = exports.begin();
	while (it != exports.end())
	{
	  if (allWrittenRegisters.count (it->second) == 0)
	    it = exports.erase (it);
	  else
	    ++it;
	}
      }
    }

  } // namespace intermediate
} // namespace s1

