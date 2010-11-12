#include "base/common.h"

#include <boost/functional/hash.hpp>
#include <boost/make_shared.hpp>

#include "base/hash_UnicodeString.h"
#include "intermediate/Sequence.h"
#include "intermediate/SequenceOp/SequenceOp.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include <unicode/ustdio.h>

namespace s1
{
  namespace intermediate
  {
    Sequence::Register::Register (const UnicodeString& name, const TypePtr& originalType)
     : originalName (name), generation (0), name (name), originalType (originalType)
    {}
    
    Sequence::Register::Register (const Sequence::Register& other)
     : originalName (other.originalName), generation (other.generation+1),
       name (other.originalName), originalType (other.originalType)
    {
      UChar generationSuffix[charsToFormatUint + 2];
      u_snprintf (generationSuffix, sizeof (generationSuffix)/sizeof (UChar),
		  ".%u", generation);
      name.append (generationSuffix);
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
     
    RegisterPtr Sequence::RegisterBank::AddRegister (const UnicodeString& name)
    {
      RegisterPtr reg (boost::make_shared<Register> (name, originalType));
      registers.push_back (reg);
      return reg;
    }
    
    RegisterPtr Sequence::RegisterBank::AddRegister (const RegisterPtr& oldReg)
    {
      RegisterPtr reg (boost::make_shared<Register> (*oldReg));
      registers.push_back (reg);
      return reg;
    }
    
    void Sequence::RegisterBank::TrackRegister (const RegisterPtr& reg)
    {
      registers.push_back (reg);
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
      identToReg.clear();
      imports.clear();;
      exports.clear();;
    }

    Sequence::RegisterBankPtr Sequence::GetRegisterBank (const TypePtr& originalType)
    {
      std::string typeStr (IntermediateGeneratorSemanticsHandler::GetTypeString (originalType));
      
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
      return registerBanks[bank];
    }
      
    RegisterPtr Sequence::AllocateRegister (const TypePtr& originalType,
					    const UnicodeString& name)
    {
      RegisterBankPtr bankPtr = GetRegisterBank (originalType);
      return bankPtr->AddRegister (UnicodeString (name));
    }
    
    RegisterPtr Sequence::AllocateRegister (const RegisterPtr& oldReg)
    {
      if (!oldReg) return RegisterPtr ();
      
      RegisterBankPtr bankPtr = GetRegisterBank (oldReg->GetOriginalType());
      
      return bankPtr->AddRegister (oldReg);
    }

    void Sequence::TrackRegister (const RegisterPtr& reg)
    {
      RegisterBankPtr bankPtr = GetRegisterBank (reg->GetOriginalType());
      return bankPtr->TrackRegister (reg);
    }

    void Sequence::SetIdentifierRegister (const UnicodeString& identifier, const RegisterPtr& reg)
    {
      identToReg[identifier] = reg;
    }
    
    RegisterPtr Sequence::GetIdentifierRegister (const UnicodeString& identifier) const
    {
      IdentifierToRegMap::const_iterator regIt = identToReg.find (identifier);
      if (regIt != identToReg.end())
	return regIt->second;
      return RegisterPtr ();
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
			      const RegisterPtr& local)
    {
      imports.push_back (std::make_pair (parentRegName, local));
    }
    
    void Sequence::SetExport (const UnicodeString& parentRegName,
			      const RegisterPtr& local)
    {
      exports[parentRegName] = local;
    }
    
    RegisterSet Sequence::GetAllReadRegisters() const
    {
      RegisterSet allReadRegisters;
      for (OpsVector::const_iterator op = ops.begin(); op != ops.end(); ++op)
      {
	const RegisterSet& opRead = (*op)->GetReadRegisters();
	allReadRegisters.insert (opRead.begin(), opRead.end());
      }
      return allReadRegisters;
    }
    
    RegisterSet Sequence::GetAllWrittenRegisters() const
    {
      RegisterSet allWrittenRegisters;
      for (OpsVector::const_iterator op = ops.begin(); op != ops.end(); ++op)
      {
	const RegisterSet& opWritten = (*op)->GetWrittenRegisters();
	allWrittenRegisters.insert (opWritten.begin(), opWritten.end());
      }
      return allWrittenRegisters;
    }
    
    void Sequence::CleanUnusedImportsExports ()
    {
      RegisterSet allReadRegisters (GetAllReadRegisters ());
      RegisterSet allWrittenRegisters (GetAllWrittenRegisters ());
      
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

