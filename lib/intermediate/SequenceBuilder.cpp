/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include "base/common.h"

#include "intermediate/SequenceBuilder.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace intermediate
  {
    SequenceBuilder::SequenceBuilder ()
      : sequence (boost::make_shared<Sequence> ())
    {
    }
      
    const SequencePtr& SequenceBuilder::GetSequence()
    {
      return sequence;
    }

    void SequenceBuilder::AddOp (SequenceOpPtr op)
    {
      sequence->ops.push_back (op);
    }
    
    void SequenceBuilder::InsertOp (size_t before, SequenceOpPtr op)
    {
      sequence->ops.insert (sequence->ops.begin() + before, op);
    }

    size_t SequenceBuilder::GetNumOps() const
    {
      return sequence->ops.size();
    }
    
    SequenceOpPtr SequenceBuilder::GetOp(size_t index) const
    {
      return sequence->ops[index];
    }
      
    void SequenceBuilder::Clear ()
    {
      sequence->ops.clear();
      identToReg.clear();
      sequence->imports.clear();
      sequence->exports.clear();
    }

    RegisterPtr SequenceBuilder::AllocateRegister (const TypePtr& originalType,
                                                   const uc::String& name)
    {
      RegisterPtr reg (boost::make_shared<Register> (name, originalType));
      return reg;
    }
    
    RegisterPtr SequenceBuilder::AllocateRegister (const RegisterPtr& oldReg)
    {
      if (!oldReg) return RegisterPtr ();
      
      RegisterPtr reg (boost::make_shared<Register> (*oldReg));
      return reg;
    }

    void SequenceBuilder::SetIdentifierRegister (const uc::String& identifier, const RegisterPtr& reg)
    {
      identToReg[identifier] = reg;
    }
    
    RegisterPtr SequenceBuilder::GetIdentifierRegister (const uc::String& identifier) const
    {
      IdentifierToRegMap::const_iterator regIt = identToReg.find (identifier);
      if (regIt != identToReg.end())
        return regIt->second;
      return RegisterPtr ();
    }

    void SequenceBuilder::SetIdentifierRegisters (const IdentifierToRegMap& map)
    {
      for(const IdentRegPair& i2r : map)
      {
        identToReg[i2r.first] = i2r.second;
      }
    }
    
    const SequenceBuilder::IdentifierToRegMap& SequenceBuilder::GetIdentifierToRegisterMap () const
    {
      return identToReg;
    }

    void SequenceBuilder::SetImport (const RegisterPtr& localReg,
                                     const uc::String& parentRegName)
    {
      RegisterImpMappings::iterator it = sequence->imports.begin();
      while (it != sequence->imports.end())
      {
        if (it->second == localReg)
        {
          it->first = parentRegName;
          return;
        }
        ++it;
      }
        
      sequence->imports.push_back (std::make_pair (parentRegName, localReg));
    }
    
    void SequenceBuilder::AddImports (const RegisterImpMappings& imports)
    {
      for(const IdentRegPair& imp : imports)
      {
        SetImport (imp.second, imp.first);
      }
    }

    SequenceBuilder::RegisterExpMappings& SequenceBuilder::GetExports ()
    {
      return sequence->exports;
    }

    void SequenceBuilder::SetExport (const uc::String& parentRegName,
                                     const RegisterPtr& local)
    {
      sequence->exports[parentRegName] = local;
    }
    
    void SequenceBuilder::AddExports (const RegisterExpMappings& exports)
    {
      for(const IdentRegPair& exp : exports)
      {
        sequence->exports[exp.first] = exp.second;
      }
    }

    void SequenceBuilder::CleanUnusedImportsExports (const RegisterSet& keepImports,
                                                     const RegisterSet& keepExports)
    {
      RegisterSet allReadRegisters (sequence->GetAllReadRegisters ());
      RegisterSet allWrittenRegisters (sequence->GetAllWrittenRegisters ());
      
      // Forcibly kept import/export regs are treated like read/written
      allReadRegisters.insert (keepImports.begin(), keepImports.end());
      allWrittenRegisters.insert (keepExports.begin(), keepExports.end());
      // Import destinations are considered 'written'
      for(const IdentRegPair& imp : sequence->imports)
      {
        allWrittenRegisters.insert (imp.second);
      }
      // Export sources are considered 'read'
      for(const IdentRegPair& exp : sequence->exports)
      {
        allReadRegisters.insert (exp.second);
      }
      
      {
        RegisterImpMappings::iterator it = sequence->imports.begin();
        while (it != sequence->imports.end())
        {
          if (allReadRegisters.count (it->second) == 0)
            it = sequence->imports.erase (it);
          else
            ++it;
        }
      }
      {
        RegisterExpMappings::iterator it = sequence->exports.begin();
        while (it != sequence->exports.end())
        {
          if (allWrittenRegisters.count (it->second) == 0)
            it = sequence->exports.erase (it);
          else
            ++it;
        }
      }
    }

    RegisterSet SequenceBuilder::GetAllReadRegisters() const
    {
      return sequence->GetAllReadRegisters();
    }

    RegisterSet SequenceBuilder::GetAllWrittenRegisters() const
    {
      return sequence->GetAllWrittenRegisters();
    }
  } // namespace intermediate
} // namespace s1
