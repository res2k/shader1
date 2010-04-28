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
      /* (sizeof(x)*25)/10+1 is an approximation of the number of characters
       * needed to display x in decimal system. (x can be at most 256^sizeof(x).
       * You need log10(256^sizeof(x)) characters, becoming
       * sizeof(x)*log10(256). 25/10 is an (over-)approximation of log10(256).)
       * Add 1 for null terminator, add 1 for dot */
      UChar generationSuffix[(sizeof(unsigned int) * 25) / 10 + 2];
      u_snprintf (generationSuffix, sizeof (generationSuffix)/sizeof (UChar),
		  ".%u", generation);
      name.append (generationSuffix);
    }
    
    // ----------------------------------------------------------------------

    Sequence::RegisterBank::RegisterBank (OriginalTypePtr originalType)
     : originalType (originalType) {}

    unsigned int Sequence::RegisterBank::AddRegister (const UnicodeString& name)
    {
      unsigned int num = registers.size();
      registers.push_back (RegisterPtr (new Register (name)));
      return num;
    }
    
    unsigned int Sequence::RegisterBank::AddRegister (const RegisterPtr& oldReg)
    {
      unsigned int num = registers.size();
      registers.push_back (RegisterPtr (new Register (*oldReg)));
      return num;
    }
    
    // ----------------------------------------------------------------------
    
    void Sequence::AddOp (SequenceOpPtr op)
    {
      ops.push_back (op);
    }
      
    Sequence::RegisterID Sequence::AllocateRegister (const std::string& typeStr,
						     const OriginalTypePtr& originalType,
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
    
    Sequence::RegisterID Sequence::AllocateRegister (const RegisterID& oldReg)
    {
      if (!oldReg.IsValid()) return RegisterID ();
      
      RegisterBankPtr bankPtr = registerBanks[oldReg.bank];
      RegisterPtr regPtr = bankPtr->registers[oldReg.num];
      
      unsigned int regNum = bankPtr->AddRegister (regPtr);
      return RegisterID (oldReg.bank, regNum);
    }
      
    void Sequence::Visit (SequenceVisitor& visitor) const
    {
      for (OpsVector::const_iterator op = ops.begin(); op != ops.end(); ++op)
      {
	(*op)->Visit (visitor);
      }
    }
  } // namespace intermediate
} // namespace s1

