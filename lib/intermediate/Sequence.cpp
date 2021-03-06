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

#include <boost/functional/hash.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "intermediate/Sequence.h"
#include "intermediate/SequenceOp/SequenceOp.h"
#include "intermediate/SequenceVisitor.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

#include "base/format/Formatter.ipp"

namespace s1
{
  namespace intermediate
  {
    DECLARE_STATIC_FORMATTER(FormatRegGeneration, "{0}.{1}");

    Sequence::Register::Register (const uc::String& name, const TypePtr& originalType)
     : originalName (name), generation (0), name (name), originalType (originalType)
    {}
    
    Sequence::Register::Register (const Sequence::Register& other)
     : originalName (other.originalName), generation (other.generation+1),
       originalType (other.originalType)
    {
      FormatRegGeneration (name, other.originalName, generation);
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
    
    size_t Sequence::GetNumOps() const
    {
      return ops.size();
    }

    SequenceOpPtr Sequence::GetOp(size_t index) const
    {
      return ops[index];
    }

    void Sequence::Visit (SequenceVisitor& visitor) const
    {
      for(const SequenceOpPtr& op : ops)
      {
        visitor.PreVisitOp (op);
        op->Visit (visitor);
        visitor.PostVisitOp ();
      }
      visitor.VisitEnd ();
    }
    
    void Sequence::ReverseVisit (SequenceVisitor& visitor) const
    {
      for(const SequenceOpPtr& op : boost::adaptors::reverse(ops))
      {
        visitor.PreVisitOp (op);
        op->Visit (visitor);
        visitor.PostVisitOp ();
      }
      visitor.VisitEnd ();
    }

    const Sequence::RegisterExpMappings& Sequence::GetExports () const
    {
      return exports;
    }
    
    RegisterPtr Sequence::GetExport (const uc::String& name) const
    {
      RegisterExpMappings::const_iterator it (exports.find (name));
      if (it != exports.end())
        return it->second;
      return RegisterPtr();
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
  } // namespace intermediate
} // namespace s1

