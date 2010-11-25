#include "base/common.h"

#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>
#include <boost/make_shared.hpp>

#include "intermediate/Sequence.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "intermediate/SequenceVisitor.h"

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
      identToReg.clear();
      imports.clear();;
      exports.clear();;
    }

    RegisterPtr Sequence::AllocateRegister (const TypePtr& originalType,
					    const UnicodeString& name)
    {
      RegisterPtr reg (boost::make_shared<Register> (name, originalType));
      return reg;
    }
    
    RegisterPtr Sequence::AllocateRegister (const RegisterPtr& oldReg)
    {
      if (!oldReg) return RegisterPtr ();
      
      RegisterPtr reg (boost::make_shared<Register> (*oldReg));
      return reg;
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

    void Sequence::SetIdentifierRegisters (const IdentifierToRegMap& map)
    {
      BOOST_FOREACH(const IdentRegPair& i2r, map)
      {
	identToReg[i2r.first] = i2r.second;
      }
    }
      
    void Sequence::Visit (SequenceVisitor& visitor) const
    {
      for (OpsVector::const_iterator op = ops.begin(); op != ops.end(); ++op)
      {
	visitor.SetVisitedOp (*op);
	(*op)->Visit (visitor);
      }
      visitor.SetVisitedOp (SequenceOpPtr ());
    }
    
    void Sequence::ReverseVisit (SequenceVisitor& visitor) const
    {
      BOOST_REVERSE_FOREACH(const SequenceOpPtr& op, ops)
      {
	visitor.SetVisitedOp (op);
	op->Visit (visitor);
      }
      visitor.SetVisitedOp (SequenceOpPtr ());
    }
    
    void Sequence::AddImport (const UnicodeString& parentRegName,
			      const RegisterPtr& local)
    {
      imports.push_back (std::make_pair (parentRegName, local));
    }
    
    void Sequence::AddImports (const RegisterImpMappings& imports)
    {
      BOOST_FOREACH(const IdentRegPair& imp, imports)
      {
	this->imports.push_back (imp);
      }
    }
    
    void Sequence::SetExport (const UnicodeString& parentRegName,
			      const RegisterPtr& local)
    {
      exports[parentRegName] = local;
    }
    
    void Sequence::AddExports (const RegisterExpMappings& exports)
    {
      BOOST_FOREACH(const IdentRegPair& exp, exports)
      {
	this->exports[exp.first] = exp.second;
      }
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
    
    RegisterSet Sequence::GetImportOuterRegs (const IdentifierToRegMap& identToReg)
    {
      RegisterSet importRegs;
      
      RegisterImpMappings::iterator it = imports.begin();
      while (it != imports.end())
      {
	IdentifierToRegMap::const_iterator outerReg = identToReg.find (it->first);
	if (outerReg != identToReg.end())
	{
	  importRegs.insert (outerReg->second);
	}
	++it;
      }
      return importRegs;
    }
    
    RegisterSet Sequence::GetExportOuterRegs (const IdentifierToRegMap& identToReg)
    {
      RegisterSet exportRegs;
      
      RegisterExpMappings::iterator it = exports.begin();
      while (it != exports.end())
      {
	IdentifierToRegMap::const_iterator outerReg = identToReg.find (it->first);
	assert (outerReg != identToReg.end());
	exportRegs.insert (outerReg->second);
	++it;
      }
      return exportRegs;
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

