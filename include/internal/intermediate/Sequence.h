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

#ifndef __INTERMEDIATE_SEQUENCE_H__
#define __INTERMEDIATE_SEQUENCE_H__

#include "forwarddecl.h"
#include "parser/SemanticsHandler.h"

#include <boost/container/deque.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor;
    
    class Sequence
    {
      friend class SequenceBuilder;

      typedef boost::container::deque<SequenceOpPtr> OpsVector;
      OpsVector ops;
    public:
      typedef parser::SemanticsHandler::TypePtr TypePtr;
      
      class Register
        : public boost::intrusive_ref_counter<Register,
                                              boost::thread_unsafe_counter>
      {
      protected:
        friend class Sequence;
        
        uc::String originalName;
        unsigned int generation;
        uc::String name;
        
        TypePtr originalType;
      public:
        Register (const uc::String& name, const TypePtr& originalType);
        Register (const Register& other);
        
        const uc::String& GetOriginalName () const { return originalName; }
        const uc::String& GetName() const { return name; }
        void StealName (Register& other);
        
        const TypePtr& GetOriginalType () const { return originalType; }
      };
      typedef boost::intrusive_ptr<Register> RegisterPtr;
      typedef boost::unordered_set<RegisterPtr> RegisterSet;
      
      typedef boost::unordered_map<uc::String, RegisterPtr> IdentifierToRegMap;
      /// Get number of operations
      size_t GetNumOps() const;
      /// Get a specific op
      SequenceOpPtr GetOp(size_t index) const;
      
      void Visit (SequenceVisitor& visitor) const;
      void ReverseVisit (SequenceVisitor& visitor) const;
      
      typedef std::pair<uc::String, RegisterPtr> IdentRegPair;
      typedef std::vector<IdentRegPair> RegisterImpMappings;
      const RegisterImpMappings& GetImports () const { return imports; }

      typedef boost::unordered_map<uc::String, RegisterPtr> RegisterExpMappings;
      const RegisterExpMappings& GetExports () const;
      RegisterPtr GetExport (const uc::String& name) const;

      RegisterSet GetAllReadRegisters() const;
      RegisterSet GetAllWrittenRegisters() const;
      
      /**
       * Get imported registers from outer sequence.
       * \a identToReg is the identifier-to-register mapping, from the outer sequence builder,
       * to be used to resolve names.
       */
      RegisterSet GetImportOuterRegs (const IdentifierToRegMap& identToReg);
      /**
       * Get exported registers from outer sequence.
       * \a identToReg is the identifier-to-register mapping, from the outer sequence builder,
       * to be used to resolve names.
       */
      RegisterSet GetExportOuterRegs (const IdentifierToRegMap& identToReg);
    protected:
      RegisterImpMappings imports;
      RegisterExpMappings exports;
    };
    
    typedef Sequence::RegisterPtr RegisterPtr;
    typedef Sequence::RegisterSet RegisterSet;
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCE_H__
